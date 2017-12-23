// File: vertex.hpp - Created on 2017.11.25. 05:04 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

//// -------------------------------------------------------------------------------------------------
//
//template <uint32_t Channel>
//struct Attribute {
//	static constexpr uint32_t channel = Channel;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//struct Position : Attribute<0> {
//	libv::vec3f position;
//};
//
//struct Normal : Attribute<1> {
//	libv::vec3f normal;
//};
//
//struct Color0 : Attribute<2> {
//	libv::vec4f color0;
//};
//
//struct Tangent : Attribute<6> {
//	libv::vec3f tangent;
//};
//
//struct Bitangent : Attribute<7> {
//	libv::vec3f bitangent;
//};
//
//struct Texture0 : Attribute<8> {
//	libv::vec2f texture0;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename... Attributes>
//struct Vertex : Attributes... {
//
//	template <typename Binder>
//	static void bindAttributes(const Binder& bind) {
////		(bind(Attributes::channel, &Attributes::What?, Attributes::normalize), ...);
//		// static_cast<Attributes*>(this)...
//		size_t offset;
//		(bind(Attributes::channel, sizeof(Vertex), offset += sizeof(Attributes)), ...);
//	}
//};
//
//using Vertex_v31 = Vertex<Position>;
//using Vertex_v32 = Vertex<Position, Normal, Color0, Tangent, Bitangent, Texture0>;
//
//// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
