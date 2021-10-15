// Project: libv.serial, File: src/libv/serial/codec.hpp

#pragma once

// libv
#include <libv/algo/linear_find.hpp>
#include <libv/utility/type_key.hpp>
// std
#include <cassert>
#include <span>
#include <vector>
// pro
#include <libv/serial/serial.hpp>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

template <typename Handler, typename Archive>
struct Codec {
	using MessageID = uint8_t;
	using TypeID = libv::type_uid;
	using Message = std::vector<std::byte>;
	using Message_view = std::span<const std::byte>;

	using IArchive = typename Archive::input;
	using OArchive = typename Archive::output;

	using EncodeFN = void(*)(OArchive& oar, const void* object_ptr);
	using DecodeFN = void(*)(IArchive& iar, void* handler_ptr);

private:
	struct Encoder {
		TypeID type; // Used for lookup during encoding and for 'is' checking
		MessageID id; // Used to save this id for the decoder
		EncodeFN encode_fn;
	};

	struct Decoder {
		TypeID type; // Used for lookup during and for 'is' checking
		MessageID id; // Used for lookup during decoding
		DecodeFN decode_fn;
	};

private:
	std::vector<Encoder> encoders;
	std::vector<Decoder> decoders;

public:
	template <MessageID MessageIndex, typename MessageType>
	void register_encoder_type() {
		auto encode_fn = +[](OArchive& oar, const void* object_ptr) {
			oar << LIBV_NVP_NAMED("message", *static_cast<const MessageType*>(object_ptr));
		};

		auto type = libv::type_key<MessageType>();
		encoders.emplace_back(type, MessageIndex, std::move(encode_fn));
	}

	template <MessageID MessageIndex, typename MessageType>
	void register_decoder_type() {
		auto decode_fn = +[](IArchive& iar, void* handler_ptr) {
			MessageType object;
			iar >> LIBV_NVP_NAMED("message", object);

			auto& handler = *static_cast<Handler*>(handler_ptr);
			handler.receive(std::move(object));
		};

		auto type = libv::type_key<MessageType>();
		decoders.emplace_back(type, MessageIndex, std::move(decode_fn));
	}

	template <MessageID MessageIndex, typename MessageType>
	void register_type() {
		register_decoder_type<MessageIndex, MessageType>();
		register_encoder_type<MessageIndex, MessageType>();
	}

private:
	[[nodiscard]] bool aux_is(TypeID type, Message_view message) const {
		IArchive iar(message);

		MessageID id;
		iar >> LIBV_NVP_NAMED("type", id);

		const auto eit = libv::linear_find_optional(encoders, id, &Encoder::id);
		if (eit)
			return eit->type == type;

		const auto dit = libv::linear_find_optional(decoders, id, &Decoder::id);
		if (dit)
			return dit->type == type;

		assert(false && "Encoder and Decoder are missing the provided type");
		return false;
	}

	[[nodiscard]] Message aux_encode(TypeID type, const void* object) const {
		Message message;

		{
			OArchive oar(message);

			auto it = libv::linear_find_optional(encoders, type, &Encoder::type);
			if (!it) {
				assert(false && "Encoder is missing for the provided type");
			} else {
				oar << LIBV_NVP_NAMED("type", it->id);
				it->encode_fn(oar, object);
			}
		}

		return message;
	}

	void aux_decode(void* handler, Message_view message) const {
		IArchive iar(message);

		MessageID id;
		iar >> LIBV_NVP_NAMED("type", id);

		auto it = libv::linear_find_optional(decoders, id, &Decoder::id);

		if (!it) {
			assert(false && "Decoder is missing for the provided type");
		} else {
			it->decode_fn(iar, handler);
		}
	}

public:
	template <typename T>
	[[nodiscard]] inline bool is(Message_view message) const {
		const auto type = libv::type_key<T>();
		return aux_is(type, message);
	}
	template <typename T>
	[[nodiscard]] inline bool is(std::string_view message_str) const {
		const auto type = libv::type_key<T>();
		const auto message = std::span<const std::byte>(
				reinterpret_cast<const std::byte*>(message_str.data()),
				reinterpret_cast<const std::byte*>(message_str.data() + message_str.size())
		);
		return aux_is(type, message);
	}

public:
	template <typename T>
	[[nodiscard]] inline Message encode(const T& object) const {
		const auto type = libv::type_key<T>();
		const auto object_ptr = static_cast<const void*>(&object);

		return aux_encode(type, object_ptr);
	}

	inline void decode(Handler& handler, Message_view message) const {
		const auto handler_ptr = static_cast<void*>(&handler);
		aux_decode(handler_ptr, message);
	}
	inline void decode(Handler& handler, std::string_view message_str) const {
		const auto handler_ptr = static_cast<void*>(&handler);
		const auto message = std::span<const std::byte>(
				reinterpret_cast<const std::byte*>(message_str.data()),
				reinterpret_cast<const std::byte*>(message_str.data() + message_str.size())
		);
		aux_decode(handler_ptr, message);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename Handler, typename Archive>
struct CodecServer : public Codec<Handler, Archive> {
	template <typename... MessageGroups>
	explicit inline CodecServer(MessageGroups...) {
		(register_message_types<MessageGroups>(), ...);
	}

public:
	template <typename MessageGroup>
	inline void register_message_types() {
		MessageGroup::message_types(*this);
	}

public:
	template <Codec<Handler, Archive>::MessageID MessageIndex, typename MessageType>
	inline void type_client() {
		this->template register_decoder_type<MessageIndex, MessageType>();
	}

	template <Codec<Handler, Archive>::MessageID MessageIndex, typename MessageType>
	inline void type_server() {
		this->template register_encoder_type<MessageIndex, MessageType>();
	}
};

// ---

template <typename Handler, typename Archive>
struct CodecClient : public Codec<Handler, Archive> {
	template <typename... MessageGroups>
	explicit inline CodecClient(MessageGroups...) {
		(register_message_types<MessageGroups>(), ...);
	}

public:
	template <typename MessageGroup>
	inline void register_message_types() {
		MessageGroup::message_types(*this);
	}

public:
	template <Codec<Handler, Archive>::MessageID MessageIndex, typename MessageType>
	inline void type_client() {
		this->template register_encoder_type<MessageIndex, MessageType>();
	}

	template <Codec<Handler, Archive>::MessageID MessageIndex, typename MessageType>
	inline void type_server() {
		this->template register_decoder_type<MessageIndex, MessageType>();
	}
};

// ---

template <typename Handler, typename Archive>
struct CodecCommon : public Codec<Handler, Archive> {
	template <typename... MessageGroups>
	explicit inline CodecCommon(MessageGroups...) {
		(register_message_types<MessageGroups>(), ...);
	}

public:
	template <typename MessageGroup>
	inline void register_message_types() {
		MessageGroup::message_types(*this);
	}

public:
	template <Codec<Handler, Archive>::MessageID MessageIndex, typename MessageType>
	inline void type_client() {
		this->template register_encoder_type<MessageIndex, MessageType>();
		this->template register_decoder_type<MessageIndex, MessageType>();
	}

	template <Codec<Handler, Archive>::MessageID MessageIndex, typename MessageType>
	inline void type_server() {
		this->template register_encoder_type<MessageIndex, MessageType>();
		this->template register_decoder_type<MessageIndex, MessageType>();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
