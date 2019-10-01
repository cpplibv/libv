// File: importer.cpp - Created on 2017.10.01. 03:09 - Author: Vader

// hpp
#include <libv/vm4imp/importer.hpp>
// ext
#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
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

// void importMaterials(Model& model, const aiScene* scene);
// void importMeshes(Model& model, const aiScene* scene);
// void importNodes(Model& model, const aiScene* scene);
// void importNodes(Model& model, const aiScene* scene, uint32_t nodeID, const aiNode* nd);
// void importLod(Model& model, const aiScene* scene);
// void importGeometry(Model& model, const aiScene* scene);

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

void importMaterials(Model& model, const aiScene* scene) {
	//http://assimp.sourceforge.net/lib_html/materials.html

	auto& materials = model.materials;
	materials.resize(scene->mNumMaterials);

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
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

void importMeshes(Model& model, const aiScene* scene) {
	auto& meshes = model.meshes;
	meshes.resize(scene->mNumMeshes);

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
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

void importNodes(Model& model, const aiScene* scene, uint32_t nodeID, const aiNode* aiNode) {
	model.nodes[nodeID].parentID = 0;
	model.nodes[nodeID].name = aiNode->mName.C_Str();
	aiMatrix4x4 m = aiNode->mTransformation;
	aiTransposeMatrix4(&m);
	model.nodes[nodeID].transformation = libv::mat4f(
			m.a1, m.a2, m.a3, m.a4,
			m.b1, m.b2, m.b3, m.b4,
			m.c1, m.c2, m.c3, m.c4,
			m.d1, m.d2, m.d3, m.d4);

	for (unsigned int i = 0; i < aiNode->mNumMeshes; i++) {
		model.nodes[nodeID].meshIDs.push_back(aiNode->mMeshes[i]);
	}

	for (unsigned int i = 0; i < aiNode->mNumChildren; i++) {
		uint32_t childNodeID = static_cast<uint32_t>(model.nodes.size() - 1);

		model.nodes.emplace_back();
		model.nodes[model.nodes.size() - 1].parentID = nodeID;
		model.nodes[nodeID].childrenIDs.push_back(childNodeID);
		importNodes(model, scene, childNodeID, aiNode->mChildren[i]);
	}
}

void importNodes(Model& model, const aiScene* scene) {
	model.nodes.resize(1);
	importNodes(model, scene, 0, scene->mRootNode);

	// TODO P4: Prompt for Unit scale factor
	model.nodes[0].transformation.scale(libv::vec3f(100.f, 100.f, 100.f));
}

void importLod(Model& model, const aiScene*) {
	model.lods.resize(1);
	model.lods[0].rootNodeID = 0;
	model.lods[0].rangeNear = 0;
	model.lods[0].rangeFar = 0;
}

void importGeometry(Model& model, const aiScene* scene) {
	for (unsigned int i = 0; i < model.meshes.size(); i++) {
		const aiMesh* mesh = scene->mMeshes[i];

		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			auto& vertex = model.vertices.emplace_back();

			vertex.position = libv::vec3f(mesh->mVertices[j]);
			if (mesh->HasNormals())
				vertex.normal = libv::vec3f(mesh->mNormals[j]);
			if (mesh->HasTangentsAndBitangents()) {
				vertex.tangent = libv::vec3f(mesh->mTangents[j]);
				vertex.bitangent = libv::vec3f(mesh->mBitangents[j]);
			}
			if (mesh->HasTextureCoords(0))
				vertex.texCoord0 = libv::vec::xy(mesh->mTextureCoords[0][j]);

			if (mesh->HasBones()) {
				//vertex.boneID = vec4i();
				//vertex.boneWieght = vec4();
			}
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			const aiFace& face = mesh->mFaces[j];
			if (face.mNumIndices != 3) {
				log_vm4.warn("Illegal number of vertex {} in a face of a mesh: {}", face.mNumIndices, mesh->mName.C_Str());
				continue;
			}
			model.indices.push_back(face.mIndices[0]);
			model.indices.push_back(face.mIndices[1]);
			model.indices.push_back(face.mIndices[2]);
		}
	}
}
} // namespace -------------------------------------------------------------------------------------

// TODO P2: flag for adding opposite version on a mesh indices if it is oddly mirrored

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

std::optional<Model> import(const std::string& filePath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene) {
		log_vm4.error("Failed to import model {}: {}", filePath, importer.GetErrorString());
		return {};
	}

	auto model = std::optional<Model>{std::in_place};

	model->name = scene->mRootNode->mName.C_Str();
	importMaterials(*model, scene);
	importMeshes(*model, scene);
	importNodes(*model, scene);
	importLod(*model, scene);
	importGeometry(*model, scene);

	return model;
}

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv





//void get_aabb_for_node(const struct aiNode* nd,
//		aiVector3D* min,
//		aiVector3D* max,
//		aiMatrix4x4* trafo) {
//	aiMatrix4x4 prev;
//	unsigned int n = 0, t;
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
