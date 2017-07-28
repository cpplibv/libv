// File: render.hpp - Created on 2017.10.22. 10:21 - Author: Vader

#pragma once

// pro
#include <libv/ui/module.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextRender;

// -------------------------------------------------------------------------------------------------

struct ModuleRender : public Module {
public:
	void create(ContextRender& cr) {
		doCreate(cr);
	}
	void render(ContextRender& cr) {
		doRender(cr);
	}
	void destroy(ContextRender& cr) {
		doDestroy(cr);
	}
	virtual ~ModuleRender() = default;

private:
	virtual void doCreate(ContextRender&) = 0;
	virtual void doRender(ContextRender&) = 0;
	virtual void doDestroy(ContextRender&) = 0;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
