// Project: libv.net, File: src/libv/net/detail/handler_guard.hpp

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
	template <typename K, typename L> friend class HandlerGuard;
	template <typename K> friend class ::std::hash;

private:
	raw_ptr<BaseHandler> handler_;
	raw_ptr<T> user_object;

public:
	template <typename... Args>
	inline explicit HandlerGuard(IOContext& io_context, Args&&... args) {
		libv::net::detail::current_io_context = &io_context;

		auto user_object_up = std::make_unique<T>(std::forward<Args>(args)...);
		handler_ = user_object_up.get();
		user_object = user_object_up.get();

		handler_->inject_handler(std::move(user_object_up));
		handler_->increment_ref_count();
	}
	inline explicit HandlerGuard(raw_ptr<BaseHandler> handler_ptr, raw_ptr<T> user_object_) noexcept :
		handler_(handler_ptr),
		user_object(user_object_) {
		handler_->increment_ref_count();
	}

public:
	template <typename K>
	inline explicit(false) HandlerGuard(const HandlerGuard<BaseHandler, K>& orig) noexcept
			requires std::derived_from<K, T> || std::same_as<const K*, T*> || std::same_as<T, void> :
		handler_(orig.handler_),
		user_object(orig.user_object) {
		handler_->increment_ref_count();
	}
	template <typename K>
	inline explicit(false) HandlerGuard(HandlerGuard<BaseHandler, K>&& orig) noexcept
			requires std::derived_from<K, T> || std::same_as<const K*, T*> || std::same_as<T, void> :
		handler_(orig.handler_),
		user_object(orig.user_object) {
		orig.handler_ = nullptr;
		orig.user_object = nullptr;
	}

public:
	inline HandlerGuard(const HandlerGuard& orig) noexcept :
		handler_(orig.handler_),
		user_object(orig.user_object) {
		handler_->increment_ref_count();
	}
	inline HandlerGuard(HandlerGuard&& orig) noexcept :
		handler_(orig.handler_),
		user_object(orig.user_object) {
		orig.handler_ = nullptr;
		orig.user_object = nullptr;
	}
	inline HandlerGuard& operator=(const HandlerGuard& orig) & noexcept {
		if (this == &orig)
			return *this;

		if (handler_ != nullptr)
			handler_->decrement_ref_count();

		handler_ = orig.handler_;
		user_object = orig.user_object;

		if (handler_ != nullptr)
			handler_->increment_ref_count();

		return *this;
	}
	inline HandlerGuard& operator=(HandlerGuard&& orig) & noexcept {
		// NOTE: using this == &other instead of ptr == other.ptr to have all moved from state the same (and be reset)
		if (this == &orig)
			return *this;

		if (handler_ != nullptr)
			handler_->decrement_ref_count();

		handler_ = std::move(orig.handler_);
		user_object = std::move(orig.user_object);
		orig.handler_ = nullptr;
		orig.user_object = nullptr;
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
		user_object = nullptr;
	}

public:
	[[nodiscard]] explicit inline operator bool() const noexcept {
		return user_object != nullptr;
	}

public:
	inline T* operator->() const noexcept
			requires (!std::is_void_v<T>) {
		assert(user_object != nullptr);
		return user_object;
	}

	template <typename TT = T>
			requires (!std::is_void_v<TT>)
	[[nodiscard]] inline TT& operator*() const noexcept {
		assert(user_object != nullptr);
		return *user_object;
	}

	[[nodiscard]] inline BaseHandler& handler() const noexcept {
		assert(handler_ != nullptr);
		return *handler_;
	}

public:
	[[nodiscard]] friend constexpr inline bool operator==(const HandlerGuard& lhs, const HandlerGuard& rhs) noexcept {
		return lhs.handler_ == rhs.handler_;
	}
	[[nodiscard]] friend constexpr inline bool operator<(const HandlerGuard& lhs, const HandlerGuard& rhs) noexcept {
		return lhs.handler_ < rhs.handler_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace detail
} // namespace mtcp
} // namespace net
} // namespace libv

namespace std { // ---------------------------------------------------------------------------------

template <typename BaseHandler, typename T>
struct hash<libv::net::mtcp::detail::HandlerGuard<BaseHandler, T>> {
	[[nodiscard]] static constexpr std::size_t operator()(const libv::net::mtcp::detail::HandlerGuard<BaseHandler, T>& t) const noexcept {
		return std::hash<BaseHandler*>{}(t.handler_);
	}
};

} // -----------------------------------------------------------------------------------------------

