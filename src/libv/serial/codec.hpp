// Project: libv.serial, File: src/libv/serial/codec.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/algo/linear_find.hpp>
// std
#include <span>
#include <typeindex>
#include <vector>
#include <sstream> // TODO P4: Remove dependency once libv.serial supports non stream io
// pro
#include <libv/serial/serial.hpp>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

template <typename Handler, typename Archive>
struct Codec {
	using MessageID = uint8_t;
	using TypeID = std::type_index;
	using Message = std::vector<std::byte>;
	using Message_view = std::span<const std::byte>;

	using IArchive = typename Archive::input;
	using OArchive = typename Archive::output;

	using EncodeFN = void(*)(OArchive& oar, const void* object_ptr);
	using DecodeFN = void(*)(IArchive& iar, void* handler_ptr);

private:
	struct Encoder {
		TypeID type; // Used for lookup during encoding
		MessageID id; // Used to save this id for the decoder
		EncodeFN encode_fn;
	};

	struct Decoder {
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

		auto type = std::type_index(typeid(MessageType));
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

		decoders.emplace_back(MessageIndex, std::move(decode_fn));
	}

private:
	Message aux_encode(std::type_index type, const void* object) const {
		// TODO P4: Performance issue with ostringstream, cause is libv.serial archive API
		std::ostringstream message; // <<< Implement network/serial std::byte support
//		Message message;

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

//		return message;
		auto tmp = std::move(message).str();
		return std::vector<std::byte>(
				reinterpret_cast<std::byte*>(tmp.data()),
				reinterpret_cast<std::byte*>(tmp.data() + tmp.size())
		);
	}

//	void aux_decode(void* handler, Message_view message) const {
	void aux_decode(void* handler, Message_view tmp) const {
		// TODO P4: Performance issue with ostringstream, cause is libv.serial archive API
		std::istringstream message(std::string( // <<< Implement network/serial std::byte support
				reinterpret_cast<const char*>(tmp.data()),
				reinterpret_cast<const char*>(tmp.data() + tmp.size())
		));
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
	[[nodiscard]] inline Message encode(const T& object) const {
		const auto type = std::type_index(typeid(T));
		const auto object_ptr = static_cast<const void*>(&object);

		return aux_encode(type, object_ptr);
	}

	void decode(Handler& handler, Message_view message) const {
		const auto handler_ptr = static_cast<void*>(&handler);

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

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv