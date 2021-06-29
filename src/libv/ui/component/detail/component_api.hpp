// Project: libv.ui, File: src/libv/ui/component/detail/component_api.hpp, Author: Császár Mátyás [Vader]

#pragma once

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
[[nodiscard]] inline core_ptr create_core_ptr(Args&&... args) noexcept {
	return new T(std::forward<Args>(args)...);
}

[[nodiscard]] std::string generate_component_name(std::string_view type, size_t index) noexcept;

// -------------------------------------------------------------------------------------------------

template <typename BaseT, typename HandlerT, typename CoreT, template <typename> typename EventHostT>
struct ComponentAPI : public BaseT {
private:
	static inline size_t nextID = 0;

protected:
	using Base = BaseT;

public:
	explicit inline ComponentAPI(core_ptr ptr) noexcept :
		BaseT(ptr) { }

	template <typename... Args>
	explicit inline ComponentAPI(std::string name, Args&&... args) :
		ComponentAPI(HandlerT::create_core(std::move(name), std::forward<Args>(args)...)) { }

	template <typename... Args>
	explicit inline ComponentAPI(GenerateName_t = {}, Args&&... args) :
		ComponentAPI(generate_component_name(HandlerT::component_type, nextID++), std::forward<Args>(args)...) { }

protected:
	[[nodiscard]] constexpr inline CoreT& self() noexcept {
		return static_cast<CoreT&>(this->core());
	}
	[[nodiscard]] constexpr inline const CoreT& self() const noexcept {
		return static_cast<const CoreT&>(this->core());
	}

public:
	[[nodiscard]] inline EventHostT<HandlerT> event() noexcept {
		return EventHostT<HandlerT>{static_cast<HandlerT&>(*this)};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
