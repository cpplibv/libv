// File: importer.cpp - Created on 2017.10.01. 03:09 - Author: Vader

// hpp
#include <libv/vm4imp/importer.hpp>
// ext
#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <fmt/format.h>
// libv
#include <libv/math/vec.hpp>
// std
#include <vector>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4imp/log.hpp>


namespace libv {
namespace vm4 {
namespace {

// -------------------------------------------------------------------------------------------------

struct ImportContext {
	Model& model;
	const aiScene* scene;

private:
	boost::container::flat_set<NodeID> flippedCWnodes;
	boost::container::flat_map<MeshID, MeshID> flippedCWmeshes;

public:
	ImportContext(Model& model, const aiScene* scene) : model(model), scene(scene) { }

public:
	const char* to_libv_technique(int tech) {
		switch (tech) {
		case aiShadingMode_Flat: return "flat";
		case aiShadingMode_Gouraud: return "gouraud";
		case aiShadingMode_Phong: return "phong";
		case aiShadingMode_Blinn: return "blinn";
		case aiShadingMode_Toon: return "toon";
		case aiShadingMode_OrenNayar: return "orennayar";
		case aiShadingMode_Minnaert: return "minnaert";
		case aiShadingMode_CookTorrance: return "cooktorrance";
		case aiShadingMode_Fresnel: return "fresnel";
		case aiShadingMode_NoShading: return "noshading";
		}
		return "noshading";
	}

