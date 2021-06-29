// Project: libv.ui, File: src/libv/ui/component/detail/component.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <string>
// pro
#include <libv/ui/component/detail/core_ptr.hpp>
#include <libv/ui/event_host.hpp>
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/padding.hpp>
#include <libv/ui/property/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

/// Handler class for components
/// @Warning: Derived classes must not contain any data member
class Component {
private:
	core_ptr ptr = nullptr;

public:
	Component() noexcept = delete;

	explicit Component(core_ptr ptr_) noexcept;

public:
	Component(const Component& other) noexcept;
	Component(Component&& other) noexcept;
	Component& operator=(const Component& other) & noexcept;
	Component& operator=(Component&& other) & noexcept;
	~Component() noexcept;

public:
	[[nodiscard]] inline EventHostGeneral<Component> event() noexcept {
		return EventHostGeneral<Component>{*this};
	}

public:
	[[nodiscard]] inline CoreComponent& core() noexcept {
		return *ptr;
	}
	[[nodiscard]] inline const CoreComponent& core() const noexcept {
		return *ptr;
	}

public:
	[[nodiscard]] bool isFloatRegion() const noexcept;

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
	[[nodiscard]] libv::vec2f calculate_local_mouse_coord() const noexcept;

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

} // namespace ui
} // namespace libv
