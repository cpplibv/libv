// File: Model.hpp, Created on 2015. március 6. 16:29, Author: Vader

#pragma once

// hpp
#include <libv/vm3/model.hpp>
// cfg
#include <libv/vm3/config.hpp>
// libv
#include <libv/serialization/nvp.hpp>
#include <libv/serialization/types/mat.hpp>
#include <libv/serialization/types/vec.hpp>
// pro
#include <libv/vm3/serialization/material.hpp>

namespace libv {
namespace vm3 {

constexpr char VM3_MODEL_MAGIC_BYTE[] = "d99575d0-ddf8-4462-8338-0f6755913d73/VM3";

// -------------------------------------------------------------------------------------------------

struct ModelSerialization {
	template <typename Archive>
	static inline void serialize(Archive& ar, ::libv::vm3::Model& var, const unsigned int) {
		ar & LIBV_NVP_NAMED("name", var.name);
		ar & LIBV_NVP_NAMED("materials", var.materials);
		ar & LIBV_NVP_NAMED("meshes", var.meshes);
		ar & LIBV_NVP_NAMED("nodes", var.nodes);
		//		ar & LIBV_NVP_NAMED("rootNode", var.rootNode);
		ar & LIBV_NVP_NAMED("lods", var.lods);
		ar & LIBV_NVP_NAMED("vertices", var.vertices);
		ar & LIBV_NVP_NAMED("indices", var.indices);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace vm3
} // namespace libv

namespace boost {
namespace serialization {

// -------------------------------------------------------------------------------------------------
template <typename Archive>
void serialize(Archive& ar, ::libv::vm3::Vertex& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("position", var.position);
	ar & LIBV_NVP_NAMED("normal", var.normal);
	ar & LIBV_NVP_NAMED("tangent", var.tangent);
	ar & LIBV_NVP_NAMED("bitangent", var.bitangent);
	ar & LIBV_NVP_NAMED("texCoord0", var.texCoord0);
	ar & LIBV_NVP_NAMED("boneID", var.boneID);
	ar & LIBV_NVP_NAMED("boneWieght", var.boneWieght);
}
template <typename Archive>
void serialize(Archive& ar, ::libv::vm3::Mesh& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("name", var.name);
	ar & LIBV_NVP_NAMED("baseIndex", var.baseIndex);
	ar & LIBV_NVP_NAMED("baseVertex", var.baseVertex);
	ar & LIBV_NVP_NAMED("numIndices", var.numIndices);
	ar & LIBV_NVP_NAMED("material", var.materialID);
}
template <typename Archive>
void serialize(Archive& ar, ::libv::vm3::Node& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("name", var.name);
	ar & LIBV_NVP_NAMED("parent", var.parentID);
	//ar & LIBV_NVP_NAMED("visibility", var.visibility);
	ar & LIBV_NVP_NAMED("transformation", var.transformation);
	ar & LIBV_NVP_NAMED("meshes", var.meshIDs);
	ar & LIBV_NVP_NAMED("children", var.childrenIDs);
}
template <typename Archive>
void serialize(Archive& ar, ::libv::vm3::LOD& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("rangeNear", var.rangeNear);
	ar & LIBV_NVP_NAMED("rangeFar", var.rangeFar);
	ar & LIBV_NVP_NAMED("rootNode", var.rootNodeID);
}
template <typename Archive>
void serialize(Archive& ar, ::libv::vm3::Model& var, const unsigned int ver) {
	::libv::vm3::ModelSerialization::serialize(ar, var, ver);
}

// -------------------------------------------------------------------------------------------------

} // namespace boost
} // namespace serialization