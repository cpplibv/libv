// Project: libv.vm4, File: src/libv/vm4/model.hpp

#pragma once

// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/serial/serial.hpp>
// std
#include <cstdint>
#include <vector>
// pro
#include <libv/vm4/material.hpp>

// TODO P2: Use dynarray instead of vector
// TODO P2: Use pointers instead of IDs (serialization with pointer <-> id resolving)


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

using MeshID = uint32_t;
using NodeID = uint32_t;

struct Animation {
	template <typename Archive> void serialize(Archive&) {
	}
};

struct AnimationChannel {
	template <typename Archive> void serialize(Archive&) {
	}
};

struct Vertex {
	libv::vec3f position;
	libv::vec3f normal;
	libv::vec3f tangent;
	libv::vec3f bitangent;
	libv::vec2f texture0;
	libv::vec4i boneID;
	libv::vec4f boneWeight;

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP(position));
		ar(LIBV_NVP(normal));
		ar(LIBV_NVP(tangent));
		ar(LIBV_NVP(bitangent));
		ar(LIBV_NVP(texture0));
		ar(LIBV_NVP(boneID));
		ar(LIBV_NVP(boneWeight));
	}
};

struct Mesh {
	std::string name;
	uint32_t baseIndex;
	uint32_t baseVertex;
	uint32_t numIndices;
	uint32_t materialID;
	//bounding box ?

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP(name));
		ar(LIBV_NVP(baseIndex));
		ar(LIBV_NVP(baseVertex));
		ar(LIBV_NVP(numIndices));
		ar(LIBV_NVP(materialID));
	}
};

struct Node {
	//bounding box ?
	std::string name;
	NodeID parentID;
	//bool visibility;
	libv::mat4f transform;
	std::vector<MeshID> meshIDs;
	std::vector<NodeID> childrenIDs;

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP(name));
		ar(LIBV_NVP(parentID));
		ar(LIBV_NVP(transform));
		ar(LIBV_NVP(meshIDs));
		ar(LIBV_NVP(childrenIDs));
	}
};

struct LOD {
	double rangeNear; //threshold for increasing LOD
	double rangeFar; //threshold for decreasing LOD
	NodeID rootNodeID;

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP(rangeNear));
		ar(LIBV_NVP(rangeFar));
		ar(LIBV_NVP(rootNodeID));
	}
};

struct Model {
	int32_t format_version = 20220324; /// VM4 format version identifier

//	int64_t hash; /// Pre-calculated hash value of the Model, useful to detect changes
//	binary<32> hash; /// Pre-calculated hash value of the Model, useful to detect changes
//	libv::Hash256 hash; /// Pre-calculated hash value of the Model, useful to detect changes
	int32_t model_version = 0; /// Incremental informal version number, identical version does not mean identical model
//	std::chrono::system_clock::time_point import_time;
	std::string name;

	libv::vec3f AABB_min;
	libv::vec3f AABB_max;
	libv::vec3f BS_origin; /// Bounding Sphere Origin
	float BS_radius; /// Bounding Sphere Radius

	std::vector<Animation> animations;
	std::vector<AnimationChannel> animationChannels;

	std::vector<Material> materials;
	std::vector<Mesh> meshes;
	std::vector<Node> nodes;

	std::vector<LOD> lods;

//	std::vector<libv::vec3f> vertex_positions;
//	std::vector<libv::vec3f> vertex_normals;
//	std::vector<libv::vec3f> vertex_tangents;
//	std::vector<libv::vec3f> vertex_bitangents;
//	std::vector<libv::vec2f> vertex_texCoord0s;
//	std::vector<libv::vec4i> vertex_boneIDs;
//	std::vector<libv::vec4f> vertex_boneWeights;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP(format_version));
	//	ar(LIBV_NVP(hash));
		ar(LIBV_NVP(model_version));
	//	ar(LIBV_NVP(import_time));
		ar(LIBV_NVP(name));

		ar(LIBV_NVP(AABB_min));
		ar(LIBV_NVP(AABB_max));
		ar(LIBV_NVP(BS_origin));
		ar(LIBV_NVP(BS_radius));

		ar(LIBV_NVP(animations));
		ar(LIBV_NVP(animationChannels));
		ar(LIBV_NVP(materials));
		ar(LIBV_NVP(meshes));
		ar(LIBV_NVP(nodes));
		ar(LIBV_NVP(lods));

	//	ar(LIBV_NVP(vertex_positions));
	//	ar(LIBV_NVP(vertex_normals));
	//	ar(LIBV_NVP(vertex_tangents));
	//	ar(LIBV_NVP(vertex_bitangents));
	//	ar(LIBV_NVP(vertex_texCoord0s));
	//	ar(LIBV_NVP(vertex_boneIDs));
	//	ar(LIBV_NVP(vertex_boneWeights));
		ar(LIBV_NVP(vertices));
		ar(LIBV_NVP(indices));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
