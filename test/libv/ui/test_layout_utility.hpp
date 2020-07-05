// File: layout_test_utils.hpp - Created on 2017.10.18. 04:56 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/approx.hpp>
// std
#include <memory>
#include <ostream>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/style.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

using Pos = libv::vec3_t<libv::Approx<float>>;
using Size = libv::vec3_t<libv::Approx<float>>;

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
		const auto rhs_pos_approx = libv::vec3_t<libv::Approx<float>>{rhs.position};
		const auto rhs_size_approx = libv::vec3_t<libv::Approx<float>>{rhs.size};
		return lhs.position == rhs_pos_approx && lhs.size == rhs_size_approx;
	}

	friend std::ostream& operator<<(std::ostream& os, const Bounds& bounds) {
		return os << "p:" << bounds.position << ", s:" << bounds.size;
	}
};

class TestComponent : public libv::ui::CoreComponent {
public:
	libv::vec3f dynamicSize = {};

	TestComponent(CoreComponent& parent) :
			libv::ui::CoreComponent(parent, libv::ui::GenerateName, "test") {};

	virtual void doLayout1(const libv::ui::ContextLayout1&) override {
		libv::ui::AccessLayout::lastDynamic(*this) = dynamicSize;
	}
};

template <typename T>
class BasicTestChild {
protected:
	std::vector<typename T::Child>& components;
	size_t index;

public:
	BasicTestChild(std::vector<typename T::Child>& components, size_t index) :
		components(components), index(index) { }

public:
	void dynamic(libv::vec3f value) {
		static_cast<TestComponent&>(components[index].ptr.core()).dynamicSize = value;
	}

	void dynamic(float x, float y, float z) {
		dynamic({x, y, z});
	}

	void dynamic(float x, float y) {
		dynamic(x, y, 0);
	}

	Bounds bounds() {
		return {components[index].ptr.core().position(), components[index].ptr.core().size()};
	}
};

template <typename T, typename TC>
class BasicTestLayout {
protected:
	TestComponent ignore{static_cast<libv::ui::CoreComponent&>(ignore)};
	// Fix UB: passing an uninitialized object address to itself to copy the context ptr that is not used
	typename T::Properties property;
	std::vector<typename T::Child> components;

public:
	BasicTestLayout() {
		// Initialize property with fallback values
		libv::ui::ContextStyle ctx{nullptr, ignore};
		T::style(property, ctx);
	}

public:
	TC add() {
		auto sp = std::make_shared<TestComponent>(static_cast<libv::ui::CoreComponent&>(ignore));
		auto& child = components.emplace_back(sp);

		// Initialize child property with fallback values
		libv::ui::ContextStyle ctx{nullptr, ignore};
		T::style(child.property, ctx);

		return TC{components, components.size() - 1};
	}

	libv::vec3f layout1(libv::ui::ContextLayout1& le) {
		return T::layout1(le, components, property, ignore);
	}

	void layout2(libv::ui::ContextLayout2& le) {
		return T::layout2(le, components, property, ignore);
	}
};

} // namespace

// -------------------------------------------------------------------------------------------------
