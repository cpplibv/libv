// File: Importer.hpp, Created on 2015. január 17. 7:33, Author: Vader

#pragma once

#include <string>

class aiScene;
class aiNode;

namespace vl {
	namespace vm3 {
		class Model;
		class Node;

		class Importer {
		private:
			static void importMaterials(Model& model, const aiScene* scene);
			static void importMeshes(Model& model, const aiScene* scene);
			static void importNodes(Model& model, const aiScene* scene);
			static void importNodes(Model& model, const aiScene* scene, uint32_t nodeID, const aiNode* nd);
			static void importLod(Model& model, const aiScene* scene);
			static void importGeometry(Model& model, const aiScene* scene);
		public:
			static bool import(Model& model, const std::string& filePath);
		};
	} //namespace vm3
} //namespace vl