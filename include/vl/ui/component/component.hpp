// File: Component.hpp, Created on 2014. január 6. 15:45, Author: Vader

#pragma once

// vl
#include <vl/vec.hpp>
#include <vl/memory.hpp>
// std
#include <string>
// pro
#include "vl/ui/layout/properties.hpp"
#include "vl/ui/render/renderer.hpp"

namespace vl {
namespace ui {

class Frame;
class ProtectedContainer;

class Component {
private:
	ProtectedContainer* parent = nullptr;
	Frame* frame = nullptr;
	ivec3 pos;
	ivec3 size;
	std::string name;

	bool invalid = false;
protected:
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
	template<typename T>
	T get(const PropertyMap::Address<T>& address) const {
		return properties.get(address);
	}

	virtual void build(Renderer& renderer);
	virtual void destroy(Renderer& renderer);
	virtual void invalidate();
	virtual void render(Renderer& renderer);
	virtual void update();

	Component();
	Component(ivec2 size);
	Component(ivec2 size, ivec2 pos);
	virtual ~Component();
};

using ComponentPtr = ::vl::shared_or_raw_ptr<Component>;

} //namespace ui
} //namespace vl

//	void getComponentOrientation();
//	int getComponentZOrder(ComponentPtr);
//	Font2D getFont();
//	void invalidate();