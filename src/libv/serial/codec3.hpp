// Project: libv.serial, File: src/libv/serial/codec3.hpp

#pragma once

// std
#include <type_traits>
// pro
#include <libv/serial/serial.hpp>
#include <libv/serial/codec_message_id.hpp>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

template <typename CRTP>
class Codec3 {
public:
	using MessageID = CodecMessageID;

	struct RejectMessage {};

public:
	struct UnexpectedIDException : std::exception {
		MessageID id;

		explicit inline UnexpectedIDException(MessageID id = MessageID{0}) : id(id) {}

		[[nodiscard]] virtual const char* what() const noexcept override {
			return "Unexpected id is not registered in the codec";
		}
	};

protected:
	struct TypeAccessEntry {
		MessageID id;
		void(*decode_fn)(void* ar, const void* ob);
	};

private:
	template <MessageID id>
	static void unexpected_message_handler(void*, const void*) {
		throw UnexpectedIDException(id);
	}

protected:
	template <typename Archive, typename Handler, typename T>
	static constexpr TypeAccessEntry create_entry() {
		if constexpr (!std::is_invocable_v<Handler, T&&>)
			return {T::id, unexpected_message_handler<T::id>};

		else if constexpr (std::is_same_v<std::invoke_result_t<Handler, T&&>, RejectMessage>)
			return {T::id, unexpected_message_handler<T::id>};

		else
			return {T::id, +[](void* iar_ptr, const void* handler_ptr) {
				auto& iar = *static_cast<Archive*>(iar_ptr);
				auto& handler = *static_cast<Handler*>(const_cast<void*>(handler_ptr));

				T object;
				iar(LIBV_NVP_NAMED("message", object));

				handler(std::move(object));
			}};
	}

//	template <typename Archive, typename Handler, MessageID id>
//	static constexpr TypeAccessEntry create_entry_bypass() {
//		return {id, +[](void* iar_ptr, const void* handler_ptr) {
//			auto& iar = *static_cast<Archive*>(iar_ptr);
//			auto& handler = *static_cast<Handler*>(const_cast<void*>(handler_ptr));
//
//			handler(iar);
//		}};
//	}

public:
	template <typename Archive, typename F>
	auto decode(Archive& iar, F&& f) {
		MessageID id;
		iar(LIBV_NVP_NAMED("type", id));

		for (const auto& c : CRTP::template gen_table<Archive, std::decay_t<F>>::types)
			if (c.id == id) {
				c.decode_fn(&iar, &f);
				return;
			}

		throw UnexpectedIDException(id);
	}

	template <typename Archive, typename T>
	auto encode(Archive& oar, T&& t) {
		oar(LIBV_NVP_NAMED("type", std::remove_cvref_t<T>::id));
		oar(LIBV_NVP_NAMED("message", t));
	}

//	template <typename Archive>
//	auto encode_bypass(Archive& oar, MessageID id) {
//		oar(LIBV_NVP_NAMED("type", id));
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
