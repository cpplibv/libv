// Project: libv.ui, File: src/libv/ui/component/detail/component.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/detail/component.hpp>
// libv
#include <libv/utility/concat.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::string generate_component_name(const std::string_view type, size_t index) noexcept {
	return libv::concat(type, '-', index);
}

Component::Component(core_ptr ptr_) noexcept :
	ptr(std::move(ptr_)) {
	assert(ptr != nullptr && "Internal error: Component cannot be null");
	++ptr->ref_count;
}

Component::Component(const Component& other) noexcept {
	ptr = other.ptr;
	++ptr->ref_count;
}

Component::Component(Component&& other) noexcept {
	ptr = other.ptr;
	other.ptr = nullptr;
}

Component& Component::operator=(const Component& other) & noexcept {
	ptr = other.ptr;
	++ptr->ref_count;
	return *this;
}

Component& Component::operator=(Component&& other) & noexcept {
	ptr = other.ptr;
	other.ptr = nullptr;
	return *this;
}

Component::~Component() noexcept {
	if (ptr && --ptr->ref_count == 0)
		delete ptr;
}

bool Component::isFloatRegion() const noexcept {
	return ptr->isFloatRegion();
}

const std::string& Component::name() const noexcept {
	return ptr->name;
}

std::string Component::path() const noexcept {
	return ptr->path();
}

libv::vec3f Component::layout_position() const noexcept {
	return ptr->layout_position();
}

libv::vec2f Component::layout_position2() const noexcept {
	return ptr->layout_position2();
}

libv::vec3f Component::layout_size() const noexcept {
	return ptr->layout_size();
}

libv::vec2f Component::layout_size2() const noexcept {
	return ptr->layout_size2();
}

// -------------------------------------------------------------------------------------------------

void Component::size(const Size& value) noexcept {
	ptr->size(value);
}

const Size& Component::size() const noexcept {
	return ptr->size();
}

void Component::anchor(Anchor value) noexcept {
	ptr->anchor(value);
}

Anchor Component::anchor() const noexcept {
	return ptr->anchor();
}

void Component::margin(Margin value) noexcept {
	ptr->margin(value);
}

Margin Component::margin() const noexcept {
	return ptr->margin();
}

void Component::padding(Padding value) noexcept {
	ptr->padding(value);
}

Padding Component::padding() const noexcept {
	return ptr->padding();
}

// -------------------------------------------------------------------------------------------------

libv::vec2f Component::calculate_local_mouse_coord() const noexcept {
	return ptr->calculate_local_mouse_coord();
}

// -------------------------------------------------------------------------------------------------

void Component::markRemove() {
	ptr->markRemove();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
