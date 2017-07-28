// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/observer_ref.hpp>
#include <libv/utility/flag_enum.hpp>
// std
#include <functional>
#include <string>
#include <string_view>
// pro
#include <libv/ui/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextLayoutPass1;
class ContextLayoutPass2;
class ContextRender;
class ContextUI;

using Flag_t = libv::flag_enum<uint32_t, struct Flag_tag>;
namespace Flag {
static constexpr Flag_t invalidAttachStrong = libv::bit(0);
static constexpr Flag_t invalidAttachWeak = libv::bit(1);
static constexpr Flag_t invalidAttach = invalidAttachStrong | invalidAttachWeak;
static constexpr Flag_t invalidLayout = libv::bit(2);
static constexpr Flag_t invalidRender = libv::bit(3);

static constexpr Flag_t maskPropagateUp = invalidAttachWeak | invalidLayout | invalidRender;
//static constexpr Flag_t propagateDown = ;

static constexpr Flag_t renderChangeSize = libv::bit(4);
static constexpr Flag_t renderChangePosition = libv::bit(5);

//static constexpr Flag_t invalidLayoutStrong = libv::bit(4); /// Layout has been invalidated
//static constexpr Flag_t invalidLayoutWeak = libv::bit(5); /// Has at least one children with invalidated layout
//static constexpr Flag_t invalidLayout = invalidLayoutStrong | invalidLayoutWeak;
//static constexpr Flag_t hasChildren = libv::bit(7);
//static constexpr Flag_t hasChildrenWithRender = libv::bit(8);
//static constexpr Flag_t hasChildrenWithMouse = libv::bit(9);
//static constexpr Flag_t hasChildrenWithKeyboard = libv::bit(10);
//static constexpr Flag_t hasChildrenWithFocus = libv::bit(11);

} // namespace flag

// -------------------------------------------------------------------------------------------------

struct UnnamedTag {};

struct ComponentBase {
	// <<< P1: These fields cannot be public
	Flag_t flags =
			Flag::invalidAttach |
			Flag::invalidRender |
			Flag::invalidLayout |
			Flag::renderChangeSize |
			Flag::renderChangePosition;
	libv::vec3f position; /// Component position absolute to origin
	libv::vec3f size;

	libv::vec3f lastPosition;
	libv::vec3f lastSize;

	libv::observer_ref<ComponentBase> parent = libv::make_observer_ref(this);
	Size propertySize;
	libv::vec3f lastContent; /// Result of last layout pass1
	//boost::container::flat_map<PropertyID, Property> properties;

	inline static size_t nextID = 0;
	std::string name;

public:
	ComponentBase(std::string name);
	ComponentBase(UnnamedTag, const std::string_view type);
	virtual ~ComponentBase() = default;

public:
	void invalidate(Flag_t flags_);
	std::string path() const;

public:
	void attach(ContextUI& context);
	void create(ContextRender& context);
	void render(ContextRender& context);
	void destroy(ContextRender& context);
	void layoutPass1(const ContextLayoutPass1& environment);
	void layoutPass2(const ContextLayoutPass2& environment);
	void foreachChildren(const std::function<void(ComponentBase&)>& callback);

private:
	virtual void doAttach(ContextUI& context) { (void) context; };

	virtual void doCreate(ContextRender& context) { (void) context; };
	virtual void doRender(ContextRender& context) { (void) context; };
	virtual void doDestroy(ContextRender& context) { (void) context; };

	virtual void doLayoutPass1(const ContextLayoutPass1& le) { (void) le; };
	virtual void doLayoutPass2(const ContextLayoutPass2& le) { (void) le; };

	virtual void doForeachChildren(const std::function<void(ComponentBase&)>& callback) { (void) callback; };

	// TWO PASS layout:
	// - Pass 1: calculate everything as content bottom-top and store the result
	// - Pass 2: calculate everything top-down expanding every encounter of size where the stored value is less then the size property
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
