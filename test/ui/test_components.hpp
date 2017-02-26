// File:   test_components.hpp Author: Vader Created on 2016. április 4., 16:09

#pragma once

#include <libv/ui/component/component.hpp>
#include <libv/ui/container/panel_flow.hpp>
#include <libv/ui/layout.hpp>
#include <libv/ui/properties_eval.hpp>

struct TestQuad : public libv::ui::Component {
	TestQuad(const std::string& id) {
		setComponentID(id);
	}
private:
	virtual libv::ui::LayoutInfo doLayout(const libv::ui::LayoutInfo& parentLayoutInfo) override {
		return libv::ui::evalLayoutSize(parentLayoutInfo, *this);
	}
	virtual void doBuild(libv::ui::Context&) override { }
	virtual void doDestroy(libv::ui::Context&) override { }
	virtual void doRender(libv::ui::Context&) override { }
};

struct TestQuadDynamicLayout : public libv::ui::Component {
	libv::vec3f size;
	TestQuadDynamicLayout(const std::string& id) {
		setComponentID(id);
	}
private:
	virtual libv::ui::LayoutInfo doLayout(const libv::ui::LayoutInfo&) override {
		return libv::ui::LayoutInfo(size);
	}
	virtual void doBuild(libv::ui::Context&) override { }
	virtual void doDestroy(libv::ui::Context&) override { }
	virtual void doRender(libv::ui::Context&) override { }
};

struct TestPanel : public libv::ui::PanelFlow {
	TestPanel(const std::string& id) {
		setComponentID(id);
	}
	libv::ui::Container::Store& getComponents() {
		return components;
	}
};
