// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/ui/event_host.hpp>
#include <libv/ui/generate_name.hpp>
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/padding.hpp>
#include <libv/ui/property/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using core_ptr = class CoreComponent*;

template <typename T, typename... Args>
[[nodiscard]] inline core_ptr create_core_ptr(Args&&... args) noexcept {
	return new T(std::forward<Args>(args)...);
}

// -------------------------------------------------------------------------------------------------

/// Handler class for components
/// Warning: Derived classes must not contain any data member
class Component {
private:
	core_ptr ptr = nullptr;

//protected:
//	Component() noexcept = default;
public:
	Component() noexcept = delete;

public:
	Component(const Component& other) noexcept;
	Component(Component&& other) noexcept;
	Component& operator=(const Component& other) & noexcept;
	Component& operator=(Component&& other) & noexcept;
	~Component() noexcept;

protected:
	explicit Component(core_ptr ptr_) noexcept;

public:
	[[nodiscard]] inline EventHostGeneral<Component> event() noexcept {
		return {*this};
	}

public:
	[[nodiscard]] inline CoreComponent& core() noexcept {
		return *ptr;
	}
	[[nodiscard]] inline const CoreComponent& core() const noexcept {
		return *ptr;
	}

public:
	[[nodiscard]] const std::string& name() const noexcept;
	[[nodiscard]] std::string path() const noexcept;

	[[nodiscard]] libv::vec3f layout_position() const noexcept;
	[[nodiscard]] libv::vec2f layout_position2() const noexcept;
	[[nodiscard]] libv::vec3f layout_size() const noexcept;
	[[nodiscard]] libv::vec2f layout_size2() const noexcept;

public:
	void size(const Size& value) noexcept;
	[[nodiscard]] const Size& size() const noexcept;

	void anchor(Anchor value) noexcept;
	[[nodiscard]] Anchor anchor() const noexcept;

	/// x: left, y: down, z: right, w: top
	void margin(Margin value) noexcept;
	/// x: left, y: down, z: right, w: top
	[[nodiscard]] Margin margin() const noexcept;

	/// x: left, y: down, z: right, w: top
	void padding(Padding value) noexcept;
	/// x: left, y: down, z: right, w: top
	[[nodiscard]] Padding padding() const noexcept;

public:
	void markRemove();

public:
	friend inline bool operator==(const Component& lhs, const Component& rhs) noexcept {
		return lhs.ptr == rhs.ptr;
	}

	friend inline bool operator!=(const Component& lhs, const Component& rhs) noexcept {
		return lhs.ptr != rhs.ptr;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename ComponentT, typename EventHostT>
struct ComponentHandler : public Component {
private:
	static inline size_t nextID = 0;

protected:
	template <typename DelayedT = ComponentT>
	explicit inline ComponentHandler(std::string name) :
		Component(create_core_ptr<DelayedT>(std::move(name))) { }

	template <typename DelayedT = ComponentT>
	explicit inline ComponentHandler(GenerateName_t = {}, const std::string_view type = "component") :
		Component(create_core_ptr<DelayedT>(GenerateName, type, nextID++)) { }

	explicit inline ComponentHandler(core_ptr ptr) noexcept :
		Component(ptr) {

		assert(dynamic_cast<ComponentT*>(ptr) != nullptr && "Invalid component pointer initialization");
	}

protected:
	[[nodiscard]] constexpr inline ComponentT& self() noexcept {
		return static_cast<ComponentT&>(core());
	}
	[[nodiscard]] constexpr inline const ComponentT& self() const noexcept {
		return static_cast<const ComponentT&>(core());
	}

public:
	[[nodiscard]] inline EventHostT event() noexcept {
		return EventHostT{static_cast<typename EventHostT::component_type&>(*this)};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
