// Project: libv.serial, File: src/libv/serial/codec2.hpp

#pragma once

// libv
#include <libv/algo/linear_find.hpp>
#include <libv/utility/type_key.hpp>
// std
#include <cassert>
#include <exception>
// pro
#include <libv/serial/serial.hpp>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

template <typename Handler, typename Archive>
struct Codec2 {
	using MessageID = uint8_t;
	using TypeID = libv::type_uid;

	using IArchive = typename Archive::input;
	using OArchive = typename Archive::output;

	using EncodeFN = void(*)(OArchive& oar, const void* object_ptr);
	using DecodeFN = void(*)(IArchive& iar, void* handler_ptr);

public:
	struct UnknownTypeException : std::exception {
		TypeID type;

		explicit inline UnknownTypeException(type_uid type) : type(type) {}

		[[nodiscard]] virtual const char* what() const noexcept override {
			return "Referenced type is not registered in the codec";
		}
	};

	struct UnknownIDException : std::exception {
		MessageID id;

		explicit inline UnknownIDException(MessageID id) : id(id) {}

		[[nodiscard]] virtual const char* what() const noexcept override {
			return "Referenced id is not registered in the codec";
		}
	};

private:
	struct TypeEntry {
		TypeID type; // Used for lookup during encoding and for 'is' checking
		MessageID id; // Used to save this id for the decoder and for lookup during decoding
		EncodeFN encode_fn;
		DecodeFN decode_fn;
	};

private:
	std::vector<TypeEntry> types;

public:
	template <typename MessageType>
	void register_type(MessageID MessageIndex) {
		auto type = libv::type_key<MessageType>();

		auto encode_fn = +[](OArchive& oar, const void* object_ptr) {
			oar << LIBV_NVP_NAMED("message", *static_cast<const MessageType*>(object_ptr));
		};

		auto decode_fn = +[](IArchive& iar, void* handler_ptr) {
			MessageType object;
			iar >> LIBV_NVP_NAMED("message", object);

			auto& handler = *static_cast<Handler*>(handler_ptr);
			handler.receive(std::move(object));
		};

		types.emplace_back(type, MessageIndex, std::move(encode_fn), std::move(decode_fn));
	}

private:
	[[nodiscard]] bool aux_is(IArchive& iar, TypeID type) const {
		MessageID id;
		iar >> LIBV_NVP_NAMED("type", id);

		const auto it = libv::linear_find_optional(types, id, &TypeEntry::id);
		if (!it)
			throw UnknownTypeException(type);
		else
			return it->type == type;
	}

	void aux_encode(OArchive& oar, TypeID type, const void* object) const {
		auto it = libv::linear_find_optional(types, type, &TypeEntry::type);
		if (!it) {
			// Hard error is acceptable, we are about to create an invalid message
			assert(false && "Encode is missing for the provided type");
		} else {
			oar << LIBV_NVP_NAMED("type", it->id);
			it->encode_fn(oar, object);
		}
	}

	void aux_decode(IArchive& iar, void* handler) const {
		MessageID id;
		iar >> LIBV_NVP_NAMED("type", id);

		auto it = libv::linear_find_optional(types, id, &TypeEntry::id);

		if (!it)
			throw UnknownIDException(id);
		else
			it->decode_fn(iar, handler);
	}

public:
	template <typename T>
	[[nodiscard]] inline bool is(IArchive& iar) const {
		return aux_is(iar, libv::type_key<T>());
	}

	template <typename T>
	inline void encode(OArchive& oar, const T& object) const {
		aux_encode(oar, libv::type_key<T>(), static_cast<const void*>(&object));
	}

	inline void decode(IArchive& iar, Handler& handler) const {
		aux_decode(iar, static_cast<void*>(&handler));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
