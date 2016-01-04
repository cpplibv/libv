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
#include <libv/ui/layout/properties.hpp>
#include <libv/ui/render/renderer.hpp>

// TODO P4: Protect component DisplayPosition and DisplaySize and Parent

namespace libv {
namespace ui {

class Frame;
class Container;

class Component {
private:
	Container* parent = nullptr;
	Frame* frame_ = nullptr;
protected:
	std::string propertyID;
	std::string propertyClass;

	ivec3 displayPosition;
	ivec3 displaySize;

	std::atomic<bool> invalid{true};
	PropertyMap properties;

public:
	void setDisplayPosition(const ivec3& pos);
	void setDisplaySize(const ivec3& size);
	ivec3 getDisplayPosition() const;
	ivec3 getDisplaySize() const;

	Container* getParent() const;
	void setParent(Container* parent);
	Frame* frame() const;

public:
	template<typename T>
	T get(const PropertyMap::Address<T>& address) const {
		return properties.get(address);
	}

	template <typename T, typename Value>
	void set(const PropertyMap::Address<T>& address, Value&& value) {
		properties.set(address, std::forward<Value>(value));
	}
	PropertyMap::SetterProxy set() {
		return properties.set();
	}

public:
	bool isInvalid() const;
	void validate();

public:
	/** OpenGL control function. Called before render if the object was previously invalidated.
	 * @param renderer
	 * @note Called by render function
	 * @context GL */
	virtual void build(Renderer& renderer);
	/** OpenGL control function. Called before the object is destroyed to cleanup OpenGL resources.
	 * @param renderer
	 * @note Called by UI
	 * @context GL */
	virtual void destroy(Renderer& renderer);
	/** Invalidates the object requesting it to rebuild before the next render.
	 * @context ANY */
	virtual void invalidate();
	/** OpenGL control function. Renders the object.
	 * @context ANY */
	virtual void render(Renderer& renderer);
	/** General control function. Updates the object.
	 * @note Called even if the object is not rendered.
	 * @context ANY */
	virtual void update();

public:
	Component();
	Component(ivec3 size);
	Component(ivec3 size, ivec3 pos);
	virtual ~Component();
};

//class Component : public Component {
//private:
//	using Component::getDisplayPosition;
//	using Component::getDisplaySize;
//	using Component::setDisplayPosition;
//	using Component::setDisplaySize;
//	using Component::setParent;
//protected:
//	using Component::getParent;
//
//protected:
//	using Component::build;
//	using Component::destroy;
//	using Component::invalidate;
//	using Component::render;
//	using Component::update;
//};

} //namespace ui
} //namespace libv


//	bool isActive() const;
//	bool isFocused() const;