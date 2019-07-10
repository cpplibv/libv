// File: layout_test_utils.hpp - Created on 2017.10.18. 04:56 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/ui/component_base.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/utility/approxing.hpp>
// std
#include <memory>
#include <ostream>


// -------------------------------------------------------------------------------------------------

namespace {

using Pos = libv::vec3_t<libv::Approxing<float>>;
using Size = libv::vec3_t<libv::Approxing<float>>;

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
		const auto rhs_pos_approx = libv::vec3_t<libv::Approxing<float>>{rhs.position};
		const auto rhs_size_approx = libv::vec3_t<libv::Approxing<float>>{rhs.size};
		return lhs.position == rhs_pos_approx && lhs.size == rhs_size_approx;
	}

	friend std::ostream& operator<<(std::ostream& os, const Bounds& bounds) {
		return os << "p:" << bounds.position << ", s:" << bounds.size;
	}
};

class TestComponent : public libv::ui::ComponentBase {
public:
	libv::vec3f dynamicSize = {};

	TestComponent() : libv::ui::ComponentBase(libv::ui::UnnamedTag{}, "test") {};

	virtual void doLayout1(const libv::ui::ContextLayout1&) override {
		lastContent = dynamicSize;
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
		std::static_pointer_cast<TestComponent>(components[index].ptr)->dynamicSize = value;
	}

	void dynamic(float x, float y, float z) {
		dynamic({x, y, z});
	}

	void dynamic(float x, float y) {
		dynamic(x, y, 0);
	}

	Bounds bounds() {
		return {components[index].ptr->position(), components[index].ptr->size()};
	}
};

template <typename T, typename TC>
class BasicTestLayout {
protected:
	TestComponent ignore;
	typename T::Properties properties;
	std::vector<typename T::Child> components;

public:
	BasicTestLayout() {
		ignore.set(properties); // Initialize properties with fallback values
	}

public:
	TC add() {
		auto& child = components.emplace_back(std::make_shared<TestComponent>());
		child.ptr->set(child.properties); // Initialize child properties with fallback values
		return TC{components, components.size() - 1};
	}

	libv::vec3f layout1(libv::ui::ContextLayout1& le) {
		return T::layout1(le, components, properties, ignore);
	}

	void layout2(libv::ui::ContextLayout2& le) {
		return T::layout2(le, components, properties, ignore);
	}
};

} // namespace

// -------------------------------------------------------------------------------------------------
