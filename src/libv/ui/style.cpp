// File:   style.cpp Author: Vader Created on 25 July 2019, 00:27


// hpp
#include <libv/ui/style.hpp>
// pro
#include <libv/ui/base_component.hpp>
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

void Style::set(std::string property, PropertyDynamic value) {
	properties.insert_or_assign(std::move(property), std::move(value));
	dirty();
}

libv::observer_ptr<const PropertyDynamic> Style::get_optional(const std::string_view property) const {
	libv::observer_ptr<const PropertyDynamic> result;

	const auto it = properties.find(property);
	if (it != properties.end()) {
		result = libv::make_observer(it->second);
	} else {
		for (const auto& parent : parents) {
			result = parent->get_optional(property);
			if (result)
				break;
		}
	}

	return result;
}

const PropertyDynamic& Style::get_or_throw(const std::string_view property) const {
	const auto result = get_optional(property);
	if (!result)
		throw std::invalid_argument(libv::concat("Requested property \"", property, "\" is not found"));

	return *result;
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
