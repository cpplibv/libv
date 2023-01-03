// Project: libv.ui, File: src/libv/ui/component/component.cpp

// hpp
#include <libv/ui/component/component.hpp>
// libv
#include <libv/utility/concat.hpp>
// pro
#include <libv/ui/component/component_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::string generate_component_name(const std::string_view type, std::size_t index) noexcept {
	return libv::concat(type, '-', index);
}

// =================================================================================================

Component::Component(core_ptr ptr_) noexcept :
	ptr_(std::move(ptr_)) {
	assert(ptr_ != nullptr && "Internal error: Component core cannot be null on direct construction");
	++ptr_->ref_count;
}

Component::Component(const Component& other) noexcept :
	ptr_(other.ptr_) {
	if (ptr_)
		++ptr_->ref_count;
}

Component::Component(Component&& other) noexcept :
	ptr_(other.ptr_) {
	other.ptr_ = nullptr;
}

Component& Component::operator=(const Component& other) & noexcept {
	if (ptr_ == other.ptr_)
		return *this;

	if (ptr_ && --ptr_->ref_count == 0)
		delete ptr_;

	ptr_ = other.ptr_;

	if (ptr_)
		++ptr_->ref_count;

	return *this;
}

Component& Component::operator=(Component&& other) & noexcept {
	// NOTE: using this == &other instead of ptr == other.ptr to have all moved from state the same (and be reset)
	if (this == &other)
		return *this;

	if (ptr_ && --ptr_->ref_count == 0)
		delete ptr_;

	ptr_ = other.ptr_;
	other.ptr_ = nullptr;
	return *this;
}

Component::~Component() noexcept {
	if (ptr_ && --ptr_->ref_count == 0)
		delete ptr_;
}

// -------------------------------------------------------------------------------------------------

ContextUI& Component::ui() const noexcept {
	return ptr_->ui();
}

// -------------------------------------------------------------------------------------------------

ReentryGuard Component::event_reentry_guard(const void* source, const void* target) const noexcept {
	return ReentryGuard{ptr_->ui(), source, target};
}

// -------------------------------------------------------------------------------------------------

Component Component::parent() const noexcept {
	const auto s = libv::make_observer_ref(ptr_);
	const auto p = ptr_->parent();
	return s == p ? Component{} : Component{p};
}

// -------------------------------------------------------------------------------------------------

Flag_t Component::flags() const noexcept {
	return ptr_->flags;
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

void Component::z_index_offset(ZIndexOffset value) noexcept {
	ptr_->z_index_offset(value);
}

ZIndexOffset Component::z_index_offset() const noexcept {
	return ptr_->z_index_offset();
}

void Component::margin_extent(MarginExtent value) noexcept {
	ptr_->margin_extent(value);
}

MarginExtent Component::margin_extent() const noexcept {
	return ptr_->margin_extent();
}

void Component::margin(Margin value) noexcept {
	ptr_->margin(value);
}

Margin Component::margin() const noexcept {
	return ptr_->margin();
}

void Component::padding_extent(PaddingExtent value) noexcept {
	ptr_->padding_extent(value);
}

PaddingExtent Component::padding_extent() const noexcept {
	return ptr_->padding_extent();
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

//void Component::style(libv::intrusive2_ptr<Style> style) noexcept {
//	ptr_->style(std::move(style));
//}

void Component::style(std::string_view style_name) {
	ptr_->style(style_name);
}

void Component::style_state(StyleState state, bool value) noexcept {
	ptr_->style_state(state, value);
}

// -------------------------------------------------------------------------------------------------

void Component::focus() {
	ptr_->focus();
}

void Component::enable(bool value) {
	ptr_->enable(value);
}
bool Component::enable() const {
	return ptr_->enable();
}
void Component::show(bool value) {
	ptr_->show(value);
}
bool Component::show() const {
	return ptr_->show();
}

// -------------------------------------------------------------------------------------------------

void Component::foreach_children(libv::function_ref<void(Component&)> callback) {
	ptr_->doForeachChildren(callback);
}

void Component::foreach_recursive_children(libv::function_ref<void(Component&)> callback) {
	ptr_->doForeachChildren([&callback](Component& child) {
		callback(child);
		child.foreach_recursive_children(callback);
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
