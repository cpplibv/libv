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

using approx_size = libv::vec2_t<libv::approx<float, -3>>;
using approx_pos = libv::vec2_t<libv::approx<float, -3>>;

struct Bounds {
	libv::vec2f position;
	libv::vec2f size;

	Bounds(libv::vec2f position, libv::vec2f size) :
			position(position),
			size(size) { }

	Bounds(float px, float py, float sx, float sy) :
			position(px, py),
			size(sx, sy) { }

	bool friend operator==(const Bounds& lhs, const Bounds& rhs) {
		const auto rhs_pos_approx = libv::vec2_t<libv::approx<float>>{rhs.position};
		const auto rhs_size_approx = libv::vec2_t<libv::approx<float>>{rhs.size};
		return lhs.position == rhs_pos_approx && lhs.size == rhs_size_approx;
	}

	friend std::ostream& operator<<(std::ostream& os, const Bounds& bounds) {
		return os << "p:" << bounds.position << ", s:" << bounds.size;
	}
};

struct DynamicTag : libv::vec2f{
	using libv::vec2f::vec2f;
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
	libv::vec2f injectedDynamicSize;
	libv::vec2b dynamicProvidedUnlimited;
	libv::vec2f dynamicProvidedLimit;
	libv::vec2f dynamicProvidedParent;

	using CoreComponent::CoreComponent;

protected:
	virtual libv::vec2f doLayout1(const libv::ui::ContextLayout1& environment) override {
		dynamicProvidedUnlimited = environment.unlimited;
		dynamicProvidedLimit = environment.limit;
		dynamicProvidedParent = environment.parent;
		return injectedDynamicSize + padding_size();
	}
};

struct TestComponent : libv::ui::ComponentAPI<libv::ui::Component, TestComponent, class CoreTestComponent, libv::ui::EventHostGeneral> {
public:
	using libv::ui::ComponentAPI<libv::ui::Component, TestComponent, class CoreTestComponent, libv::ui::EventHostGeneral>::ComponentAPI;
	static constexpr std::string_view component_type = "test";
	static libv::ui::core_ptr create_core(std::string name) {
		return libv::ui::create_core_ptr<CoreType>(std::move(name));
	}

	void injectedDynamicSize(libv::vec2f v) {
		self().injectedDynamicSize = v;
	}

	[[nodiscard]] Bounds bounds() const {
		return {layout_position(), layout_size()};
	}
	[[nodiscard]] libv::vec2b providedUnlimited() const {
		return self().dynamicProvidedUnlimited;
	}
	[[nodiscard]] libv::vec2f providedLimit() const {
		return self().dynamicProvidedLimit;
	}
	[[nodiscard]] libv::vec2f providedParent() const {
		return self().dynamicProvidedParent;
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
		comp.injectedDynamicSize(t);
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
	libv::vec2f layout1(libv::vec2f limit, libv::vec2f parent) {
		// assert(limit.x != 0);
		// assert(limit.y != 0);
		return libv::ui::AccessRoot::layout1(this->core(), libv::ui::ContextLayout1{
			libv::vec2b(limit.x < 0.f, limit.y < 0.f), limit, parent
		});
	}
	libv::vec2f layout1(libv::vec2f limit) {
		return layout1(limit, limit);
	}
	libv::vec2f layout1(float cx, float cy) {
		return layout1({cx, cy});
	}
	void layout2(libv::vec2f pos, libv::vec2f size) {
		libv::ui::AccessRoot::layout2(this->core(), libv::ui::ContextLayout2{pos, size});
	}
	void layout2(float px, float py, float sx, float sy) {
		layout2({px, py}, {sx, sy});
	}
	void layout2(float sx, float sy) {
		layout2({0, 0}, {sx, sy});
	}
};

// -------------------------------------------------------------------------------------------------
