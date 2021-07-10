// Project: libv.ui, File: src/libv/ui/component/panel_status_line.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/panel_status_line.hpp>
// libv
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/algo/erase_unstable.hpp>
#include <libv/algo/linear_find.hpp>
// pro
#include <libv/ui/component/panel_line_core.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CorePanelStatusLine : public CorePanelLine {
	friend class PanelStatusLine;
	[[nodiscard]] inline auto handler() { return PanelStatusLine{this}; }

private:
	struct LogEntry {
		PanelStatusLine::EntryID id;
//		std::string style;
		Component component;
		time_point time_of_death;
	};

	std::vector<LogEntry> entries;

public:
	using CorePanelLine::CorePanelLine;

protected:
	virtual void doUpdate() override {
		const auto time_of_frame = context().state.time_frame();

		libv::erase_if_unstable(entries, [&](LogEntry& entry) {
			if (entry.time_of_death != time_point{} && entry.time_of_death < time_of_frame) {
				entry.component.markRemove();
				fire(EventPanelStatusExpired{{}, entry.id});
				return true;
			}
			return false;
		});

		CorePanelLine::doUpdate();
	}

	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override {
		// This pattern is not my favorite, for now it will do
		//	I think if the remove were instant (and detach would happen later with a global pit)
		//	this could have a chance of improving, but have no concrete idea so far
		CorePanelLine::doDetachChildren([this, &callback](auto& child) {
			const auto remove = callback(child);
			if (remove)
				libv::erase_unstable(entries, child, &LogEntry::component);
			return remove;
		});
	}
};

// =================================================================================================

core_ptr PanelStatusLine::create_core(std::string name) {
	return create_core_ptr<CorePanelStatusLine>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

void PanelStatusLine::add(EntryID id, Component component, time_duration lifetime) {
	add(id, std::move(component), self().context().state.time_frame() + lifetime);
}

void PanelStatusLine::add(EntryID id, Component component, time_point time_of_death) {
	const auto time_of_frame = self().context().state.time_frame();
	const auto already_dead = time_of_death == time_point{} ? false : time_of_death < time_of_frame;

	const auto it = libv::linear_find_iterator(self().entries, id, &CorePanelStatusLine::LogEntry::id);
	if (it != self().entries.end()) {
		if (already_dead) {
			auto comp = std::move(it->component);
			libv::erase_unstable(self().entries, it);
			Base::remove(comp);
//			Base::remove(it->component);
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

void PanelStatusLine::remove(EntryID id) {
	auto it = libv::linear_find_iterator(self().entries, id, &CorePanelStatusLine::LogEntry::id);
	if (it != self().entries.end()) {
		auto comp = std::move(it->component);
		libv::erase_unstable(self().entries, it);
		Base::remove(comp);
//		Base::remove(it->component);
	}
}

void PanelStatusLine::clear() {
	self().entries.clear();
	Base::clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
