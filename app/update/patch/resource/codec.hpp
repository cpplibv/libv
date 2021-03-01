// Project: libv, File: app/update/patch/resource/codec.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/serial/serial.hpp>
// std
#include <span>
#include <typeindex>
#include <vector>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

template <typename Handler, typename Archive>
struct Codec {
	using MessageID = uint8_t;
	using TypeID = std::type_index;
	using Message = std::vector<std::byte>;
	using Message_view = std::span<std::byte>;

	using DecodeFN = void(*)(Handler&, Message_view);
	using EncodeFN = Message(*)(const void* object);

	using IArchive = typename Archive::input;
	using OArchive = typename Archive::output;

private:
	struct Encoder {
		TypeID type;
		EncodeFN encode_fn;
	};

	struct Decoder {
		MessageID id;
		DecodeFN decode_fn;
	};

private:
	std::vector<Encoder> encoders;
	std::vector<Decoder> decoders;

public:
	template <MessageID MessageIndex, typename MessageType>
	void register_encoder_type() {
		auto encode_fn = [](const void* object) {
			Message message;
			IArchive oar(message);
			oar << LIBV_NVP_NAMED("message", *static_cast<const MessageType*>(object));

			return message;
		};

		auto type = std::type_index(typeid(MessageType));
		encoders.emplace_back(type, std::move(encode_fn));
	}

	template <MessageID MessageIndex, typename MessageType>
	void register_decoder_type() {
		auto decode_fn = [](Handler& handler, Message_view message) {
			MessageType object;
			IArchive iar(message);
			iar >> LIBV_NVP_NAMED("message", object);

			handler.receive(std::move(object));
		};

		decoders.emplace_back(MessageIndex, std::move(decode_fn));
	}

public:
	void decode(Handler& handler, Message message) const {
//		it = find();
//
//		if (!it)
////			assert
//			return;
//
//		it->decode_fn(handler, message);
	}

private:
	Message aux_encode(std::type_index type, const void* object) const {
		Message message;
//		it = find(type);
//
//		if (!it)
////			assert
//			return;
//
//		return it->encode_fn();
		return message;
	}

public:
	template <typename T>
	[[nodiscard]] inline Message encode(const T& object) const {
		const auto type = std::type_index(typeid(T));
		const auto object_ptr = static_cast<const void*>(&object);

		return aux_encode(type, object_ptr);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename Handler, typename Archive>
struct CodecServer : public Codec<Handler, Archive> {
	template <typename... MessageGroups>
	inline CodecServer(MessageGroups...) {
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

template <typename Handler, typename Archive>
struct CodecClient : public Codec<Handler, Archive> {
	template <typename... MessageGroups>
	inline CodecClient(MessageGroups...) {
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

} // namespace update
} // namespace libv
