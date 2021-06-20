// Project: libv.ui, File: src/libv/ui/component/panel_status_log.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/panel_status_log.hpp>
// libv
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/algo/erase_unstable.hpp>
#include <libv/algo/linear_find.hpp>
// pro
#include <libv/ui/component/panel_line_core.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>
//#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CorePanelStatusLog : public CorePanelLine {
	friend class PanelStatusLog;
	[[nodiscard]] inline auto handler() { return PanelStatusLog{this}; }

private:
	struct LogEntry {
		PanelStatusLog::EntryID id;
//		std::string style;
		Component component;
		time_point time_of_death;
	};

	std::vector<LogEntry> entries;

public:
	using CorePanelLine::CorePanelLine;

private:
	virtual void doRender(Renderer& r) override {
		const auto time_of_frame = context().state.time_frame();

		libv::erase_if_unstable(entries, [&](LogEntry& entry) {
			if (entry.time_of_death < time_of_frame) {
				entry.component.markRemove();
				return true;
			}
			return false;
		});

		CorePanelLine::doRender(r);
	}
};

// =================================================================================================

core_ptr PanelStatusLog::create_core(std::string name) {
	return create_core_ptr<CorePanelStatusLog>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

void PanelStatusLog::add(EntryID id, Component component, time_duration lifetime) {
	add(id, std::move(component), self().context().state.time_frame() + lifetime);
}

void PanelStatusLog::add(EntryID id, Component component, time_point time_of_death) {
	const auto time_of_frame = self().context().state.time_frame();
	const auto already_dead = time_of_death < time_of_frame;

	const auto it = libv::linear_find_iterator(self().entries, id, &CorePanelStatusLog::LogEntry::id);
	if (it != self().entries.end()) {
		if (already_dead) {
			Base::remove(it->component);
			libv::erase_unstable(self().entries, it);
		} else {
			if (it->component != component) {
				Base::remove(it->component);
				it->component = component;
				Base::add(std::move(component));
			}

			it->time_of_death = time_of_death;
		}

	} else if (!already_dead) {
		self().entries.emplace_back(id, component, time_of_death);
		Base::add(std::move(component));
	}
}

void PanelStatusLog::remove(EntryID id) {
	auto it = libv::linear_find_iterator(self().entries, id, &CorePanelStatusLog::LogEntry::id);
	if (it != self().entries.end()) {
		libv::erase_unstable(self().entries, it);
		Base::remove(it->component);
	}
}

void PanelStatusLog::clear() {
	self().entries.clear();
	Base::clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
