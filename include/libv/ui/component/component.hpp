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
//#include <libv/ui/component/ui.hpp>
#include <libv/ui/properties.hpp>
#include <libv/ui/render/renderer.hpp>

// TODO P4: Protect component DisplayPosition, DisplaySize, Parent and Frame?

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//class Frame;
//class Container;
class Layout;

class Component {
protected:
	std::string componentID;
	std::string componentClass;

	std::atomic<bool> invalid{true};
	PropertyMap properties;

public:
	template<typename T>
	decltype(auto) get(const PropertyAddress<T>& address) const {
		return properties.get(address);
	}
	template <typename T, typename Value>
	void set(const PropertyAddress<T>& address, Value&& value) {
//		onPropertyChange.fire();
		onLayoutChange.fire();
		invalidate();
		properties.set(address, std::forward<Value>(value));
	}
	PropertyMap::SetterProxy set() {
//		onPropertyChange.fire();
		invalidate();
		return properties.set();
	}
	const std::string& getComponentID() {
		return this->componentID;
	}
	void setComponentID(std::string componentID) {
		this->componentID = componentID;
	}

public:
//	Signal<> onPropertyChange;
//	Signal<> onInvalidation;
	Signal<> onLayoutChange;

protected:
	void validate();
	/** Invalidates the object requesting it to rebuild before the next render.
	 * @context ANY */
	void invalidate();
	bool isInvalid() const;

//public:
//	void visit(std::function<void(Component&)>&);
//private:
//	virtual void doVisit(std::function<void(Component&)>&);

public:
	/** Layout the underlying component into the received layout.
	 * @param layout - The owner layout
	 * @context ANY */
	Layout layout(const Layout& parentLayout);

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
	virtual Layout doLayout(const Layout& parentLayout);
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