	void importMaterials() {
		//http://assimp.sourceforge.net/lib_html/materials.html

		auto& materials = model.materials;
		materials.resize(scene->mNumMaterials);

		for (uint32_t i = 0; i < scene->mNumMaterials; ++i) {
			aiString ai_str;
			float ai_float;
			int ai_int = 0;
			aiVector2D ai_vec2;
			aiVector3D ai_vec3;
			aiColor3D ai_col3;
			aiColor4D ai_col4;

			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_NAME, ai_str))
				materials[i].name = ai_str.C_Str();

			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, ai_col4))
				materials[i].properties.emplace("color_diffuse", libv::vec::from_rgba(ai_col4));
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, ai_col4))
				materials[i].properties.emplace("color_specular", libv::vec::from_rgba(ai_col4));
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ai_col4))
				materials[i].properties.emplace("color_ambient", libv::vec::from_rgba(ai_col4));
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, ai_col4))
				materials[i].properties.emplace("color_emissive", libv::vec::from_rgba(ai_col4));
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_REFLECTIVE, ai_col4))
				materials[i].properties.emplace("color_reflective", libv::vec::from_rgba(ai_col4));

			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_ENABLE_WIREFRAME, ai_int))
				materials[i].properties.emplace("wireframe", ai_int != 0);
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TWOSIDED, ai_int))
				materials[i].properties.emplace("twosided", ai_int != 0);
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_SHADING_MODEL, ai_int))
				materials[i].shader = to_libv_technique(ai_int);
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_BLEND_FUNC, ai_int))
				materials[i].properties.emplace("blend_func", ai_int);

			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_OPACITY, ai_float))
				materials[i].properties.emplace("opacity", ai_float);
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, ai_float))
				materials[i].properties.emplace("shininess", ai_float);
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, ai_float))
				materials[i].properties.emplace("shininess_strength", ai_float);
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_REFRACTI, ai_float))
				materials[i].properties.emplace("refraction", ai_float);

			const auto loadTexture = [&](const auto& stack, const auto& name) {
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(stack, n), ai_str); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_path", name, n), std::string(ai_str.C_Str()));
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXBLEND(stack, n), ai_float); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_blend", name, n), ai_float);
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXOP(stack, n), ai_int); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_texop", name, n), ai_int);
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_MAPPING(stack, n), ai_int); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_mapping", name, n), ai_int);
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_UVWSRC(stack, n), ai_int); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_uvwsrc", name, n), ai_int);
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_MAPPINGMODE_U(stack, n), ai_int); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_mappingmode_u", name, n), ai_int);
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_MAPPINGMODE_V(stack, n), ai_int); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_mappingmode_v", name, n), ai_int);
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXMAP_AXIS(stack, n), ai_vec3); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_texmap_axis", name, n), vec3f(ai_vec3));
				for (uint32_t n = 0; AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXMAP_AXIS(stack, n), ai_int); ++n)
					materials[i].properties.emplace(fmt::format("texture_{}_{:02}_texflags", name, n), ai_int);
			};

			loadTexture(aiTextureType_NONE, "none");
			loadTexture(aiTextureType_DIFFUSE, "diffuse");
			loadTexture(aiTextureType_SPECULAR, "specular");
			loadTexture(aiTextureType_AMBIENT, "ambient");
			loadTexture(aiTextureType_EMISSIVE, "emissive");
			loadTexture(aiTextureType_HEIGHT, "height");
			loadTexture(aiTextureType_NORMALS, "normals");
			loadTexture(aiTextureType_SHININESS, "shininess");
			loadTexture(aiTextureType_OPACITY, "opacity");
			loadTexture(aiTextureType_DISPLACEMENT, "displacement");
			loadTexture(aiTextureType_LIGHTMAP, "lightmap"); // Ambient occlusion
			loadTexture(aiTextureType_REFLECTION, "reflection");
			loadTexture(aiTextureType_UNKNOWN, "unknown");
		}
	}

	void importMeshes() {
		auto& meshes = model.meshes;
		meshes.resize(scene->mNumMeshes);

		uint32_t numVertices = 0;
		uint32_t numIndices = 0;

		for (uint32_t i = 0; i < scene->mNumMeshes; ++i) {
			meshes[i].name = scene->mMeshes[i]->mName.C_Str();
			meshes[i].baseIndex = numIndices;
			meshes[i].baseVertex = numVertices;
			meshes[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;

			meshes[i].materialID = scene->mMeshes[i]->mMaterialIndex;

			numVertices += scene->mMeshes[i]->mNumVertices;
			numIndices += meshes[i].numIndices;
		}

		model.vertices.reserve(numVertices);
		model.indices.reserve(numIndices);
	}

	void importNodes(uint32_t nodeID, uint32_t parentID, const aiNode* aiNode, libv::mat4f parentMat) {
		aiMatrix4x4 m = aiNode->mTransformation;
		aiTransposeMatrix4(&m);
		const auto nodeMat = libv::mat4f(
				m.a1, m.a2, m.a3, m.a4,
				m.b1, m.b2, m.b3, m.b4,
				m.c1, m.c2, m.c3, m.c4,
				m.d1, m.d2, m.d3, m.d4);

		const auto globalMat = parentMat * nodeMat;
		// If the determinant of the upper-left 3x3 part of the transformation matrix is negative: The winding order is flipped.
		const bool isCW = libv::mat3f(globalMat).determinant() < 0;

		if (isCW)
			flippedCWnodes.emplace(nodeID);

		model.nodes[nodeID].parentID = parentID;
		model.nodes[nodeID].name = aiNode->mName.C_Str();
		model.nodes[nodeID].transformation = nodeMat;

		for (uint32_t i = 0; i < aiNode->mNumMeshes; i++) {
			model.nodes[nodeID].meshIDs.push_back(aiNode->mMeshes[i]);
			if (isCW)
				flippedCWmeshes.emplace(aiNode->mMeshes[i], 0);
		}

		for (uint32_t i = 0; i < aiNode->mNumChildren; i++) {
			uint32_t childNodeID = static_cast<uint32_t>(model.nodes.size());
			model.nodes.emplace_back();
			model.nodes[nodeID].childrenIDs.push_back(childNodeID);
			importNodes(childNodeID, nodeID, aiNode->mChildren[i], globalMat);
		}
	}

	void importNodes() {
		model.nodes.emplace_back();
		importNodes(0, 0, scene->mRootNode, libv::mat4f::identity());
	}

	void importLod() {
		model.lods.resize(1);
		model.lods[0].rootNodeID = 0;
		model.lods[0].rangeNear = 0;
		model.lods[0].rangeFar = 0;
	}

	void importGeometry() {
		for (uint32_t i = 0; i < model.meshes.size(); i++) {
			const aiMesh* mesh = scene->mMeshes[i];

			for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
				auto& vertex = model.vertices.emplace_back();

				vertex.position = libv::vec3f(mesh->mVertices[j]);
				if (mesh->HasNormals())
					vertex.normal = libv::vec3f(mesh->mNormals[j]);
				if (mesh->HasTangentsAndBitangents()) {
					vertex.tangent = libv::vec3f(mesh->mTangents[j]);
					vertex.bitangent = libv::vec3f(mesh->mBitangents[j]);
				}
				if (mesh->HasTextureCoords(0))
					vertex.texture0 = libv::vec::xy(mesh->mTextureCoords[0][j]);

				if (mesh->HasBones()) {
					//vertex.boneID = vec4i();
					//vertex.boneWieght = vec4();
				}
			}

			for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
				const aiFace& face = mesh->mFaces[j];
				if (face.mNumIndices != 3) {
					log_vm4.warn("Illegal number of vertex {} in face {} of a mesh: {}. Skipping face", face.mNumIndices, j, mesh->mName.C_Str());
					continue;
				}
				model.indices.push_back(face.mIndices[0]);
				model.indices.push_back(face.mIndices[1]);
				model.indices.push_back(face.mIndices[2]);
			}
		}
	}

	// -------------------------------------------------------------------------------------------------

	void calculateAABB() {
		// TODO P5: libv.vm4: include AABB for each node(?) / LOD(?) / mesh(?) / model

		//void get_aabb_for_node(const struct aiNode* nd,
		//		aiVector3D* min,
		//		aiVector3D* max,
		//		aiMatrix4x4* trafo) {
		//	aiMatrix4x4 prev;
		//	uint32_t n = 0, t;
		//	prev = *trafo;
		//	aiMultiplyMatrix4(trafo, &nd->mTransformation);
		//	for (; n < nd->mNumMeshes; ++n) {
		//		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		//		for (t = 0; t < mesh->mNumVertices; ++t) {
		//			aiVector3D tmp = mesh->mVertices[t];
		//			aiTransformVecByMatrix4(&tmp, trafo);
		//			min->x = aisgl_min(min->x, tmp.x);
		//			min->y = aisgl_min(min->y, tmp.y);
		//			min->z = aisgl_min(min->z, tmp.z);
		//			max->x = aisgl_max(max->x, tmp.x);
		//			max->y = aisgl_max(max->y, tmp.y);
		//			max->z = aisgl_max(max->z, tmp.z);
		//		}
		//	}
		//	for (n = 0; n < nd->mNumChildren; ++n) {
		//		get_aabb_for_node(nd->mChildren[n], min, max, trafo);
		//	}
		//	*trafo = prev;
		//}
		//
		//void get_aabb(aiVector3D* min, aiVector3D* max) {
		//	aiMatrix4x4 trafo;
		//	aiIdentityMatrix4(&trafo);
		//	min->x = min->y = min->z = std::numeric_limits<float4>::max();
		//	max->x = max->y = max->z = std::numeric_limits<float4>::min();
		//	get_aabb_for_node(scene->mRootNode, min, max, &trafo);
		//}
	}

	void calculateBoundingSphere() {
		// TODO P5: libv.vm4: include BSO and BSR for each node(?) / LOD(?) / mesh(?) / model

		//	// Find (BSO) Bounding sphere origin and (BSR) Bounding sphere radius
		//	const auto& node = scene.model.nodes[0];
		//
		//	auto referencePointW = libv::vec3f{};
		//	auto referenceDistanceWSQ = 0.f;
		//
		//	for (const auto& meshID : node.meshIDs) {
		//		const auto& mesh = scene.model.meshes[meshID];
		//		const auto indexBegin = mesh.baseIndex;
		//		const auto indexEnd = mesh.baseIndex + mesh.numIndices;
		//
		//		for (size_t index = indexBegin; index < indexEnd; ++index) {
		//			const auto vertexID = scene.model.indices[index] + mesh.baseVertex;
		//			const auto vertexPositionM = scene.model.vertices[vertexID].position;
		//			const auto vertexPositionW = parentTransformation * node.transformation * vertexPositionM;
		//
		//			const auto vertexDistanceWSQ = (vertexPositionW - referencePointW).lengthSQ();
		//			if (vertexDistanceWSQ > referenceDistanceWSQ) {
		//				referencePointW
		//
		//			}
		//		}
		//	}
		//
		//	for (const auto& childID : node.childrenIDs)
		//		referencePoint = findFarestVertex(scene.model.nodes[childID], referencePoint);
		//
		////	for (const auto& node : scene.model.nodes)
		////	for (const auto& node : scene.model.nodes)
	}

	// -------------------------------------------------------------------------------------------------

	void fixupFlippedWindingOrder() {
		for (auto& [meshID, fixedMeshID] : flippedCWmeshes) {
			Mesh& mesh = model.meshes.emplace_back(model.meshes[meshID]);

			fixedMeshID = static_cast<MeshID>(model.meshes.size() - 1);
			const auto fixedStart = static_cast<uint32_t>(model.indices.size());

			model.indices.resize(model.indices.size() + mesh.numIndices);

			for (size_t i = 0; i < mesh.numIndices; i += 3) {
				model.indices[fixedStart + i + 0] = model.indices[mesh.baseIndex + i + 2];
				model.indices[fixedStart + i + 1] = model.indices[mesh.baseIndex + i + 1];
				model.indices[fixedStart + i + 2] = model.indices[mesh.baseIndex + i + 0];
			}

			mesh.name += "@CWFix";
			mesh.baseIndex = fixedStart;

			log_vm4.warn("Fixing winding order for Mesh {} {} start {} count {} to {} {} start {} count {}",
					meshID, model.meshes[meshID].name,
					model.meshes[meshID].baseIndex, model.meshes[meshID].numIndices,
					fixedMeshID, mesh.name,
					fixedStart, mesh.numIndices);
		}

		for (const auto& nodeID : flippedCWnodes) {
			for (auto& meshID : model.nodes.at(nodeID).meshIDs)
				meshID = flippedCWmeshes.at(meshID);

			log_vm4.warn("Fixing winding order for Node {} {}", nodeID, model.nodes[nodeID].name);
		}
	}
};

} // namespace -------------------------------------------------------------------------------------

