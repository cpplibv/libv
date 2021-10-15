// Project: libv.ui, File: src/libv/ui/style.cpp


// hpp
#include <libv/ui/style.hpp>
// libv
#include <libv/algo/erase_if_stable.hpp>
// std
#include <ranges>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Style::Style(std::string style_name) :
	style_name(std::move(style_name)) { }

Style::~Style() {
	for (const auto& parent : parents) {
		parent->children.erase(libv::make_observer_ref(this));
	}
}

// -------------------------------------------------------------------------------------------------

void Style::dirty() noexcept {
	if (dirty_)
		return;

	dirty_ = true;
	for (const auto& child : children)
		child->dirty();
}

void Style::clearDirty() noexcept {
	dirty_ = false;
}

// -------------------------------------------------------------------------------------------------

bool Style::detect_cycle(const Style& node, const Style& leaf) {
	for (const auto& parent : node.parents) {
		if (parent->style_name == leaf.style_name)
			return true;
		else
			detect_cycle(*parent, leaf);
	}
	return false;
}

void Style::inherit(const libv::intrusive_ref<Style>& parent) {
	bool cyclic_dependency = detect_cycle(*parent, *this);
	if (cyclic_dependency) {
		log_ui.error("Cyclic dependency found in style {} when attempting to inherit from {}. Requested inheritance is ignored", style_name, parent->style_name);
		return;
	}

	parent->children.emplace(libv::make_observer_ref(this));
	parents.emplace_back(parent);
	dirty();
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
	dirty();
}

libv::optional_ref<const PropertyDynamic> Style::get_optional(StyleState state, const std::string_view property) const {
	// TODO P1: For performance flatten the hierarchy into each node (mark the flatten properties because they might have to be cleared or rebuilt, and maintain it during set/clear/inherit operations)

	const auto it = properties.find(property);
	if (it != properties.end()) {
		for (const PropertyEntry& entry : it->second | std::views::reverse)
			if (state_matches_requirement(state, entry.state_mask, entry.state_value))
				return libv::make_optional_ref(entry.value);

	} else {
		for (const auto& parent : parents)
			if (auto result = parent->get_optional(state, property); result)
				return result;
	}

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
	for (const auto& parent : parents) {
		if (style_name.starts_with(parent->style_name) && style_name.size() > parent->style_name.size() && style_name[parent->style_name.size()] == '.')
			continue;

		parent->children.erase(libv::make_observer_ref(this));
	}

	properties.clear();
	dirty();
}

// -------------------------------------------------------------------------------------------------

void intrusive_ptr_add_ref(Style* style) {
	++style->ref_count;
}

void intrusive_ptr_release(Style* style) {
	if (--style->ref_count == 0)
		delete style;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
