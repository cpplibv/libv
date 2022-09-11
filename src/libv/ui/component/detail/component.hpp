// Project: libv.ui, File: src/libv/ui/component/detail/component.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/function_ref.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/ui/component/detail/core_ptr.hpp>
#include <libv/ui/component/detail/flag.hpp>
#include <libv/ui/event/detail/event_reentry_guard.hpp>
#include <libv/ui/event_host.hpp>
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/padding.hpp>
#include <libv/ui/property/size.hpp>
#include <libv/ui/property/z_index_offset.hpp>
#include <libv/ui/style_state.hpp>
//#include <libv/ui/style_fwd.hpp>
//#include <libv/utility/memory/intrusive_ptr.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

/// Handler class for components
/// @Warning: Derived classes must not contain any data member
class Component {
public:
	using CoreT = CoreComponent;

private:
	core_ptr ptr_ = nullptr;

public:
	constexpr inline Component() noexcept = default;
	explicit constexpr inline Component(std::nullptr_t) noexcept { }
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

	/// Reentry-locks the source and reentry-tests the target.
	/// ReentryGuard evaluates to true if entry is allowed (without causing a reentry)
	[[nodiscard]] ReentryGuard event_reentry_guard(const void* source, const void* target) const noexcept;

public:
	[[nodiscard]] inline CoreComponent& core() noexcept {
		return *ptr_;
	}
	[[nodiscard]] inline const CoreComponent& core() const noexcept {
		return *ptr_;
	}
	[[nodiscard]] inline const void* ptr() const noexcept {
		return ptr_;
	}

public:
	[[nodiscard]] constexpr explicit inline operator bool() const noexcept {
		return ptr_ != nullptr;
	}
	[[nodiscard]] constexpr inline bool operator!() const noexcept {
		return ptr_ == nullptr;
	}

public:
	[[nodiscard]] Flag_t flags() const noexcept;

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

	void z_index_offset(ZIndexOffset value) noexcept;
	[[nodiscard]] ZIndexOffset z_index_offset() const noexcept;

	// --- Margin ---

	/// x: left, y: down, z: right, w: top
	void margin(Margin value) noexcept;
	inline void margin(float left_down_right_top) noexcept {
		margin(Margin{left_down_right_top, left_down_right_top, left_down_right_top, left_down_right_top});
	}
	inline void margin(float left_right, float down_top) noexcept {
		margin(Margin{left_right, down_top, left_right, down_top});
	}
	inline void margin(float left, float down, float right, float top) noexcept {
		margin(Margin{left, down, right, top});
	}
	/// x: left, y: down, z: right, w: top
	[[nodiscard]] Margin margin() const noexcept;
	/// x: left, y: down
	[[nodiscard]] inline libv::vec2f margin_LB() const noexcept {
		return xy(margin());
	}
	/// x: left, y: down, z: 0
	[[nodiscard]] inline libv::vec3f margin_LB3() const noexcept {
		return {margin_LB(), 0.0f};
	}
	/// x: right, y: top
	[[nodiscard]] inline libv::vec2f margin_RT() const noexcept {
		return zw(margin());
	}
	/// x: right, y: top, z: 0
	[[nodiscard]] inline libv::vec3f margin_RT3() const noexcept {
		return {margin_RT(), 0.0f};
	}
	/// x: left + right, y: down + top
	[[nodiscard]] inline libv::vec2f margin_size() const noexcept {
		return margin_LB() + margin_RT();
	}
	/// x: left + right, y: down + top, z: 0
	[[nodiscard]] inline libv::vec3f margin_size3() const noexcept {
		return {margin_size(), 0.0f};
	}

	// --- Padding ---

	/// x: left, y: down, z: right, w: top
	void padding(Padding value) noexcept;
	inline void padding(float left_down_right_top) noexcept {
		padding(Padding{left_down_right_top, left_down_right_top, left_down_right_top, left_down_right_top});
	}
	inline void padding(float left_right, float down_top) noexcept {
		padding(Padding{left_right, down_top, left_right, down_top});
	}
	inline void padding(float left, float down, float right, float top) noexcept {
		padding(Padding{left, down, right, top});
	}
	/// x: left, y: down, z: right, w: top
	[[nodiscard]] Padding padding() const noexcept;
	/// x: left, y: down
	[[nodiscard]] inline libv::vec2f padding_LB() const noexcept {
		return xy(padding());
	}
	/// x: left, y: down, z: 0
	[[nodiscard]] inline libv::vec3f padding_LB3() const noexcept {
		return {padding_LB(), 0.0f};
	}
	/// x: right, y: top
	[[nodiscard]] inline libv::vec2f padding_RT() const noexcept {
		return zw(padding());
	}
	/// x: right, y: top, z: 0
	[[nodiscard]] inline libv::vec3f padding_RT3() const noexcept {
		return {padding_RT(), 0.0f};
	}
	/// x: left + right, y: down + top
	[[nodiscard]] inline libv::vec2f padding_size() const noexcept {
		return padding_LB() + padding_RT();
	}
	/// x: left + right, y: down + top, z: 0
	[[nodiscard]] inline libv::vec3f padding_size3() const noexcept {
		return {padding_size(), 0.0f};
	}

public:
	[[nodiscard]] libv::vec2f calculate_local_mouse_coord() const noexcept;

public:
	void markRemove();
//	void style(libv::intrusive_ptr<Style> style) noexcept;
	void style(std::string_view style_name);

	void style_state(StyleState state, bool value) noexcept;

public:
	void focus();

	void enable(bool value);
	[[nodiscard]] bool enable() const;
	void show(bool value);
	[[nodiscard]] bool show() const;

public:
	void foreach_children(libv::function_ref<void(Component&)> callback);
	void foreach_recursive_children(libv::function_ref<void(Component&)> callback);

public:
	[[nodiscard]] inline bool operator==(const Component& other) const noexcept = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
