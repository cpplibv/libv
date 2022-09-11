// Project: libv.ui, File: src/libv/ui/component/_idea_radio_button.hpp

#pragma once

// libv
#include <libv/algo/linear_find.hpp>
// std
#include <memory>
#include <optional>
#include <vector>
// pro
#include <libv/ui/component/toggle_button.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// TODO P4: Selection Group advanced features:
//			variants
//				unique
//				optional
//				multiple (min-max)
//			over select policy
//				replace last
//				replace first
//				disallow
//				allow as error
//			under select policy
//				reselect last
//				disallow
//				allow as error

template <typename T>
class SelectionGroup;

// -------------------------------------------------------------------------------------------------

template <typename T>
struct ImplSelectionGroup : std::enable_shared_from_this<ImplSelectionGroup<T>> {
//struct ImplSelectionGroup {
//	int numMinSelected = 1; // 0 means allowEmpty
//	int numMaxSelected = 1;

	struct Entry {
		ToggleButton button;
		T value;
	};
	std::vector<Entry> entries;
	static constexpr int32_t invalid_index = -1;
	int32_t selection_index = invalid_index;

	~ImplSelectionGroup() {
		get_nexus(0).disconnect_all(this);
	}
};

// -------------------------------------------------------------------------------------------------

/// Handler type
template <typename T>
class SelectionGroup {

private:
	using Self = ImplSelectionGroup<T>;
	std::shared_ptr<Self> self = std::make_shared<Self>();

private:
	explicit SelectionGroup(const std::shared_ptr<ImplSelectionGroup<T>>& self) : self(self) {}

public:
	SelectionGroup() = default;

public: // Core / Event Hub support
	using CoreT = ImplSelectionGroup<T>;

	[[nodiscard]] static inline SelectionGroup<T> from_core(CoreT* ptr) noexcept {
		return SelectionGroup{ptr->shared_from_this()};
	}

	[[nodiscard]] constexpr inline CoreT* ptr() const noexcept {
		return self.get();
	}

	[[nodiscard]] inline EventHostSelectionGroup<SelectionGroup<T>> event() noexcept {
		return EventHostSelectionGroup<SelectionGroup<T>>{*this};
	}

public:
	void add(ToggleButton button, T value_, bool select = false) {
		{
			const auto old_index = self->selection_index;
			const auto new_index = static_cast<int32_t>(self->entries.size());

			self->entries.emplace_back(button, std::move(value_));

			if (select) {
				if (old_index != Self::invalid_index)
					self->entries[old_index].button.select(false);

				self->entries[new_index].button.select(true);
				self->selection_index = new_index;
			} else if (button.select()) {
				self->selection_index = new_index;
			}
		}

		// NOTE: SelectionGroup has no reentry issue because ToggleButton's select is different from submit
		// NOTE: BY VALUE COPY of *this as a handler object
		button.event().submit.connect_system([*this](const ToggleButton& rb, const EventSubmit& event) mutable {
			const auto old_index = self->selection_index;
			const auto new_index = libv::linear_find_index<int32_t>(self->entries, rb, &Self::Entry::button);

			if (old_index == new_index) {
				event.reject_submit(); // Cannot un-toggle currently active button
				return;
			}

			if (old_index != Self::invalid_index)
				self->entries[old_index].button.select(false);

			// NOTE: No need to select this button as toggle buttons default toggle behaviour will take care of it
			//if (new_index != Self::invalid_index)
			//	self->entries[new_index].button.select(true);

			self->selection_index = new_index;
			this->event().change.fire();
		});

	}

public:
	[[nodiscard]] inline bool has_value() const noexcept {
		return self->selection_index != Self::invalid_index;
	}

	[[nodiscard]] const T& value() const noexcept {
		assert(has_value());
		return self->entries[self->selection_index].value;
	}

	[[nodiscard]] inline bool has_selection() const noexcept {
		return has_value();
	}

	void select(const T& value) {
		const auto old_index = self->selection_index;
		const auto new_index = libv::linear_find_index<int32_t>(self->entries, value, &Self::Entry::value);

		if (old_index == new_index)
			return;

		if (old_index != Self::invalid_index)
			self->entries[old_index].button.select(false);

		if (new_index != Self::invalid_index)
			self->entries[new_index].button.select(true);

		self->selection_index = new_index;
		event().change.fire();
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
using SelectionGroupUnique = SelectionGroup<T>;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
