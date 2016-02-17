// File: Component.hpp, Created on 2014. január 6. 15:45, Author: Vader

#pragma once

// libv
#include <libv/memory.hpp>
#include <libv/vec.hpp>
// std
#include <atomic>
#include <memory>
#include <string>
// pro
#include <libv/ui/component/ui.hpp>
#include <libv/ui/properties.hpp>
#include <libv/ui/render/renderer.hpp>

// TODO P4: Protect component DisplayPosition, DisplaySize, Parent and Frame?

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Frame;
class Container;

class Component {
protected:
	std::string componentID;
	std::string componentClass;

	vec3 displayPosition;
	vec3 displaySize;

	std::atomic<bool> invalid{true};
	PropertyMap properties;

public:
	vec3 getDisplayPosition() const;
	vec3 getDisplaySize() const;
	void setDisplayPosition(vec3 pos);
	void setDisplaySize(vec3 size);

public:
	template<typename T>
	T get(const PropertyMap::Address<T>& address) const {
		auto ptr = properties.get(address);
		return ptr ? *ptr : T();
	}
	template <typename T, typename Value>
	void set(const PropertyMap::Address<T>& address, Value&& value) {
		properties.set(address, std::forward<Value>(value));
	}
	PropertyMap::SetterProxy set() {
		return properties.set();
	}
	void setComponentID(std::string componentID) {
		this->componentID = componentID;
	}

protected:
	void validate();
	/** Invalidates the object requesting it to rebuild before the next render.
	 * @context ANY */
	void invalidate();
	bool isInvalid() const;

public:
	void visit(std::function<void(libv::observer_ptr<Component>)>&);
private:
	virtual void doVisit(std::function<void(libv::observer_ptr<Component>)>&);

public:
	/** Control function. Called before render if the object was previously invalidated.
	 * @param renderer - The render context
	 * @note Does not require of previous destroy
	 * @context GL */
	void build(Renderer& renderer);
	/** Control function. Called before the object is destroyed to cleanup OpenGL resources.
	 * @param renderer - The render context
	 * @note Does not require of previous build
	 * @context GL */
	void destroy(Renderer& renderer);
	/** Control function. Renders the object.
	 * @note Does not require of previous build
	 * @param renderer - The render context
	 * @context GL */
	void render(Renderer& renderer);

private:
	virtual void doBuild(Renderer& renderer) = 0;
	virtual void doDestroy(Renderer& renderer) = 0;
	virtual void doRender(Renderer& renderer) = 0;

public:
	Component();
	Component(const std::string& componentID, const std::string& componentClass);
	virtual ~Component();
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv