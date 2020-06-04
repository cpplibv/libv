// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

// hpp
#include <libv/ui/component.hpp>
// pro
#include <libv/ui/base_component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//template <typename T, typename... Args>
//base_ptr create_base_ptr(Args&&... args) noexcept {
//	static_assert(std::is_base_of_v<BaseComponent, T>, "");
//
//	return new T(std::forward<Args>(args)...);
//}
//
//void add_ref_base_ptr(base_ptr ptr) noexcept {
//	++ptr->ref_count;
//}
//
//bool remove_ref_base_ptr(base_ptr ptr) noexcept {
//	return --ptr->ref_count == 0;
//}
//
//void destroy_base_ptr(base_ptr ptr) noexcept {
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

Component::Component(base_ptr ptr_) noexcept :
	ptr(std::move(ptr_)) {
	++ptr->ref_count;
}


std::string Component::path() const noexcept {
	return ptr->path();
}

std::string Component::name() const noexcept {
	return ptr->name;
}

libv::vec3f Component::position() const noexcept {
	return ptr->position();
}

libv::vec2f Component::position2() const noexcept {
	return ptr->position2();
}

libv::vec3f Component::size() const noexcept {
	return ptr->size();
}

libv::vec2f Component::size2() const noexcept {
	return ptr->size2();
}

void Component::markRemove() {
	ptr->markRemove();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
