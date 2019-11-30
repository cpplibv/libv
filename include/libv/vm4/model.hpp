// File: Model.hpp, Created on 2015. január 17. 7:21, Author: Vader

#pragma once

// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serialization/enable.hpp>
// std
#include <vector>
// pro
#include <libv/vm4/material.hpp>

// TODO P2: Use dynarray instead of vector
// TODO P2: Use pointers instead of IDs


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

using MeshID = uint32_t;
using NodeID = uint32_t;

struct Animation {
	LIBV_REFLECTION_EMPTY();
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct AnimationChannel {
	LIBV_REFLECTION_EMPTY();
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct Vertex {
	libv::vec3f position;
	libv::vec3f normal;
	libv::vec3f tangent;
	libv::vec3f bitangent;
	libv::vec2f texture0;
	libv::vec4i boneID;
	libv::vec4f boneWieght;

	LIBV_REFLECTION_ACCESS(position);
	LIBV_REFLECTION_ACCESS(normal);
	LIBV_REFLECTION_ACCESS(tangent);
	LIBV_REFLECTION_ACCESS(bitangent);
	LIBV_REFLECTION_ACCESS(texture0);
	LIBV_REFLECTION_ACCESS(boneID);
	LIBV_REFLECTION_ACCESS(boneWieght);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct Mesh {
	std::string name;
	uint32_t baseIndex;
	uint32_t baseVertex;
	uint32_t numIndices;
	uint32_t materialID;
	//bounding box ?

	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(baseIndex);
	LIBV_REFLECTION_ACCESS(baseVertex);
	LIBV_REFLECTION_ACCESS(numIndices);
	LIBV_REFLECTION_ACCESS(materialID);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct Node {
	//bounding box ?
	std::string name;
	NodeID parentID;
	//bool visibility;
	libv::mat4f transformation;
	std::vector<MeshID> meshIDs;
	std::vector<NodeID> childrenIDs;

	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(parentID);
	LIBV_REFLECTION_ACCESS(transformation);
	LIBV_REFLECTION_ACCESS(meshIDs);
	LIBV_REFLECTION_ACCESS(childrenIDs);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct LOD {
	double rangeNear; //threshold for increasing LOD
	double rangeFar; //threshold for decreasing LOD
	NodeID rootNodeID;

	LIBV_REFLECTION_ACCESS(rangeNear);
	LIBV_REFLECTION_ACCESS(rangeFar);
	LIBV_REFLECTION_ACCESS(rootNodeID);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct Model {
	int32_t format = 20191201;

	std::string name;
	int32_t version = 0;

	libv::vec3f AABB_min;
	libv::vec3f AABB_max;
	libv::vec3f BS_origin; // Bounding Sphere Origin
	float BS_radius; // Bounding Sphere Radius

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
//	std::vector<libv::vec4f> vertex_boneWieghts;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	LIBV_REFLECTION_ACCESS(format);
	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(version);

	LIBV_REFLECTION_ACCESS(AABB_min);
	LIBV_REFLECTION_ACCESS(AABB_max);
	LIBV_REFLECTION_ACCESS(BS_origin);
	LIBV_REFLECTION_ACCESS(BS_radius);

	LIBV_REFLECTION_ACCESS(animations);
	LIBV_REFLECTION_ACCESS(animationChannels);
	LIBV_REFLECTION_ACCESS(materials);
	LIBV_REFLECTION_ACCESS(meshes);
	LIBV_REFLECTION_ACCESS(nodes);
	LIBV_REFLECTION_ACCESS(lods);
//	LIBV_REFLECTION_ACCESS(vertex_positions);
//	LIBV_REFLECTION_ACCESS(vertex_normals);
//	LIBV_REFLECTION_ACCESS(vertex_tangents);
//	LIBV_REFLECTION_ACCESS(vertex_bitangents);
//	LIBV_REFLECTION_ACCESS(vertex_texCoord0s);
//	LIBV_REFLECTION_ACCESS(vertex_boneIDs);
//	LIBV_REFLECTION_ACCESS(vertex_boneWieghts);
	LIBV_REFLECTION_ACCESS(vertices);
	LIBV_REFLECTION_ACCESS(indices);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
