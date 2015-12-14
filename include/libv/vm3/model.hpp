// File: Model.hpp, Created on 2015. január 17. 7:21, Author: Vader

#pragma once

// ext
#include <glm/glm.hpp>
// std
#include <vector>
// pro
#include <libv/vm3/material.hpp>

//TODO P2: Use dynarray instead of vector
//TODO P2: Use pointers instead of IDs
//TODO P5: drop boost serialization for vm3

namespace libv {
namespace vm3 {

struct Animation {
};

struct AnimationChannel {
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec2 texCoord0;
	glm::ivec4 boneID;
	glm::vec4 boneWieght;
};

struct Mesh {
	std::string name;
	uint32_t baseIndex;
	uint32_t baseVertex;
	uint32_t numIndices;
	uint32_t materialID;
	//bounding box ?
};

struct Node {
	//bounding box ?
	std::string name;
	uint32_t parentID;
	//bool visibility;
	glm::mat4 transformation;
	std::vector<uint32_t> meshIDs;
	std::vector<uint32_t> childrenIDs;
};

struct LOD {
	double rangeNear; //threshold for increasing LOD
	double rangeFar; //threshold for decreasing LOD
	uint32_t rootNodeID;
};

struct ModelSerialization;

class Model {
	friend class Importer;
	friend class ModelSerialization;

private:
	std::string name;
public:
	std::vector<Animation> animations;
	std::vector<AnimationChannel> animationChannels;
public:
	std::vector<Material> materials;
	std::vector<Mesh> meshes;
	std::vector<Node> nodes;
public:
	std::vector<LOD> lods;
public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	// -------------------------------------------------------------------------------------
public:
	std::string getName() const {
		return name;
	}

	void update(float deltaTime);

public:
	Model();

	bool load(const char* data, const size_t size);

	bool save(std::ostream& file) const;
	bool save(std::ostream&& file) const;

	void addMaterial(const Material& mat) {
		materials.emplace_back(mat);
	}
	virtual ~Model() { }
};
} //namespace vm3
} //namespace libv
