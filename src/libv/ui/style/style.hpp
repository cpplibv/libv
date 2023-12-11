// Project: libv.ui, File: src/libv/ui/style/style.hpp

#pragma once

// fwd
#include <libv/ui/style/style_fwd.hpp>
// ext
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
// libv
#include <libv/utility/concat.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
#include <libv/utility/memory/observer_ref.hpp>
#include <libv/utility/memory/optional_ref.hpp>
// std
#include <stdexcept>
#include <string>
#include <string_view>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/property_system/property.hpp>
#include <libv/ui/style/style_state.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Style : public libv::ref_count_base {
	friend libv::ref_count_access;

private:
	struct PropertyEntry {
		StyleState state_mask = StyleState::none;
		StyleState state_value = StyleState::none;
		PropertyDynamic value;
	};

private:
	bool dirty_ = false; // Fits into padding

	boost::container::flat_map<std::string, std::vector<PropertyEntry>, std::less<>> properties;
	std::vector<libv::intrusive2_ref<Style>> bases;
	boost::container::flat_set<libv::observer_ref<Style>> children;

public:
	const std::string style_name;

public:
	explicit Style(std::string style_name);
	~Style();

public:
	void markAsDirty() noexcept;
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
	void inherit(const libv::intrusive2_ref<Style>& parent);

public:
	void set(StyleState state_mask, StyleState state_value, const std::string& property, PropertyDynamic value);

	template <typename T>
	inline void set(StyleState state, std::string property, T&& value) {
		set(state, std::move(property), PropertyDynamic{std::forward<T>(value)});
	}

public:
	[[nodiscard]] libv::optional_ref<const PropertyDynamic> get_optional(StyleState state, const std::string_view property) const;
	template <typename T>
	[[nodiscard]] const T* get_optional(StyleState state, const std::string_view property) const {
		const auto* result = get_optional(state, property);

		if (result == nullptr)
			return nullptr;

		const auto* value = std::any_cast<T>(result);
		log_ui.warn_if(value == nullptr, "Property type missmatch: {}", property);
		return value;
	}

	[[nodiscard]] const PropertyDynamic& get_or_throw(StyleState state, const std::string_view property) const;
	template <typename T>
	[[nodiscard]] const T& get_or_throw(StyleState state, const std::string_view property) const {
		const auto& result = get_or_throw(state, property);
		const auto* value = std::any_cast<T>(&result);

		if (value == nullptr) {
			log_ui.warn("Property type missmatch: {}", property);
			throw std::invalid_argument(libv::concat("Requested property \"", property, "\" has different type"));
		}

		return *value;
	}

public:
	void clear();

public:
	template <typename F>
	inline void foreach_bases(const F& func) const {
		for (const auto& base : bases)
			func(*base);
	}
	template <typename F>
	inline void foreach_bases_recursive(const F& func) const {
		for (const auto& base : bases) {
			base->foreach_bases_recursive(func);
			func(*base);
		}
	}
	template <typename F>
	inline void foreach(const F& func) {
		// TODO P4: foreach should also iterate over inherited properties, or at least has an API for it
		for (const auto& [key, entires] : properties)
			for (const auto& entry : entires)
				func(key, entry.value, entry.state_mask, entry.state_value);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
