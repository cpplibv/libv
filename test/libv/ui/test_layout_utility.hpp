// Project: libv.ui, File: test/libv/ui/test_layout_utility.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/approx.hpp>
// std
#include <ostream>
// pro
//#include <libv/ui/context/context_style.hpp>
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component_system/core_ptr.hpp>
#include <libv/ui/component_system/component_api.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_ui_link.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/utility/approx.hpp>


// -------------------------------------------------------------------------------------------------

using approx_size = libv::vec3_t<libv::approx<float, -3>>;
using approx_pos = libv::vec3_t<libv::approx<float, -3>>;

struct Bounds {
	libv::vec3f position;
	libv::vec3f size;

	Bounds(libv::vec3f position, libv::vec3f size) :
			position(position),
			size(size) { }

	Bounds(float px, float py, float sx, float sy) :
			position(px, py, 0),
			size(sx, sy, 0) { }

	Bounds(float px, float py, float pz, float sx, float sy, float sz) :
			position(px, py, pz),
			size(sx, sy, sz) { }

	bool friend operator==(const Bounds& lhs, const Bounds& rhs) {
		const auto rhs_pos_approx = libv::vec3_t<libv::approx<float>>{rhs.position};
		const auto rhs_size_approx = libv::vec3_t<libv::approx<float>>{rhs.size};
		return lhs.position == rhs_pos_approx && lhs.size == rhs_size_approx;
	}

	friend std::ostream& operator<<(std::ostream& os, const Bounds& bounds) {
		return os << "p:" << bounds.position << ", s:" << bounds.size;
	}
};

struct DynamicTag : libv::vec3f{
	using libv::vec3f::vec3f;
};
struct MarginTag : libv::vec4f{
	using libv::vec4f::vec4f;
};
struct PaddingTag : libv::vec4f{
	using libv::vec4f::vec4f;
};

using D = DynamicTag;
using M = MarginTag;
using P = PaddingTag;

struct CoreTestComponent : libv::ui::CoreComponent {
	libv::vec3f injected_dynamic_size;

	using CoreComponent::CoreComponent;

protected:
	virtual libv::vec3f doLayout1(const libv::ui::ContextLayout1& environment) override {
		(void) environment;
		return injected_dynamic_size + padding_size3();
	}
};

struct TestComponent : libv::ui::ComponentAPI<libv::ui::Component, TestComponent, class CoreTestComponent, libv::ui::EventHostGeneral> {
public:
	using libv::ui::ComponentAPI<libv::ui::Component, TestComponent, class CoreTestComponent, libv::ui::EventHostGeneral>::ComponentAPI;
	static constexpr std::string_view component_type = "test";
	static libv::ui::core_ptr create_core(std::string name) {
		return libv::ui::create_core_ptr<CoreType>(std::move(name));
	}

	void injected_dynamic_size(libv::vec3f v) {
		self().injected_dynamic_size = v;
	}

	Bounds bounds() const {
		return {layout_position(), layout_size()};
	}
};

template <typename T>
struct TestPanel : T {
	TestPanel() : T((libv::ui::current_thread_context(reinterpret_cast<libv::ui::ContextUI&>(*this)), "layout")) {
		// Hacky injection of this as a non-null contextUI, it will not be used anyways

		// Hacky styling to initialize properties of only this panel (Child are not yet attached, their non-base properties are ignored during testing)! AND It will crash if ContextUI is used to init a property
		libv::ui::AccessRoot::styleScanAll(this->core());
	}

private:
	void apply(TestComponent& comp) {
		(void) comp;
	};

	template <typename... Tags>
	void apply(TestComponent& comp, DynamicTag t, Tags&&... tags) {
		comp.injected_dynamic_size(t);
		apply(comp, std::forward<Tags>(tags)...);
	}

	template <typename... Tags>
	void apply(TestComponent& comp, PaddingTag t, Tags&&... tags) {
		comp.padding(t);
		apply(comp, std::forward<Tags>(tags)...);
	}

	template <typename... Tags>
	void apply(TestComponent& comp, MarginTag t, Tags&&... tags) {
		comp.margin(t);
		apply(comp, std::forward<Tags>(tags)...);
	}

public:
	template <typename... Tags>
	TestComponent add(const std::string_view size, Tags&&... tags) {
		TestComponent comp;
		comp.size(libv::ui::parse_size_or_throw(size));
		apply(comp, std::forward<Tags>(tags)...);
		T::add(comp);
		return comp;
	}

public:
	libv::vec3f layout1(libv::vec3f constraint_size) {
		return libv::ui::AccessRoot::layout1(this->core(), libv::ui::ContextLayout1{constraint_size});
	}
	libv::vec3f layout1(float cx, float cy, float cz = 0.f) {
		return layout1({cx, cy, cz});
	}
	void layout2(libv::vec3f pos, libv::vec3f size) {
		libv::ui::AccessRoot::layout2(this->core(), libv::ui::ContextLayout2{pos, size});
	}
	void layout2(float px, float py, float pz, float sx, float sy, float sz) {
		layout2({px, py, pz}, {sx, sy, sz});
	}
	void layout2(float px, float py, float sx, float sy) {
		layout2({px, py, 0}, {sx, sy, 0});
	}
	void layout2(float sx, float sy, float sz = 0) {
		layout2({0, 0, 0}, {sx, sy, sz});
	}
};

// -------------------------------------------------------------------------------------------------
