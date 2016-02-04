// File: panel.hpp, Created on 2015. április 8. 14:32, Author: Vader

#pragma once

#include <libv/ui/component/component.hpp>
#include <libv/ui/component/container.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BasePanel : public Container, public Component {
	virtual void doBuild(Renderer& renderer) override;
	virtual void doDestroy(Renderer& renderer) override;
	virtual void doRender(Renderer& renderer) override;
public:
	virtual ~BasePanel() = default;
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv