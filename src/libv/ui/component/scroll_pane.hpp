// Project: libv.ui, File: src/libv/ui/component/scroll_pane.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/scroll_area_mode.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct EventScrollArea : BaseEvent {
	libv::vec2f old_position;
	libv::vec2f old_size;

	constexpr inline EventScrollArea(libv::vec2f old_position, libv::vec2f old_size) noexcept :
		old_position(old_position),
		old_size(old_size) {}
};

template <typename ComponentT>
struct EventHostScrollArea : EventHostGeneral<ComponentT> {
	BasicEventProxy<ComponentT, EventScrollArea> area;

	explicit inline EventHostScrollArea(ComponentT& core) : EventHostGeneral<ComponentT>(core),
			area(core) {}
};

// -------------------------------------------------------------------------------------------------

//class ScrollPane :
//		public ComponentHandler<class CoreScrollArea>,
//		public EventHost<EventHostGeneral<ScrollPane>>> {
class ScrollArea : public ComponentHandler<class CoreScrollArea, EventHostScrollArea<ScrollArea>> {
public:
	explicit ScrollArea(std::string name);
	explicit ScrollArea(GenerateName_t = {}, const std::string_view type = "s-area");
	explicit ScrollArea(core_ptr core) noexcept;

public:
	void mode(ScrollAreaMode value) noexcept;
	[[nodiscard]] ScrollAreaMode mode() const noexcept;

	void content(Component&& value) noexcept;
	void content(const Component& value) noexcept;
	[[nodiscard]] Component& content() noexcept;
	[[nodiscard]] const Component& content() const noexcept;

public:
	void area_position(libv::vec2f value) noexcept;
	[[nodiscard]] libv::vec2f area_position() const noexcept;

	[[nodiscard]] libv::vec2f area_size() const noexcept;
};

// =================================================================================================

//enum class BarVisibility {
//	/// The default. The scroll bar is displayed when the client area does not fit in the viewport.
//	on_demand = 0,
//
//	/// The scroll bar appears when the viewport is smaller than the client and disappears when the viewport is larger than the client.
//	on_demand_lazy,
//
//	/// Always display the scroll bar. The knob disappears if the viewport is large enough to show the whole client.
//	always,
//
//	/// Never display the scroll bar. Use this option if you don't want the user to directly control what part of the client is shown, or if you want them to use only non-scroll-bar techniques (such as dragging).
//	never,
//};
//
//enum class ScrollPaneMode {
//	horizontal,
//	vertical,
//
//	both_with_corner,
//	both_with_horizontal_main,
//	both_with_vertical_main,
//};
//
//enum class BarPlacementHorizontal {
//	left,
//	right,
//
//	both,
//};
//
//enum class BarPlacementVertical {
//	top,
//	bottom,
//
//	both,
//};
//
//// -------------------------------------------------------------------------------------------------
//
//class ScrollPane : public ComponentHandler<class CoreScrollPane, EventHostGeneral<ScrollPane>> {
//public:
//	explicit ScrollPane(std::string name);
//	explicit ScrollPane(GenerateName_t = {}, const std::string_view type = "s-pane");
//	explicit ScrollPane(core_ptr core) noexcept;
//
//public:
//	void content(Component&& value) noexcept;
//	void content(const Component& value) noexcept;
//	[[nodiscard]] Component& content() noexcept;
//	[[nodiscard]] const Component& content() const noexcept;
//
//public:
//	[[nodiscard]] ScrollArea& scroll_area();
//	[[nodiscard]] const ScrollArea& scroll_area() const;
//
//	[[nodiscard]] ScrollBar& bar_vertical();
//	[[nodiscard]] const ScrollBar& bar_vertical() const;
//
//	[[nodiscard]] ScrollBar& bar_horizontal();
//	[[nodiscard]] const ScrollBar& bar_horizontal() const;
//
//	void corner(Component value);
//	void clear_corner() noexcept;
//	[[nodiscard]] bool has_corner() const noexcept;
//	[[nodiscard]] const Component& corner() const;
//};

// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
//
//// Property public API
//#define LIBV_UI_PROPERTY_PAPI(RType, WType, Name) \
//	void Name(WType value); \
//	[[nodiscard]] RType Name() const noexcept;
//
//// Property member manual
//#define LIBV_UI_PROPERTY_MMAN(Owner, RType, WType, Name) \
//	void Owner::Name(WType value) { \
//		AccessProperty::manual(self(), self().property.Name, std::move(value)); \
//	} \
//	RType Owner::Name() const noexcept { \
//		return self().property.Name(); \
//	}
//
//// Property public API | value based
//#define LIBV_UI_PROPERTY_PAPI_V(Type, Name) LIBV_UI_PROPERTY_PAPI(Type, Type, Name)
//// Property public API | reference based
//#define LIBV_UI_PROPERTY_PAPI_R(Type, Name) LIBV_UI_PROPERTY_PAPI(const Type&, Type, Name)
//
//// Property member manual | value based
//#define LIBV_UI_PROPERTY_MMAN_V(Owner, Type, Name) LIBV_UI_PROPERTY_MMAN(Owner, Type, Type, Name)
//// Property member manual | reference based
//#define LIBV_UI_PROPERTY_MMAN_R(Owner, Type, Name) LIBV_UI_PROPERTY_MMAN(Owner, const Type&, Type, Name)
//
//
//// =================================================================================================
//// =================================================================================================
//
//class ScrollPane : public ComponentHandler<class CoreScrollPane, EventHostGeneral<ScrollPane>> {
////class ScrollPane :
////	public ScrollArea,
////	public ComponentDerived<ScrollPane>,
////	public EventHost<EventHostGeneral<ScrollPane>>> {
//public:
//	explicit ScrollPane(std::string name);
//	explicit ScrollPane(GenerateName_t = {}, const std::string_view type = "s-pane");
//	explicit ScrollPane(core_ptr core) noexcept;
//
//public:
//	[[nodiscard]] ScrollBar& bar_vertical();
//	[[nodiscard]] const ScrollBar& bar_vertical() const;
//
//	[[nodiscard]] ScrollBar& bar_horizontal();
//	[[nodiscard]] const ScrollBar& bar_horizontal() const;
//
//	void corner(Component value);
//	void clear_corner();
//	[[nodiscard]] const Component& corner() const;
//
////	[[nodiscard]] ScrollArea& scroll_area();
////	[[nodiscard]] const ScrollArea& scroll_area() const;
//
//public:
//	LIBV_UI_PROPERTY_PAPI_V(BarPlacementHorizontal, bar_placement_horizontal)
//	LIBV_UI_PROPERTY_PAPI_V(BarPlacementVertical, bar_placement_vertical)
//	LIBV_UI_PROPERTY_PAPI_V(BarVisibility, bar_visibility_horizontal)
//	LIBV_UI_PROPERTY_PAPI_V(BarVisibility, bar_visibility_vertical)
//	LIBV_UI_PROPERTY_PAPI_V(ScrollPaneMode, mode)
//};
//
//LIBV_UI_PROPERTY_MMAN_V(ScrollPane, BarPlacementHorizontal, bar_placement_horizontal)
//LIBV_UI_PROPERTY_MMAN_V(ScrollPane, BarPlacementVertical, bar_placement_vertical)
//LIBV_UI_PROPERTY_MMAN_V(ScrollPane, BarVisibility, bar_visibility_horizontal)
//LIBV_UI_PROPERTY_MMAN_V(ScrollPane, BarVisibility, bar_visibility_vertical)
//LIBV_UI_PROPERTY_MMAN_V(ScrollPane, ScrollPaneMode, mode)

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