//template <typename F>
//struct ProgressHandler : Assimp::ProgressHandler {
//	F callback;
//
//	virtual bool Update(float percentage = -1.f) override {
//		callback(percentage);
//		return true;
//	}
//};
//
//template <typename F>
//void setProgressCallback(F&& func) {
//	Assimp::Importer importer;
//
//	ProgressHandler ph{std::forward<F>(func)};
//
//	importer.SetProgressHandler();
//}

//	using namespace Assimp;
//	// Create a logger instance
//	DefaultLogger::create("", Logger::VERBOSE);
//	// Now I am ready for logging my stuff
//	DefaultLogger::get()->info("this is my info-call");
//	// Kill it after the work is done
//	DefaultLogger::kill();



void scanNode(uint32_t depth, const aiNode* aiNode) {
	fmt::print("{:{}}Node {} num_mesh: {} num_child: {}\n", "", depth * 4, aiNode->mName.C_Str(), aiNode->mNumMeshes, aiNode->mNumChildren);

	for (uint32_t i = 0; i < aiNode->mNumMeshes; i++) {
		fmt::print("{:{}}Mesh {}\n", "", depth * 4, aiNode->mMeshes[i]);
	}

	for (uint32_t i = 0; i < aiNode->mNumChildren; i++) {
		scanNode(depth + 1, aiNode->mChildren[i]);
	}
}

void scanNode(uint32_t depth, const Model& model, uint32_t nodeID) {
	fmt::print("{:{}}Node {} num_mesh: {} num_child: {}\n", "", depth * 4, model.nodes[nodeID].name, model.nodes[nodeID].meshIDs.size(), model.nodes[nodeID].childrenIDs.size());

	for (const auto& meshID : model.nodes[nodeID].meshIDs) {
		fmt::print("{:{}}Mesh {} {}\n", "", depth * 4, meshID, model.meshes[meshID].name);
	}

	for (const auto& childID : model.nodes[nodeID].childrenIDs) {
		scanNode(depth + 1, model, childID);
	}
}

std::optional<Model> import(const std::string& filePath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, 0
		| aiProcess_CalcTangentSpace
		| aiProcess_GenNormals
		| aiProcess_GenUVCoords
		| aiProcess_LimitBoneWeights
		| aiProcess_Triangulate

		| aiProcess_SortByPType
//		| aiProcess_SplitLargeMeshes

		| aiProcess_FindInstances
		| aiProcess_ImproveCacheLocality
		| aiProcess_JoinIdenticalVertices
		| aiProcess_OptimizeGraph
		| aiProcess_OptimizeMeshes
		| aiProcess_RemoveRedundantMaterials

		| aiProcess_FindDegenerates
		| aiProcess_FindInvalidData
		| aiProcess_ValidateDataStructure

//		| aiProcess_PreTransformVertices
//		| aiProcess_GlobalScale

	);

	if (!scene) {
		log_vm4.error("Failed to import model {}: {}", filePath, importer.GetErrorString());
		return std::nullopt;
	}

	auto model = std::optional<Model>{std::in_place};
	ImportContext ctx{*model, scene};

	model->name = scene->mRootNode->mName.C_Str();
	ctx.importMaterials();
	ctx.importMeshes();
	ctx.importNodes();
	ctx.importLod();
	ctx.importGeometry();

	ctx.calculateAABB();
	ctx.calculateBoundingSphere();

	// --- Post-process ---
	// TODO P4: libv.vm4: Prompt / importer setting for unit scale factor
	// TODO P4: libv.vm4: Handle multiple LOD case for unit scale factor
	model->nodes[0].transformation.scale(libv::vec3f(100.f, 100.f, 100.f));
	// TODO P4: libv.vm4: Prompt / importer setting for winding order fix
	ctx.fixupFlippedWindingOrder();
	// TODO P4: libv.vm4: Prompt / importer setting for aiProcess_OptimizeGraph

//	fmt::print("=== Scan assimp =====================================================================\n");
//	scanNode(0, scene->mRootNode);
//	fmt::print("=== Scan vm4 ========================================================================\n");
//	scanNode(0, *model, 0);

	return model;
}

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
