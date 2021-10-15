// Project: libv.ui, File: src/libv/ui/component/detail/component.cpp

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
	ptr_(std::move(ptr_)) {
	assert(ptr_ != nullptr && "Internal error: Component cannot be null");
	++ptr_->ref_count;
}

Component::Component(const Component& other) noexcept {
	ptr_ = other.ptr_;
	++ptr_->ref_count;
}

Component::Component(Component&& other) noexcept {
	ptr_ = other.ptr_;
	other.ptr_ = nullptr;
}

Component& Component::operator=(const Component& other) & noexcept {
	ptr_ = other.ptr_;
	++ptr_->ref_count;
	return *this;
}

Component& Component::operator=(Component&& other) & noexcept {
	ptr_ = other.ptr_;
	other.ptr_ = nullptr;
	return *this;
}

Component::~Component() noexcept {
	if (ptr_ && --ptr_->ref_count == 0)
		delete ptr_;
}

bool Component::isFloatRegion() const noexcept {
	return ptr_->isFloatRegion();
}

const std::string& Component::name() const noexcept {
	return ptr_->name;
}

std::string Component::path() const noexcept {
	return ptr_->path();
}

libv::vec3f Component::layout_position() const noexcept {
	return ptr_->layout_position();
}

libv::vec2f Component::layout_position2() const noexcept {
	return ptr_->layout_position2();
}

libv::vec3f Component::layout_size() const noexcept {
	return ptr_->layout_size();
}

libv::vec2f Component::layout_size2() const noexcept {
	return ptr_->layout_size2();
}

// -------------------------------------------------------------------------------------------------

void Component::size(const Size& value) noexcept {
	ptr_->size(value);
}

const Size& Component::size() const noexcept {
	return ptr_->size();
}

void Component::anchor(Anchor value) noexcept {
	ptr_->anchor(value);
}

Anchor Component::anchor() const noexcept {
	return ptr_->anchor();
}

void Component::margin(Margin value) noexcept {
	ptr_->margin(value);
}

Margin Component::margin() const noexcept {
	return ptr_->margin();
}

void Component::padding(Padding value) noexcept {
	ptr_->padding(value);
}

Padding Component::padding() const noexcept {
	return ptr_->padding();
}

// -------------------------------------------------------------------------------------------------

libv::vec2f Component::calculate_local_mouse_coord() const noexcept {
	return ptr_->calculate_local_mouse_coord();
}

// -------------------------------------------------------------------------------------------------

void Component::markRemove() {
	ptr_->markRemove();
}

//void Component::style(libv::intrusive_ptr<Style> style) noexcept {
//	ptr_->style(std::move(style));
//}

void Component::style(std::string_view style_name) {
	ptr_->style(style_name);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
