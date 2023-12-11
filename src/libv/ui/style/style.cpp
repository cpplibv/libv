// Project: libv.ui, File: src/libv/ui/style/style.cpp

// hpp
#include <libv/ui/style/style.hpp>
// libv
#include <libv/algo/erase_if_stable.hpp>
// std
#include <ranges>
// pro
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Style::Style(std::string style_name) :
	style_name(std::move(style_name)) { }

Style::~Style() {
	for (const auto& base : bases)
		base->children.erase(libv::make_observer_ref(this));
}

// -------------------------------------------------------------------------------------------------

void Style::markAsDirty() noexcept {
	if (dirty_)
		return;

	dirty_ = true;
	for (const auto& child : children)
		child->markAsDirty();
}

void Style::clearDirty() noexcept {
	dirty_ = false;
}

// -------------------------------------------------------------------------------------------------

bool Style::detect_cycle(const Style& node, const Style& leaf) {
	for (const auto& base : node.bases) {
		if (base->style_name == leaf.style_name)
			return true;
		else
			detect_cycle(*base, leaf);
	}
	return false;
}

void Style::inherit(const libv::intrusive2_ref<Style>& base) {
	bool cyclic_dependency = detect_cycle(*base, *this);
	if (cyclic_dependency) {
		log_ui.error("Cyclic dependency found in style {} when attempting to inherit from {}. Requested inheritance is ignored", style_name, base->style_name);
		return;
	}

	base->children.emplace(libv::make_observer_ref(this));
	bases.emplace_back(base);
	markAsDirty();
}

// -------------------------------------------------------------------------------------------------

void Style::set(StyleState state_mask, StyleState state_value, const std::string& property, PropertyDynamic value) {
	auto& stated_values = properties[property]; // (cannot move in property string, log uses it)

	// Discard and warn on always shadowed stated values
	libv::erase_if_stable(stated_values, [&](const auto& entry) {
		const auto discard = always_shadows(state_mask, state_value, entry.state_mask, entry.state_value);
		log_ui.warn_if(discard, "Discarding old stated value entry with state {}/{} in style {} for property {} as new entry with {}/{} will always shadow the old", +entry.state_mask, +entry.state_value, style_name, property, +state_mask, +state_value);
		return discard;
	});

	stated_values.emplace_back(state_mask, state_value, std::move(value));
	markAsDirty();
}

libv::optional_ref<const PropertyDynamic> Style::get_optional(StyleState state, const std::string_view property) const {
	// TODO P5: For performance flatten the hierarchy into each node (mark the flatten properties because they might have to be cleared or rebuilt, and maintain it during set/clear/inherit operations)

	const auto it = properties.find(property);
	if (it != properties.end()) {
		for (const PropertyEntry& entry : it->second | std::views::reverse)
			if (state_matches_requirement(state, entry.state_mask, entry.state_value))
				return libv::make_optional_ref(entry.value);
	}

	for (const auto& base : bases)
		if (auto result = base->get_optional(state, property); result)
			return result;

	return libv::opt_ref_none;
}

const PropertyDynamic& Style::get_or_throw(StyleState state, const std::string_view property) const {
	const auto result = get_optional(state, property);
	if (!result)
		throw std::invalid_argument(libv::concat("Requested property \"", property, "\" is not found in \"", style_name ,"\" style"));

	return *result;
}

// -------------------------------------------------------------------------------------------------

void Style::clear() {
	// Clear inheritances (that are not dot based)
	// NOTE: Does not modify children to clear their inheritances to this style
	for (const auto& base : bases) {
		if (style_name.starts_with(base->style_name) && style_name.size() > base->style_name.size() && style_name[base->style_name.size()] == '.')
			continue;

		base->children.erase(libv::make_observer_ref(this));
	}

	properties.clear();
	markAsDirty();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
