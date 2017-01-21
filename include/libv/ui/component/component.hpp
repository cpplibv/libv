// File: Component.hpp, Created on 2014. január 6. 15:45, Author: Vader

#pragma once

// libv
#include <libv/memory.hpp>
#include <libv/vec.hpp>
#include <libv/sig/signal.hpp>
// std
#include <atomic>
#include <memory>
#include <string>
// pro
#include <libv/ui/layout.hpp>
#include <libv/ui/properties.hpp>

namespace libv {
namespace ui {

class LayoutInfo;
class ParentAccessor;
class UI;
class Context;

// -------------------------------------------------------------------------------------------------

class Component {
	friend class ParentAccessor;
protected:
	std::atomic<bool> invalid{true};
private:
	observer_ptr<Component> parent;
	observer_ptr<UI> ui;
	// TODO P4: parent and ui ptrs are currently hectic: root component: real ui and null as parent; every other component: null ui and real parent
	LayoutInfo lastParentLayoutInfo;
	LayoutInfo lastResultLayoutInfo;
protected:
	std::string componentID;
	std::string componentClass;

	PropertyMap properties;

public:
	template<typename T>
	inline decltype(auto) get(const PropertyAddress<T>& address) const {
		return properties.get(address);
	}
	template <typename T>
	inline decltype(auto) getOrUse(const PropertyAddress<T>& address, const T& def) const {
		return properties.getOrUse(address, def);
	}
	template <typename T>
	inline decltype(auto) getOrDefault(const PropertyAddress<T>& address) const {
		return properties.getOrDefault(address);
	}
	template <typename T, typename Value>
	void set(const PropertyAddress<T>& address, Value&& value) {
		invalidate();
		properties.set(address, std::forward<Value>(value));
	}
	template <typename T, typename... Args, typename = decltype(T(std::declval<Args>()...))>
	void set(const PropertyAddress<T>& address, Args&&... args) {
		invalidate();
		properties.set(address, T(std::forward<Args>(args)...));
	}
	PropertyMap::SetterProxy set() {
		invalidate();
		return properties.set();
	}
	const std::string& getComponentID() const {
		return this->componentID;
	}
	void setComponentID(std::string componentID) {
		this->componentID = componentID;
	}
protected:
	Context& getContext();
	void validate();
	/** Invalidates the object requesting it to rebuild before the next render.
	 * @context ANY */
	void invalidate();
public:
	bool isInvalid() const;

	//public:
	//	void visit(std::function<void(Component&)>&);
	//private:
	//	virtual void doVisit(std::function<void(Component&)>&);

public:
	/** Layout the underlying component into the received layout.
	 * @param layout - The owner layout
	 * @context ANY */
	LayoutInfo layout(const LayoutInfo& parentLayout);

	/** Control function. Called before render if the object was previously invalidated.
	 * @param renderer - The render context
	 * @note Does not require of previous destroy
	 * @context GL */
	void build(Context& context);
	/** Control function. Called before the object is destroyed to cleanup OpenGL resources.
	 * @param renderer - The render context
	 * @note Does not require of previous build
	 * @context GL */
	void destroy(Context& context);
	/** Control function. Renders the object.
	 * @note Does not require of previous build
	 * @param renderer - The render context
	 * @context GL */
	void render(Context& context);

private:
	virtual LayoutInfo doLayout(const LayoutInfo& parentLayout) = 0;
	virtual void doBuild(Context& context) = 0;
	virtual void doDestroy(Context& context) = 0;
	virtual void doRender(Context& context) = 0;

public:
	Component();
	Component(const std::string& componentID, const std::string& componentClass);
	virtual ~Component();
};

// -------------------------------------------------------------------------------------------------

struct ParentAccessor {
	static void setParent(const observer_ptr<Component>& component, const observer_ptr<Component>& parent) {
		component->parent = parent;
	}
	static void setUI(const observer_ptr<Component>& component, const observer_ptr<UI>& ui) {
		component->ui = ui;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
