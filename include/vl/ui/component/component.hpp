// File: Component.hpp, Created on 2014. január 6. 15:45, Author: Vader

#pragma once

// vl
#include <vl/memory.hpp>
#include <vl/vec.hpp>
// std
#include <atomic>
#include <memory>
#include <string>
// pro
#include <vl/ui/layout/properties.hpp>
#include <vl/ui/render/renderer.hpp>

namespace vl {
namespace ui {

class Frame;
class ProtectedContainer;

class Component {
private:
	ProtectedContainer* parent = nullptr;
	Frame* frame = nullptr;
protected:
	ivec3 pos;
	ivec3 size;
	std::string name;

	std::atomic<bool> invalid{true};
	PropertyMap properties;
public:
	//	bool isActive() const;
	//	bool isFocused() const;

	Frame* getFrame() const;
	ProtectedContainer* getParent() const;
	ivec3 getDisplayPosition() const;
	ivec3 getDisplaySize() const;

	void setParent(ProtectedContainer* parent);
	void setDisplayPosition(const ivec3& pos);
	void setDisplaySize(const ivec3& size);
	template <typename T, typename Value>
	void set(const PropertyMap::Address<T>& address, Value&& value) {
		properties.set(address, std::forward<Value>(value));
	}
	template <typename T, typename Value, typename... Args>
	void set(const PropertyMap::Address<T>& address, Value&& value, Args&&... args) {
		set(address, std::forward<Value>(value));
		set(std::forward<Args>(args)...);
	}
	//TODO P4: change to proxy operator() syntax (that result is less template generated) (see boost po)
	template<typename T>
	T get(const PropertyMap::Address<T>& address) const {
		return properties.get(address);
	}

public:
	bool isInvalid() const {
		return invalid;
	}
	void validate(){
		invalid = false;
	}

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

} //namespace ui
} //namespace vl