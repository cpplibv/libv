// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <libv/ui/component_base.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <cassert>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

ComponentBase::ComponentBase(std::string name) :
	name(std::move(name)) { }

ComponentBase::ComponentBase(UnnamedTag, const std::string_view type) :
	name(libv::concat(type, '-', nextID++)) { }

ComponentBase::~ComponentBase() {
	log_ui.error_if(isAttached(), "Component is destructed in an attached state: {}", path());
}

// -------------------------------------------------------------------------------------------------

std::string ComponentBase::path() const {
	std::string result = name;

	for (auto it = make_observer_ref(this); it != it->parent; it = it->parent)
		result = it->parent->name + '/' + std::move(result);

	return result;
}

ContextUI& ComponentBase::context() const noexcept {
	if (context_ == nullptr) {
		log_ui.fatal("Component has to be attached to acquire its context: {}", path());
		assert(false && "Component has to be attached to acquire its context");
	}
	return *context_;
}

// -------------------------------------------------------------------------------------------------

void ComponentBase::flagSelf(Flag_t flags_) noexcept {
	flags.set(flags_);
}

void ComponentBase::flagParents(Flag_t flags_) noexcept {
	for (auto it = parent; !it->flags.match_mask(flags_); it = it->parent)
		it->flags.set(flags_);
}

void ComponentBase::flagAuto(Flag_t flags_) noexcept {
	flagSelf(flags_ & Flag::mask_self);
	flagParents(flags_ & Flag::mask_propagate);
}

void ComponentBase::flagForce(Flag_t flags_) noexcept {
	flagSelf(flags_);
	flagParents(flags_);
}

// -------------------------------------------------------------------------------------------------

void ComponentBase::style(libv::intrusive_ptr<Style> newStyle) noexcept {
	style_ = std::move(newStyle);
	flagAuto(Flag::pendingStyle);
}

void ComponentBase::markRemove() noexcept {
	flagAuto(Flag::pendingDetach | Flag::pendingLayout);
	flags.reset(Flag::layout | Flag::render);

	foreachChildren([](ComponentBase& child) {
		child.markRemove();
	});
}

// -------------------------------------------------------------------------------------------------

void ComponentBase::attach(ComponentBase& parent_) {
	if (flags.match_any(Flag::pendingAttachSelf)) {
		context_ = libv::make_observer(parent_.context());
		parent = libv::make_observer_ref(parent_);

		log_ui.trace("Attaching {}", path());

		flagParents(flags & Flag::mask_propagate); // Trigger flag propagation

		doAttach();
		flags.reset(Flag::pendingAttachSelf);
	}

	if (flags.match_any(Flag::pendingAttachChild)) {
		foreachChildren([this](ComponentBase& child) {
			if (child.flags.match_any(Flag::pendingAttach))
				child.attach(*this);
		});
		flags.reset(Flag::pendingAttachChild);
	}
}

void ComponentBase::detach(ComponentBase& parent_) {
	if (flags.match_any(Flag::pendingDetachChild)) {

		doDetachChildren([this, &parent_](ComponentBase& child) {
			bool remove = child.flags.match_any(Flag::pendingDetachSelf);

			if (child.flags.match_any(Flag::pendingDetach))
				child.detach(*this);

			return remove;
		});

		flags.reset(Flag::pendingDetachChild);
	}

	if (flags.match_any(Flag::pendingDetachSelf)) {
		log_ui.trace("Detaching {}", path());

		doDetach();

		context_ = nullptr;
		parent = libv::make_observer_ref(this);

		flags.reset(Flag::pendingDetach);
	}
}

void ComponentBase::style() {
	if (flags.match_any(Flag::pendingStyleSelf)) {
		doStyle();
		parent->doStyle(childID);
		flags.reset(Flag::pendingStyleSelf);
	}
	if (flags.match_any(Flag::pendingStyleChild)) {
		foreachChildren([](ComponentBase& child) {
			if (child.flags.match_any(Flag::pendingStyle))
				child.style();
		});
		flags.reset(Flag::pendingStyleChild);
	}
}

void ComponentBase::styleScan() {
	if (flags.match_any(Flag::pendingStyleSelf) || (style_ && style_->isDirty())) {
		doStyle();
		parent->doStyle(childID);
		flags.reset(Flag::pendingStyleSelf);
	}
	foreachChildren([](ComponentBase& child) {
		child.styleScan();
	});
}

void ComponentBase::render(ContextRender& context) {
	ContextRender currentContext{
		context.gl,
		flags.match_any(Flag::updatedPosition | Flag::updatedSize),
		flags.match_any(Flag::updatedPosition),
		flags.match_any(Flag::updatedSize)};

	if (isRendered()) {
		if (flags.match_any(Flag::pendingCreate)) {
			doCreate(currentContext);
			flags.reset(Flag::pendingCreate);
		}

		{
	//	if (flags.match_any(Flag::pendingRender)) {
			doRender(currentContext);

			foreachChildren([&currentContext](ComponentBase& child) {
				child.render(currentContext);
			});
	//		flags.reset(Flag::pendingRender);
		}

		flags.reset(Flag::updatedPosition | Flag::updatedSize);
	}

	if (flags.match_any(Flag::pendingDetachSelf)) {
		doDestroy(currentContext);
		return;
	}
}

void ComponentBase::layout1(const ContextLayout1& environment) {
	bool dirty = flags.match_any(Flag::pendingLayout);

	if (dirty) {
		this->doLayout1(environment);
		log_ui.trace("Layout content {:>11}, {}", lastContent, path());
	}
}

void ComponentBase::layout2(const ContextLayout2& environment) {
//	log_ui.trace("Layout Pass2 {}", path());
	bool dirty = flags.match_any(Flag::pendingLayout);

	if (environment.position != position_) {
		dirty = true;
		flags.set(Flag::updatedPosition);
		position_ = environment.position;
	}

	if (environment.size != size_) {
		dirty = true;
		flags.set(Flag::updatedSize);
		size_ = environment.size;
	}

	if (dirty) {
		this->doLayout2(environment);

		flags.reset(Flag::pendingLayout);
		log_ui.trace("Layout {:>11}, {:>11}, {}", position_, size_, path());
	}
}

void ComponentBase::foreachChildren(libv::function_ref<void(ComponentBase&)> callback) {
	doForeachChildren(callback);
}

// -------------------------------------------------------------------------------------------------

void ComponentBase::doAttach() { }

void ComponentBase::doDetach() { }

void ComponentBase::doDetachChildren(libv::function_ref<bool(ComponentBase&)> callback) {
	(void) callback;
}

void ComponentBase::doStyle() { }

void ComponentBase::doStyle(uint32_t childID) {
	(void) childID;
}

void ComponentBase::doCreate(ContextRender& context) {
	(void) context;
}

void ComponentBase::doDestroy(ContextRender& context) {
	(void) context;
}

void ComponentBase::doRender(ContextRender& context) {
	(void) context;
}

void ComponentBase::doLayout1(const ContextLayout1& environment) {
	(void) environment;
}

void ComponentBase::doLayout2(const ContextLayout2& environment) {
	(void) environment;
}

void ComponentBase::doForeachChildren(libv::function_ref<void(ComponentBase&)> callback) {
	(void) callback;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
