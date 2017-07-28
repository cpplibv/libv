// File: layout_test_utils.hpp - Created on 2017.10.18. 04:56 - Author: Vader

#pragma once

#include <libv/ui4/component_static.hpp>
#include <libv/utility/approxing.hpp>


// -------------------------------------------------------------------------------------------------

using ApproxVec3f = libv::vec3_t<libv::Approxing<float>>;

struct Quad : libv::ui::ComponentStatic<Quad> {
	template <typename T>
	void ui_access(T&&) { }
};

template <typename C0>
struct Nest1 : libv::ui::ComponentStatic<Nest1<C0>> {
	C0 sub0;
	template <typename T> void ui_access(T && access) {
		access(sub0);
	}
};

template <typename C0, typename C1>
struct Nest2 : libv::ui::ComponentStatic<Nest2<C0, C1>> {
	C0 sub0;
	C1 sub1;
	template <typename T> void ui_access(T && access) {
		access(sub0);
		access(sub1);
	}
};

template <typename C0, typename C1, typename C2>
struct Nest3 : libv::ui::ComponentStatic<Nest3<C0, C1, C2>> {
	C0 sub0;
	C1 sub1;
	C2 sub2;
	template <typename T> void ui_access(T && access) {
		access(sub0);
		access(sub1);
		access(sub2);
	}
};

template <typename... C0>
struct Merge : C0... {
	template <typename T> void ui_access(T&& access) {
		(C0::ui_access(access), ...);
	}
};

// -------------------------------------------------------------------------------------------------
