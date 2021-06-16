//// Project: libv.ui, File: src/libv/ui/component/panel_status_log.cpp, Author: Császár Mátyás [Vader]
//
//// hpp
//#include <libv/ui/component/panel_status_log.hpp>
//// libv
//#include <libv/algo/erase_unstable.hpp>
//#include <libv/algo/linear_find.hpp>
////#include <libv/utility/min_max.hpp>
////#include <libv/utility/to_underlying.hpp>
//// pro
//#include <libv/ui/component/panel_line.hpp>
//#include <libv/ui/component/panel_line_core.hpp>
//
////#include <libv/ui/component/base_panel_static.hpp>
////#include <libv/ui/chrono.hpp>
////#include <libv/ui/component/base_panel.lpp>
////#include <libv/ui/context/context_layout.hpp>
//#include <libv/ui/context/context_style.hpp>
//#include <libv/ui/component/detail/core_component.hpp>
////#include <libv/ui/layout/view_layouted.lpp>
//#include <libv/ui/log.hpp>
////#include <libv/ui/property.hpp>
////#include <libv/ui/property_access_context.hpp>
//
//
//namespace libv {
//namespace ui {
//
//// -------------------------------------------------------------------------------------------------
//// =================================================================================================
//
////class StatusLog : libv::ui::PanelLine {
////private:
////	using EntryID = uint64_t;
////
////	struct LogEntry {
////		std::string id;
////		std::string style;
////		libv::ui::Label label;
////		time_point death_time;
//////		time_duration lifetime;
////	};
////
////private:
////	std::vector<LogEntry> entries;
////	std::unordered_map<EntryID, std::shared_ptr<LogEntry>> index;
////
//////	libv::ui::PanelLine panel;
////
////public:
////	void add(EntryID id, std::string style, std::string text, time_duration lifetime);
////	void remove(EntryID id);
////
////private:
////	virtual void update(time_duration dt) override {
////
////	}
////};
//
//// =================================================================================================
//// =================================================================================================
//
////struct CorePanelStatusLog : public CoreBasePanelStatic<CorePanelStatusLog> {
////	struct Properties {
////		PropertyL<AlignHorizontal> align_horizontal;
////		PropertyL<AlignVertical> align_vertical;
////		PropertyL<Orientation> orientation;
////	} property;
////
////	struct LogEntry {
////		PanelStatusLog::EntryID id;
//////		std::string style;
//////		libv::ui::Label label;
////		Component component;
////		time_point death_time; // time_duration lifetime;
////	};
////
////	PanelLine panel;
////	std::vector<LogEntry> entries;
////
////	template <typename T>
////	void access_properties(T& ctx) {
////	//	ctx.property(
////	//			[](auto& c) -> auto& { return c.align_horizontal; },
////	//			AlignHorizontal::left,
////	//			pgr::layout, pnm::align_horizontal,
////	//			"Horizontal align of the inner content of the component"
////	//	);
////	//	ctx.property(
////	//			[](auto& c) -> auto& { return c.align_vertical; },
////	//			AlignVertical::top,
////	//			pgr::layout, pnm::align_vertical,
////	//			"Vertical align of the inner content of the component"
////	//	);
////	//	ctx.property(
////	//			[](auto& c) -> auto& { return c.orientation; },
////	//			Orientation::LEFT_TO_RIGHT,
////	//			pgr::layout, pnm::orientation,
////	//			"Orientation of subsequent components"
////	//	);
////	}
////
////	template <typename T>
////	void access_components(T& ctx) {
////		ctx(panel);
////	}
////};
//
//struct CorePanelStatusLog : public CorePanelLine {
////	struct Properties {
////		PropertyL<AlignHorizontal> align_horizontal;
////		PropertyL<AlignVertical> align_vertical;
////		PropertyL<Orientation> orientation;
////	} property;
//
//	struct LogEntry {
//		PanelStatusLog::EntryID id;
////		std::string style;
////		libv::ui::Label label;
//		Component component;
//		time_point death_time; // time_duration lifetime;
//	};
//
////	PanelLine panel;
//	std::vector<LogEntry> entries;
//
////	template <typename T>
////	void access_properties(T& ctx) {
////	//	ctx.property(
////	//			[](auto& c) -> auto& { return c.align_horizontal; },
////	//			AlignHorizontal::left,
////	//			pgr::layout, pnm::align_horizontal,
////	//			"Horizontal align of the inner content of the component"
////	//	);
////	//	ctx.property(
////	//			[](auto& c) -> auto& { return c.align_vertical; },
////	//			AlignVertical::top,
////	//			pgr::layout, pnm::align_vertical,
////	//			"Vertical align of the inner content of the component"
////	//	);
////	//	ctx.property(
////	//			[](auto& c) -> auto& { return c.orientation; },
////	//			Orientation::LEFT_TO_RIGHT,
////	//			pgr::layout, pnm::orientation,
////	//			"Orientation of subsequent components"
////	//	);
////	}
////
////	template <typename T>
////	void access_components(T& ctx) {
////		ctx(panel);
////	}
//};
//
//// =================================================================================================
//
//core_ptr PanelStatusLog::create_core(std::string name) {
//	return create_core_ptr<CorePanelStatusLog>(std::move(name));
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void PanelStatusLog::align_horizontal(AlignHorizontal value) {
//	self().panel.align_horizontal(value);
//}
//
//AlignHorizontal PanelStatusLog::align_horizontal() const noexcept {
//	return self().panel.align_horizontal();
//}
//
//void PanelStatusLog::align_vertical(AlignVertical value) {
//	self().panel.align_vertical(value);
//}
//
//AlignVertical PanelStatusLog::align_vertical() const noexcept {
//	return self().panel.align_vertical();
//}
//
//void PanelStatusLog::orientation(Orientation value) {
//	self().panel.orientation(value);
//}
//
//Orientation PanelStatusLog::orientation() const noexcept {
//	return self().panel.orientation();
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void PanelStatusLog::add(EntryID id, Component component, time_duration lifetime) {
//	add(id, std::move(component), clock::now() + lifetime);
//}
//
//void PanelStatusLog::add(EntryID id, Component component, time_point death_time) {
//	auto it = libv::linear_find_iterator(self().entries, id, &CorePanelStatusLog::LogEntry::id);
//	if (it != self().entries.end()) {
//		it->component = std::move(component);
//		it->death_time = death_time;
//		0;//if_death_time_passed_kill_it;
//	} else {
//		self().entries.emplace_back(id, component, death_time);
//		self().panel.add(std::move(component));
//	}
//}
//
//void PanelStatusLog::remove(EntryID id) {
//	auto it = libv::linear_find_iterator(self().entries, id, &CorePanelStatusLog::LogEntry::id);
//	if (it != self().entries.end()) {
//		self().panel.remove(it->component);
//		libv::erase_unstable(self().entries, it);
//	}
//}
//
//void PanelStatusLog::clear() {
//	self().panel.clear();
//}
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace ui
//} // namespace libv
