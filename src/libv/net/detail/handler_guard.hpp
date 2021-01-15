// Project: libv.net, File: src/libv/net/mtcp/connection2.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/net/fwd.hpp>
// std
#include <cassert>
#include <concepts>
#include <memory>
// pro
#include <libv/net/detail/current_io_context.hpp>


namespace libv {
namespace net {
namespace mtcp {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename BaseHandler, typename T = void>
//		requires std::derived_from<T, ConnectionHandler<T>>
class HandlerGuard {
private:
	template <typename K> using raw_ptr = K*;
	template <typename B, typename K> friend class HandlerGuard;

private:
	raw_ptr<BaseHandler> handler_;
	raw_ptr<T> object;

public:
	template <typename... Args>
	inline explicit HandlerGuard(IOContext& io_context, Args&&... args) {
		libv::net::detail::current_io_context = &io_context;

		auto object_up = std::make_unique<T>(std::forward<Args>(args)...);
		handler_ = object_up.get();
		object = object_up.get();

		handler_->inject_handler(std::move(object_up));
		handler_->increment_ref_count();
	}
	inline explicit HandlerGuard(raw_ptr<BaseHandler> handler_ptr, raw_ptr<T> object_) noexcept :
		handler_(handler_ptr),
		object(object_) {
		handler_->increment_ref_count();
	}

public:
	template <typename K>
	inline /*implicit*/ HandlerGuard(const HandlerGuard<BaseHandler, K>& orig) noexcept
			requires std::derived_from<K, T> || std::same_as<const K*, T*> || std::same_as<T, void> :
		handler_(orig.handler_),
		object(orig.object) {
		handler_->increment_ref_count();
	}
	template <typename K>
	inline /*implicit*/ HandlerGuard(HandlerGuard<BaseHandler, K>&& orig) noexcept
			requires std::derived_from<K, T> || std::same_as<const K*, T*> || std::same_as<T, void> :
		handler_(orig.handler_),
		object(orig.object) {
		orig.handler_ = nullptr;
		orig.object = nullptr;
	}

public:
	inline HandlerGuard(const HandlerGuard& orig) noexcept :
		handler_(orig.handler_),
		object(orig.object) {
		handler_->increment_ref_count();
	}
	inline HandlerGuard(HandlerGuard&& orig) noexcept :
		handler_(orig.handler_),
		object(orig.object) {
		orig.handler_ = nullptr;
		orig.object = nullptr;
	}
	inline HandlerGuard& operator=(const HandlerGuard& orig) & noexcept {
		handler_ = orig.handler_;
		object = orig.object;
		handler_->increment_ref_count();
		return *this;
	}
	inline HandlerGuard& operator=(HandlerGuard&& orig) & noexcept {
		handler_ = std::move(orig.handler_);
		object = std::move(orig.object);
		orig.handler_ = nullptr;
		orig.object = nullptr;
		return *this;
	}
	virtual ~HandlerGuard() {
		if (handler_ != nullptr)
			handler_->decrement_ref_count();
	}

public:
	inline void clear() noexcept {
		assert(handler_ != nullptr);
		handler_->decrement_ref_count();
		handler_ = nullptr;
		object = nullptr;
	}

public:
	inline T* operator->() const noexcept {
		assert(object != nullptr);
		return object;
	}

	[[nodiscard]] inline BaseHandler& handler() const noexcept {
		assert(handler_ != nullptr);
		return *handler_;
	}

public:
	[[nodiscard]] friend constexpr inline bool operator<(const HandlerGuard& lhs, const HandlerGuard& rhs) noexcept {
		return lhs.object < rhs.object;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace detail
} // namespace mtcp
} // namespace net
} // namespace libv
