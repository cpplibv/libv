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
	libv::vec2f texCoord0;
	libv::vec4i boneID;
	libv::vec4f boneWieght;

	LIBV_REFLECTION_ACCESS(position);
	LIBV_REFLECTION_ACCESS(normal);
	LIBV_REFLECTION_ACCESS(tangent);
	LIBV_REFLECTION_ACCESS(bitangent);
	LIBV_REFLECTION_ACCESS(texCoord0);
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
	uint32_t parentID;
	//bool visibility;
	libv::mat4f transformation;
	std::vector<uint32_t> meshIDs;
	std::vector<uint32_t> childrenIDs;

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
	uint32_t rootNodeID;

	LIBV_REFLECTION_ACCESS(rangeNear);
	LIBV_REFLECTION_ACCESS(rangeFar);
	LIBV_REFLECTION_ACCESS(rootNodeID);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct Model {
	std::string name;

	std::vector<Animation> animations;
	std::vector<AnimationChannel> animationChannels;

	std::vector<Material> materials;
	std::vector<Mesh> meshes;
	std::vector<Node> nodes;

	std::vector<LOD> lods;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(animations);
	LIBV_REFLECTION_ACCESS(animationChannels);
	LIBV_REFLECTION_ACCESS(materials);
	LIBV_REFLECTION_ACCESS(meshes);
	LIBV_REFLECTION_ACCESS(nodes);
	LIBV_REFLECTION_ACCESS(lods);
	LIBV_REFLECTION_ACCESS(vertices);
	LIBV_REFLECTION_ACCESS(indices);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

//struct ImportResult {
//	bool error_code;
//	Model model;
//};

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv