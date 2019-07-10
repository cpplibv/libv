// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component_static.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

class Canvas3D : public libv::ui::ComponentStatic<Canvas3D> {
	friend class libv::ui::ComponentStaticAccess;

private:
	libv::ui::Label label_help;

private:
	template <typename T>
	void ui_access(T&& access) {
//		access(layout);
		access(label_help);
	}

public:
	Canvas3D();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
