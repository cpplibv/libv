// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/observer_ref.hpp>
// std
#include <functional>
#include <string>
#include <string_view>
// pro
#include <libv/ui/property/size.hpp>
#include <libv/ui/flag.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextLayoutPass1;
class ContextLayoutPass2;
class ContextRender;
class ContextUI;

// -------------------------------------------------------------------------------------------------

struct UnnamedTag {};

struct ComponentBase {
	Flag_t flags = Flag::mask_init;
	// <<< P1: These fields cannot be public
	libv::vec3f position; /// Component position absolute to origin
	libv::vec3f size;

	libv::vec3f lastPosition;
	libv::vec3f lastSize;

	libv::vec3f lastContent; /// Result of last layout pass1
	libv::observer_ref<ComponentBase> parent = libv::make_observer_ref(this);
	Size propertySize;

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
