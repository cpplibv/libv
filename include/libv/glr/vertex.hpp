// File: vertex.hpp - Created on 2017.11.25. 05:04 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct VertexP {
	libv::vec3f position;

	template <typename Binder>
	static void bindAttributes(const Binder& bind) {
		bind(0, &VertexP::position, false);
	}
};

struct VertexPT {
	libv::vec3f position;
	libv::vec2f texture0;

	template <typename Binder>
	static void bindAttributes(const Binder& bind) {
		bind(0, &VertexPT::position, false);
		bind(8, &VertexPT::texture0, false);
	}
};

struct VertexPCT {
	libv::vec3f position;
	libv::vec4f color0;
	libv::vec2f texture0;

	template <typename Binder>
	static void bindAttributes(const Binder& bind) {
		bind(0, &VertexPCT::position, false);
		bind(2, &VertexPCT::color0, false);
		bind(8, &VertexPCT::texture0, false);
	}
};

struct VertexPNT {
	libv::vec3f position;
	libv::vec3f normal;
	libv::vec2f texture0;

	template <typename Binder>
	static void bindAttributes(const Binder& bind) {
		bind(0, &VertexPNT::position, false);
		bind(1, &VertexPNT::normal, false);
		bind(8, &VertexPNT::texture0, false);
	}
};

struct VertexPCNT {
	libv::vec3f position;
	libv::vec3f normal;
	libv::vec4f color0;
	libv::vec2f texture0;

	template <typename Binder>
	static void bindAttributes(const Binder& bind) {
		bind(0, &VertexPCNT::position, false);
		bind(1, &VertexPCNT::normal, false);
		bind(2, &VertexPCNT::color0, false);
		bind(8, &VertexPCNT::texture0, false);
	}
};

struct VertexPNTBT {
	libv::vec3f position;
	libv::vec3f normal;
	libv::vec3f tangent;
	libv::vec3f bitangent;
	libv::vec2f texture0;

	template <typename Binder>
	static void bindAttributes(const Binder& bind) {
		bind(0, &VertexPNTBT::position, false);
		bind(1, &VertexPNTBT::normal, false);
		bind(6, &VertexPNTBT::tangent, false);
		bind(7, &VertexPNTBT::bitangent, false);
		bind(8, &VertexPNTBT::texture0, false);
	}
};

struct VertexPNCTBT {
	libv::vec3f position;
	libv::vec3f normal;
	libv::vec4f color0;
	libv::vec3f tangent;
	libv::vec3f bitangent;
	libv::vec2f texture0;

	template <typename Binder>
	static void bindAttributes(const Binder& bind) {
		bind(0, &VertexPNCTBT::position, false);
		bind(1, &VertexPNCTBT::normal, false);
		bind(2, &VertexPNCTBT::color0, false);
		bind(6, &VertexPNCTBT::tangent, false);
		bind(7, &VertexPNCTBT::bitangent, false);
		bind(8, &VertexPNCTBT::texture0, false);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
