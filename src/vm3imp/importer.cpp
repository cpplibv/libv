// File: Importer.cpp, Created on 2015. február 18. 20:08, Author: Vader

// hpp
#include <libv/vm3/importer.hpp>
// ext
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
// libv
#include <libv/math/vec.hpp>
// std
#include <vector>
// pro
#include <libv/vm3/model.hpp>
#include <libv/vm3/log.hpp>

namespace libv {
namespace vm3 {

std::string to_libv_technique(int tech) {
	switch (tech) {
	case aiShadingMode_Flat: return "flat";
	case aiShadingMode_Gouraud: return "gouraud";
		//We should explode in any case if a bullshit shader is coming and ask for guidance
		//(outdated:) Gouraud is rarely used to redirect it to lambert as the default shading mode
	case aiShadingMode_Phong: return "phong";
	case aiShadingMode_Blinn: return "blinn";
	case aiShadingMode_Toon: return "toon";
	case aiShadingMode_OrenNayar: return "orennayar";
	case aiShadingMode_Minnaert: return "minnaert";
	case aiShadingMode_CookTorrance: return "cooktorrance";
	case aiShadingMode_Fresnel: return "fresnel";
	default: case aiShadingMode_NoShading: return "noshading";
	}
}

void Importer::importMaterials(Model& model, const aiScene* scene) {
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
			materials[i].setName(std::string(ai_str.C_Str()));
		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_SHADING_MODEL, ai_int))
			materials[i].setShader(to_libv_technique(ai_int));

		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, ai_col4))
			materials[i].set("color_diffuse", libv::vec4f(ai_col4.r, ai_col4.g, ai_col4.b, ai_col4.a));
		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, ai_col4))
			materials[i].set("color_specular", libv::vec4f(ai_col4.r, ai_col4.g, ai_col4.b, ai_col4.a));
		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ai_col4))
			materials[i].set("color_ambient", libv::vec4f(ai_col4.r, ai_col4.g, ai_col4.b, ai_col4.a));
		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, ai_col4))
			materials[i].set("color_emissive", libv::vec4f(ai_col4.r, ai_col4.g, ai_col4.b, ai_col4.a));
		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_COLOR_REFLECTIVE, ai_col4))
			materials[i].set("color_reflective", libv::vec4f(ai_col4.r, ai_col4.g, ai_col4.b, ai_col4.a));

		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, ai_float))
			materials[i].set("shininess", ai_float);
		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, ai_float))
			materials[i].set("shininess_strength", ai_float);

		//http://assimp.sourceforge.net/lib_html/material_8h.html#a93e23e0201d6ed86fb4287e15218e4cf

		//http://assimp.sourceforge.net/lib_html/materials.html

		//WIREFRAME
		//TWOSIDED

		//BLEND_FUNC
		//OPACITY
		//REFRACTI
		//TEXTURE(t,n)
		//TEXBLEND(t,n)
		//TEXOP(t,n)
		//MAPPING(t,n)
		//UVWSRC(t,n)
		//MAPPINGMODE_U(t,n)
		//MAPPINGMODE_V(t,n)
		//TEXMAP_AXIS(t,n)
		//TEXFLAGS(t,n)
	}
}

void Importer::importMeshes(Model& model, const aiScene* scene) {
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

void Importer::importNodes(Model& model, const aiScene* scene) {
	model.nodes.resize(1);
	importNodes(model, scene, 0, scene->mRootNode);

	// TODO P4: Prompt for Unit scale factor
	model.nodes[0].transformation.scale(libv::vec3f(100.f, 100.f, 100.f));
}

void Importer::importNodes(Model& model, const aiScene* scene, uint32_t nodeID, const aiNode* aiNode) {
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

void Importer::importLod(Model& model, const aiScene*) {
	model.lods.resize(1);
	model.lods[0].rootNodeID = 0;
	model.lods[0].rangeNear = 0;
	model.lods[0].rangeFar = 0;
}

void Importer::importGeometry(Model& model, const aiScene* scene) {
	for (unsigned int i = 0; i < model.meshes.size(); i++) {
		const aiMesh* mesh = scene->mMeshes[i];
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			const aiVector3D* pPos = &(mesh->mVertices[j]);
			const aiVector3D* pNormal = &(mesh->mNormals[j]);
			const aiVector3D* pTangent = &(mesh->mTangents[j]);
			const aiVector3D* pBitangent = &(mesh->mBitangents[j]);
			const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][j]) : &Zero3D;

			Vertex vertex;
			vertex.position = libv::vec3f(pPos->x, pPos->y, pPos->z);
			vertex.normal = libv::vec3f(pNormal->x, pNormal->y, pNormal->z);
			vertex.tangent = libv::vec3f(pTangent->x, pTangent->y, pTangent->z);
			vertex.bitangent = libv::vec3f(pBitangent->x, pBitangent->y, pBitangent->z);
			vertex.texCoord0 = libv::vec2f(pTexCoord->x, pTexCoord->y);
			//vertex.boneID = vec4i();
			//vertex.boneWieght = vec4();

			model.vertices.push_back(vertex);
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			const aiFace& face = mesh->mFaces[j];
			if (face.mNumIndices != 3) {
				LIBV_VM3_WARN("Illegal number of vertex (%d) in a face of a mesh: %s", face.mNumIndices, mesh->mName.C_Str());
				continue;
			}
			model.indices.push_back(face.mIndices[0]);
			model.indices.push_back(face.mIndices[1]);
			model.indices.push_back(face.mIndices[2]);
		}
	}
}

// TODO P2: flag for adding opposite version on a mesh indices if it is oddly mirrored

bool Importer::import(Model& model, const std::string& filePath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs);

	if (!scene) {
		LIBV_VM3_ERROR("Failed to import model [%s]: %s", filePath, importer.GetErrorString());
		return false;
	}

	model.name = scene->mRootNode->mName.C_Str();
	importMaterials(model, scene);
	importMeshes(model, scene);
	importNodes(model, scene);
	importLod(model, scene);
	importGeometry(model, scene);

	return true;
}

} // namespace vm3
} // namespace libv




//void get_bounding_box_for_node(const struct aiNode* nd,
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
//		get_bounding_box_for_node(nd->mChildren[n], min, max, trafo);
//	}
//	*trafo = prev;
//}
//
///* ---------------------------------------------------------------------------- */
//void get_bounding_box(aiVector3D* min, aiVector3D* max) {
//	aiMatrix4x4 trafo;
//	aiIdentityMatrix4(&trafo);
//	min->x = min->y = min->z = 1e10f;
//	max->x = max->y = max->z = -1e10f;
//	get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
//}


//for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
//	aiString Path;
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_AMBIENT, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_AMBIENT Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_DIFFUSE Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_DISPLACEMENT, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_DISPLACEMENT Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_EMISSIVE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_EMISSIVE Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_HEIGHT, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_HEIGHT Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_LIGHTMAP, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_LIGHTMAP Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_NONE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_NONE Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_NORMALS Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_OPACITY, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_OPACITY Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_REFLECTION, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_REFLECTION Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_SHININESS, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_SHININESS Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_SPECULAR Texture: %s", Path.data);
//	if (scene->mMaterials[i]->GetTexture(aiTextureType_UNKNOWN, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
//		LIBV_LOG_GL_TRACE("aiTextureType_UNKNOWN Texture: %s", Path.data);
//}