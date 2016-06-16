// File:   test_components.hpp Author: Vader Created on 2016. április 4., 16:09

#pragma once

#include <libv/ui/component/component.hpp>
#include <libv/ui/container/panel_flow.hpp>
#include <libv/ui/layout.hpp>

struct TestQuad : public libv::ui::Component {
	TestQuad(const std::string& id) {
		setComponentID(id);
	}
private:
	virtual libv::ui::Layout doLayout(const libv::ui::Layout&) override {
		libv::ui::Layout layout(*get(libv::ui::Property::Size));
		return layout;
	}
	virtual void doBuild(libv::ui::Renderer&) override { }
	virtual void doDestroy(libv::ui::Renderer&) override { }
	virtual void doRender(libv::ui::Renderer&) override { }
};

struct TestQuadDynamicLayout : public libv::ui::Component {
	libv::vec3 size;
	TestQuadDynamicLayout(const std::string& id) {
		setComponentID(id);
	}
private:
	virtual libv::ui::Layout doLayout(const libv::ui::Layout&) override {
		libv::ui::Layout layout(size);
		return layout;
	}
	virtual void doBuild(libv::ui::Renderer&) override { }
	virtual void doDestroy(libv::ui::Renderer&) override { }
	virtual void doRender(libv::ui::Renderer&) override { }
};

struct TestPanel : public libv::ui::PanelFlow {
	TestPanel(const std::string& id) {
		setComponentID(id);
	}
	libv::ui::Container::Store& getComponents() {
		return components;
	}
};
