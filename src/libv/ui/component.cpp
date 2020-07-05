// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

// hpp
#include <libv/ui/component.hpp>
// pro
#include <libv/ui/core_component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//template <typename T, typename... Args>
//core_ptr create_core_ptr(Args&&... args) noexcept {
//	static_assert(std::is_base_of_v<CoreComponent, T>, "");
//
//	return new T(std::forward<Args>(args)...);
//}
//
//void add_ref_core_ptr(core_ptr ptr) noexcept {
//	++ptr->ref_count;
//}
//
//bool remove_ref_core_ptr(core_ptr ptr) noexcept {
//	return --ptr->ref_count == 0;
//}
//
//void destroy_core_ptr(core_ptr ptr) noexcept {
//	delete ptr;
//}

// -------------------------------------------------------------------------------------------------

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

Component::Component(core_ptr ptr_) noexcept :
	ptr(std::move(ptr_)) {
	++ptr->ref_count;
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

const Size& Component::size() const noexcept {
	return ptr->size();
}

void Component::size(const Size& value) noexcept {
	ptr->size(value);
}

Anchor Component::anchor() const noexcept {
	return ptr->anchor();
}

void Component::anchor(Anchor value) noexcept {
	ptr->anchor(value);
}

// -------------------------------------------------------------------------------------------------

void Component::markRemove() {
	ptr->markRemove();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
