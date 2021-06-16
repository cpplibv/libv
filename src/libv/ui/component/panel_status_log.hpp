//// Project: libv.ui, File: src/libv/ui/component/panel_status_log.hpp, Author: Császár Mátyás [Vader]
//
//#pragma once
//
//// pro
//#include <libv/ui/chrono.hpp>
//#include <libv/ui/component/detail/component_api.hpp>
//#include <libv/ui/property/align.hpp>
//#include <libv/ui/property/orientation.hpp>
//
//
//namespace libv {
//namespace ui {
//
//// -------------------------------------------------------------------------------------------------
//
//class PanelStatusLog : public ComponentAPI<Component, PanelStatusLog, class CorePanelStatusLog, EventHostGeneral> {
//public:
//	using ComponentAPI::ComponentAPI;
//	static constexpr std::string_view component_type = "status";
//	static core_ptr create_core(std::string name);
//
//public:
//	void align_horizontal(AlignHorizontal value);
//	[[nodiscard]] AlignHorizontal align_horizontal() const noexcept;
//
//	void align_vertical(AlignVertical value);
//	[[nodiscard]] AlignVertical align_vertical() const noexcept;
//
//	void orientation(Orientation value);
//	[[nodiscard]] Orientation orientation() const noexcept;
//
//public:
//	using EntryID = uint64_t;
//
//	void add(EntryID id, Component component, time_duration lifetime = {});
//	void add(EntryID id, Component component, time_point death_time);
//	void remove(EntryID id);
//	void clear();
//};
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace ui
//} // namespace libv
