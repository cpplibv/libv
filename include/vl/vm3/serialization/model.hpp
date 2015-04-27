// File: Model.hpp, Created on 2015. március 6. 16:29, Author: Vader

#pragma once

// hpp
#include "vl/vm3/model.hpp"
// conf
#include "vl/vm3/config.hpp"
// vl
#include <vl/serialization/nvp.hpp>
#include <vl/serialization/glm.hpp>
// pro
#include "vl/vm3/serialization/material.hpp"

namespace vl {
namespace vm3 {

struct ModelSerialization {
	template<typename Archive>
	static inline void serialize(Archive& ar, ::vl::vm3::Model& var, const unsigned int) {
		ar & VL_NVP_NAMED("name", var.name);
		ar & VL_NVP_NAMED("materials", var.materials);
		ar & VL_NVP_NAMED("meshes", var.meshes);
//		ar & VL_NVP_NAMED("nodes", var.nodes);
//		ar & VL_NVP_NAMED("rootNode", var.rootNode);
		ar & VL_NVP_NAMED("nodes", var.nodes);
		ar & VL_NVP_NAMED("lods", var.lods);
		ar & VL_NVP_NAMED("vertices", var.vertices);
		ar & VL_NVP_NAMED("indices", var.indices);
	}
};
} //namespace vm3
} //namespace vl
#include <iostream>
namespace boost {
namespace serialization {
template<typename Archive>
void serialize(Archive& ar, ::vl::vm3::Vertex& var, const unsigned int) {
	ar & VL_NVP_NAMED("position", var.position);
	ar & VL_NVP_NAMED("normal", var.normal);
	ar & VL_NVP_NAMED("tangent", var.tangent);
	ar & VL_NVP_NAMED("bitangent", var.bitangent);
	ar & VL_NVP_NAMED("texCoord0", var.texCoord0);
	ar & VL_NVP_NAMED("boneID", var.boneID);
	ar & VL_NVP_NAMED("boneWieght", var.boneWieght);
}
template<typename Archive>
void serialize(Archive& ar, ::vl::vm3::Mesh& var, const unsigned int) {
	ar & VL_NVP_NAMED("name", var.name);
	ar & VL_NVP_NAMED("baseIndex", var.baseIndex);
	ar & VL_NVP_NAMED("baseVertex", var.baseVertex);
	ar & VL_NVP_NAMED("numIndices", var.numIndices);
	ar & VL_NVP_NAMED("material", var.materialID);
}
template<typename Archive>
void serialize(Archive& ar, ::vl::vm3::Node& var, const unsigned int) {
	ar & VL_NVP_NAMED("name", var.name);
	ar & VL_NVP_NAMED("parent", var.parentID);
	//ar & VL_NVP_NAMED("visibility", var.visibility);
	ar & VL_NVP_NAMED("transformation", var.transformation);
	ar & VL_NVP_NAMED("meshes", var.meshIDs);
	ar & VL_NVP_NAMED("children", var.childrenIDs);
}
template<typename Archive>
void serialize(Archive& ar, ::vl::vm3::LOD& var, const unsigned int) {
	ar & VL_NVP_NAMED("rangeNear", var.rangeNear);
	ar & VL_NVP_NAMED("rangeFar", var.rangeFar);
	ar & VL_NVP_NAMED("rootNode", var.rootNodeID);
}
template<typename Archive>
void serialize(Archive& ar, ::vl::vm3::Model& var, const unsigned int ver) {
	::vl::vm3::ModelSerialization::serialize(ar, var, ver);
}
} //namespace boost
} //namespace serialization