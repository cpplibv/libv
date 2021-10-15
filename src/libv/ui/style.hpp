// Project: libv.ui, File: src/libv/ui/style.hpp

#pragma once

// fwd
#include <libv/ui/style_fwd.hpp>
// ext
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
// libv
#include <libv/utility/concat.hpp>
#include <libv/utility/intrusive_ref.hpp>
#include <libv/utility/observer_ref.hpp>
#include <libv/utility/optional_ref.hpp>
// std
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
// pro
#include <libv/ui/property.hpp>
#include <libv/ui/style_state.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Style {
	struct PropertyEntry {
		StyleState state_mask = StyleState::none;
		StyleState state_value = StyleState::none;
		PropertyDynamic value;
	};

private:
	int32_t ref_count = 0;
	friend void intrusive_ptr_add_ref(Style*);
	friend void intrusive_ptr_release(Style*);

private:
	bool dirty_ = false;
	std::vector<libv::intrusive_ref<Style>> parents;
	boost::container::flat_set<libv::observer_ref<Style>> children;
	boost::container::flat_map<std::string, std::vector<PropertyEntry>, std::less<>> properties;

public:
	const std::string style_name;

public:
	explicit Style(std::string style_name);
	~Style();

public:
	void dirty() noexcept;
	void clearDirty() noexcept;
	[[nodiscard]] constexpr inline bool isDirty() const noexcept {
		return dirty_;
	}

private:
	static bool detect_cycle(const Style& node, const Style& leaf);

public:
	// TODO P5: detect cyclic dependency attempts and swap to use an error handling version of inherit (remove old)
	//	void inherit_or_throw(const libv::intrusive_ref<Style>& parent);
	//	bool try_inherit(const libv::intrusive_ref<Style>& parent);
	//	bool inherit_optional(const libv::intrusive_ref<Style>& parent);
	void inherit(const libv::intrusive_ref<Style>& parent);

public:
	void set(StyleState state_mask, StyleState state_value, const std::string& property, PropertyDynamic value);

	template <typename T>
	inline void set(StyleState state, std::string property, T&& value) {
		set(state, std::move(property), PropertyDynamic{std::forward<T>(value)});
	}

public:
	[[nodiscard]] libv::optional_ref<const PropertyDynamic> get_optional(StyleState state, const std::string_view property) const;
	template <typename T>
	[[nodiscard]] auto get_optional(StyleState state, const std::string_view property) const {
		const auto& result = get_optional(state, property);
		return result && std::holds_alternative<T>(*result) ? &std::get<T>(*result) : nullptr;
	}

	[[nodiscard]] const PropertyDynamic& get_or_throw(StyleState state, const std::string_view property) const;
	template <typename T>
	[[nodiscard]] const T& get_or_throw(StyleState state, const std::string_view property) const {
		const auto& result = get_or_throw(state, property);
		if (not std::holds_alternative<T>(result))
			throw std::invalid_argument(libv::concat("Requested property \"", property, "\" has different type"));

		return std::get<T>(result);
	}

public:
	void clear();

public:
	template <typename F>
	inline void foreach(const F& func) {
		// TODO P4: foreach should also iterate over inherited properties, or at least has an API for it
		for (const auto& [key, entires] : properties)
			for (const auto& entry : entires)
				func(key, entry.value);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
