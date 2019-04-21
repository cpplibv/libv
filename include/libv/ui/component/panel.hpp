// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// pro
#include <libv/ui/component_static.hpp>
#include <libv/ui/layout_line.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Panel : public ComponentStatic<Panel> {
	friend class ComponentStaticAccess;

private:
public: // TODO P5: remove public
	libv::ui::LayoutLine layout; // <<< P4: Into properity layout you go

private:
	std::vector<std::shared_ptr<ComponentBase>> components;

private:
	template <typename T>
	void ui_access(T&& access) {
		access(layout);
		for (auto& component : components)
			access(*component);
	}

public:
	Panel();
	Panel(std::string name);

public:
	void add(std::shared_ptr<ComponentBase> component);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
