// Project: libv.ui, File: src/libv/ui/zzz_pastebin/component_api_gen.hpp

#pragma once



// Property public API
#define LIBV_UI_PROPERTY_PAPI(RType, WType, Name) \
	void Name(WType value); \
	[[nodiscard]] RType Name() const noexcept;

// Property member manual
#define LIBV_UI_PROPERTY_MMAN(Owner, RType, WType, Name) \
	void Owner::Name(WType value) { \
		AccessProperty::manual(self(), self().property.Name, std::move(value)); \
	} \
	RType Owner::Name() const noexcept { \
		return self().property.Name(); \
	}

// Property public API | value based
#define LIBV_UI_PROPERTY_PAPI_V(Type, Name) LIBV_UI_PROPERTY_PAPI(Type, Type, Name)
// Property public API | reference based
#define LIBV_UI_PROPERTY_PAPI_R(Type, Name) LIBV_UI_PROPERTY_PAPI(const Type&, Type, Name)

// Property member manual | value based
#define LIBV_UI_PROPERTY_MMAN_V(Owner, Type, Name) LIBV_UI_PROPERTY_MMAN(Owner, Type, Type, Name)
// Property member manual | reference based
#define LIBV_UI_PROPERTY_MMAN_R(Owner, Type, Name) LIBV_UI_PROPERTY_MMAN(Owner, const Type&, Type, Name)


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
//  // ...
//
//public:
//	[[nodiscard]] Slider& bar_vertical();
//	[[nodiscard]] const Slider& bar_vertical() const;
//
//	[[nodiscard]] Slider& bar_horizontal();
//	[[nodiscard]] const Slider& bar_horizontal() const;
//
//  // ...
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
