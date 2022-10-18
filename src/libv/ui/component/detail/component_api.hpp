// Project: libv.ui, File: src/libv/ui/component/detail/component_api.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/ui/component/detail/component.hpp>
#include <libv/ui/component/detail/generate_name.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T, typename... Args>
[[nodiscard]] LIBV_FORCE_INLINE T* create_core_ptr(Args&&... args) noexcept {
	return new T(std::forward<Args>(args)...);
}

[[nodiscard]] std::string generate_component_name(std::string_view type, std::size_t index) noexcept;

// -------------------------------------------------------------------------------------------------

template <typename BaseT, typename HandlerT, typename CoreT, template <typename> typename EventHostT>
struct ComponentAPI : public BaseT {
protected:
	static inline std::size_t nextID = 0;

protected:
	using Base = BaseT;
	using BaseAPI = ComponentAPI;

public:
	using CoreType = CoreT;

public:
	explicit LIBV_FORCE_INLINE ComponentAPI(std::nullptr_t) noexcept :
		BaseT(nullptr) { }

	explicit LIBV_FORCE_INLINE ComponentAPI(core_ptr ptr) noexcept :
		BaseT(ptr) { }

	template <typename... Args>
	explicit LIBV_FORCE_INLINE ComponentAPI(std::string name, Args&&... args) :
		ComponentAPI(HandlerT::create_core(std::move(name), std::forward<Args>(args)...)) { }

	template <typename... Args>
	explicit LIBV_FORCE_INLINE ComponentAPI(GenerateName_t = {}, Args&&... args) :
		ComponentAPI(generate_component_name(HandlerT::component_type, nextID++), std::forward<Args>(args)...) { }

public:
	[[nodiscard]] static LIBV_FORCE_INLINE HandlerT n(std::string name) {
		return HandlerT{std::move(name)};
	}
	[[nodiscard]] static LIBV_FORCE_INLINE HandlerT s(std::string_view style) {
		HandlerT handler{};
		handler.style(style);
		return handler;
	}
	template <typename... Args>
	[[nodiscard]] static LIBV_FORCE_INLINE HandlerT a(Args&&... args) {
		return HandlerT{generate_component_name(HandlerT::component_type, nextID++), std::forward<Args>(args)...};
	}
	[[nodiscard]] static LIBV_FORCE_INLINE HandlerT ns(std::string name, std::string_view style) {
		HandlerT handler{std::move(name)};
		handler.style(style);
		return handler;
	}
	template <typename... Args>
	[[nodiscard]] static LIBV_FORCE_INLINE HandlerT na(std::string name, Args&&... args) {
		return HandlerT{std::move(name), std::forward<Args>(args)...};
	}
	template <typename... Args>
	[[nodiscard]] static LIBV_FORCE_INLINE HandlerT sa(std::string_view style, Args&&... args) {
		HandlerT handler{generate_component_name(HandlerT::component_type, nextID++), std::forward<Args>(args)...};
		handler.style(style);
		return handler;
	}
	template <typename... Args>
	[[nodiscard]] static LIBV_FORCE_INLINE HandlerT nsa(std::string name, std::string_view style, Args&&... args) {
		HandlerT handler{std::move(name), std::forward<Args>(args)...};
		handler.style(style);
		return handler;
	}

public:
	[[nodiscard]] static LIBV_FORCE_INLINE HandlerT from_core(core_ptr ptr) noexcept {
		return HandlerT{ptr};
	}

protected:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE CoreT& self() noexcept {
		return static_cast<CoreT&>(this->core());
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const CoreT& self() const noexcept {
		return static_cast<const CoreT&>(this->core());
	}

public:
	[[nodiscard]] LIBV_FORCE_INLINE EventHostT<HandlerT> event() noexcept {
		return EventHostT<HandlerT>{static_cast<HandlerT&>(*this)};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
