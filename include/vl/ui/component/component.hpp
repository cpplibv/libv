// File: Component.hpp, Created on 2014. január 6. 15:45, Author: Vader

#pragma once

// vl
#include <vl/memory.hpp>
#include <vl/vec.hpp>
// std
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
	//TODO P4: change to proxy operator() syntax (that result is less template generated) (see boost po)
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
	Component(ivec3 size);
	Component(ivec3 size, ivec3 pos);
	virtual ~Component();
};

struct ComponentPtr {
	observer_ptr<Component> ptr;
	shared_ptr<Component> sp;

	explicit inline ComponentPtr(std::nullptr_t) noexcept : ptr(nullptr) { }
	explicit inline ComponentPtr(const observer_ptr<Component>& ptr) noexcept : ptr(ptr) { }
	explicit inline ComponentPtr(observer_ptr<Component>&& ptr) noexcept : ptr(ptr) { }
	explicit inline ComponentPtr(const shared_ptr<Component>& sp) noexcept : ptr(sp.get()), sp(sp) { }
	explicit inline ComponentPtr(shared_ptr<Component>&& sp) noexcept : ptr(sp.get()), sp(std::move(sp)) { }

	ComponentPtr& operator=(const observer_ptr<Component>& ptr) {
		this->ptr = ptr;
		return *this;
	}
	ComponentPtr& operator=(observer_ptr<Component>&& ptr) {
		this->ptr = ptr;
		return *this;
	}
	ComponentPtr& operator=(const shared_ptr<Component>& sp) {
		this->ptr = observer_ptr<Component>(sp.get());
		this->sp = sp;
		return *this;
	}
	ComponentPtr& operator=(shared_ptr<Component>&& sp) {
		this->ptr = observer_ptr<Component>(sp.get());
		this->sp = std::move(sp);
		return *this;
	}

	inline Component* get() const noexcept {
		return ptr.get();
	}
	inline Component& operator*() const {
		return *ptr.get();
	}
	inline Component* operator->() const noexcept {
		return ptr.get();
	}
	inline operator Component*() const noexcept {
		return ptr.get();
	}
	inline operator observer_ptr<Component>() const noexcept {
		return ptr;
	}
	explicit inline operator bool() const noexcept {
		return ptr != nullptr;
	}
};

} //namespace ui
} //namespace vl

//	void getComponentOrientation();
//	int getComponentZOrder(ComponentPtr);
//	Font2D getFont();
//	void invalidate();