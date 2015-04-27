// File: model.cpp, Created on 2014. november 28. 17:45, Author: Vader

// hpp
#include "vl/gl/detail/model_impl.hpp"
// ext
#include <GL/glew.h>
// vl
#include <vl/file_system.hpp>
#include <vl/vm3/model.hpp>
// std
#include <fstream>
// pro
#include "vl/gl/model.hpp"
#include "vl/gl/gl_contexts.hpp"
#include "vl/gl/shader.hpp"
#include "vl/gl/uniform.hpp"
#include "vl/gl/vgl.hpp"

namespace vl {
namespace gl {
namespace detail {
// -----------------------------------------------------------------

bool ModelImpl::loadIO() {
	model.load(std::ifstream{std::string("Data/Model/") + name + ".vm3", std::ios::binary});

	VLOG_TRACE(vl::gl::log(), "IO Loading model: [%s]", name);
	return true;
}

bool ModelImpl::loadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Loading model: [%s]", name);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_index);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof (vm3::Vertex) * model.vertices.size(), model.vertices.data(), GL_STATIC_DRAW);
	checkGL();

	glEnableVertexAttribArray(ATTRIBUTE_POSITION);
	glVertexAttribPointer(ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, position));
	glEnableVertexAttribArray(ATTRIBUTE_NORMAL);
	glVertexAttribPointer(ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, normal));
	glEnableVertexAttribArray(ATTRIBUTE_TANGENT);
	glVertexAttribPointer(ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, tangent));
	glEnableVertexAttribArray(ATTRIBUTE_BITANGENT);
	glVertexAttribPointer(ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, bitangent));
	glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD0);
	glVertexAttribPointer(ATTRIBUTE_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, texCoord0));
	glEnableVertexAttribArray(ATTRIBUTE_BONEINDICES);
	glVertexAttribIPointer(ATTRIBUTE_BONEINDICES, 4, GL_INT, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, boneID));
	glEnableVertexAttribArray(ATTRIBUTE_BONEWEIGHT);
	glVertexAttribPointer(ATTRIBUTE_BONEWEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, boneWieght));
	checkGL();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (model.indices[0]) * model.indices.size(), model.indices.data(), GL_STATIC_DRAW);
	checkGL();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	checkGL();
	return true;
}

bool ModelImpl::unloadIO() {
	VLOG_TRACE(vl::gl::log(), "IO Unloading model: [%s]", name);
	return true;
}

bool ModelImpl::unloadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Unloading model: [%s]", name);

	glDeleteBuffers(1, &vbo_index);
	glDeleteBuffers(1, &vbo_vertex);
	glDeleteVertexArrays(1, &vao);

	return true;
}

void ModelImpl::render() {
	glBindVertexArray(vao);
	checkGL();

	renderNode(model.lods[0].rootNodeID);

	glBindVertexArray(0);
	checkGL();
}

void ModelImpl::renderNode(uint16_t id) {
	gl::pushMMat();
	gl::MMat() *= model.nodes[id].transformation;
	glsl::Mmat = gl::MMat();
	glsl::MVPmat = gl::MVPMat();
	//node->material->get<std::string>("diffuseTexture") //<<<Bind Textures here
	//vl::glsl::material = materials[entries[i].MaterialIndex]; //<<<Material here

	if (gl::MMat()[0][0] * gl::MMat()[1][1] * gl::MMat()[2][2] < 0) {
		glFrontFace(GL_CCW);
	} else {
		glFrontFace(GL_CW);
	}


	for (auto meshID : model.nodes[id].meshIDs) {
		glDrawElementsBaseVertex(GL_TRIANGLES,
				model.meshes[meshID].numIndices,
				GL_UNSIGNED_INT,
				(void*) (sizeof (GLuint) * model.meshes[meshID].baseIndex),
				model.meshes[meshID].baseVertex);
		checkGL();
	}

	for (auto childID : model.nodes[id].childrenIDs) {
		renderNode(childID);
	}

	gl::popMMat();
	checkGL();
}

bool ModelImpl::loaded() {
	return loaded_.load();
}


// -------------------------------------------------------------------------------------

ModelImpl::ModelImpl(const std::string& name, ModelContext* context) :
	context_(context),
	name(name),
	loaded_(false) {
	context_->io.executeAsync([this] {
		if (loadIO())
			context_->gl.executeAsync([this] {
				if (loadGL())
					loaded_.exchange(true);
			});
	});
}

ModelImpl::~ModelImpl() { }

bool operator<(const std::tuple<std::string, ModelContext*>& args, const ModelImpl& model) {
	if (model.getName() != std::get<0>(args))
		return std::get<0>(args) < model.getName();
	else
		return std::get<1>(args) < model.getContext();
}

bool operator<(const ModelImpl& model, const std::tuple<std::string, ModelContext*>& args) {
	if (model.getName() != std::get<0>(args))
		return model.getName() < std::get<0>(args);
	else
		return model.getContext() < std::get<1>(args);
}

bool operator<(const ModelImpl& lhs, const ModelImpl& rhs) {
	if (lhs.getName() != rhs.getName())
		return lhs.getName() < rhs.getName();
	else
		return lhs.getContext() < rhs.getContext();
}

} //namespace detail
} //namespace gl
} //namespace vl





/* ---------------------------------------------------------------------------- */

//			void recursive_render(const aiScene *sc, const aiNode* nd) {
//				unsigned int i;
//				unsigned int n = 0;
//				aiMatrix4x4 m = nd->mTransformation;
//				/* update transform */
//				aiTransposeMatrix4(&m);
//				glPushMatrix();
//				glMultMatrixf((float*) &m);
//
//				aiVector3D t, s;
//				aiQuaternion r;
//				m.Decompose(s, r, t);
//				std::cout << nd->mName.C_Str() << " \t";
//				std::cout << t.x << "," << t.y << "," << t.z << "   ";
//				std::cout << r.w << "," << r.x << "," << r.y << "," << r.z << "   ";
//				std::cout << s.x << "," << s.y << "," << s.z << "   " << std::endl;
//				aiMatrix4x4 m2 = nd->mTransformation;
//				m2.Decompose(s, r, t);
//				std::cout << nd->mName.C_Str() << " \t";
//				std::cout << t.x << "," << t.y << "," << t.z << "   ";
//				std::cout << r.x << "," << r.y << "," << r.z << "," << r.w << "   ";
//				std::cout << s.x << "," << s.y << "," << s.z << "   " << std::endl;
//
//				/* draw all meshes assigned to this node */
//				for (; n < nd->mNumMeshes; ++n) {
//					const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
//					applyMaterial(sc->mMaterials[mesh->mMaterialIndex]);
//					if (mesh->mNormals == nullptr) {
//						glDisable(GL_LIGHTING);
//					} else {
//						glEnable(GL_LIGHTING);
//					}
//					for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
//						const struct aiFace* face = &mesh->mFaces[t];
//						GLenum face_mode;
//						switch (face->mNumIndices) {
//							case 1: face_mode = GL_POINTS;
//								break;
//							case 2: face_mode = GL_LINES;
//								break;
//							case 3: face_mode = GL_TRIANGLES;
//								break;
//							default: face_mode = GL_POLYGON;
//								break;
//						}
//						glBegin(face_mode);
//						for (i = 0; i < face->mNumIndices; i++) {
//							int index = face->mIndices[i];
//							if (mesh->mColors[0] != nullptr)
//								glColor4fv((GLfloat*) & mesh->mColors[0][index]);
//							if (mesh->mNormals != nullptr)
//								glNormal3fv(&mesh->mNormals[index].x);
//							glVertex3fv(&mesh->mVertices[index].x);
//						}
//						glEnd();
//					}
//				}
//				/* draw all children */
//				for (n = 0; n < nd->mNumChildren; ++n) {
//					recursive_render(sc, nd->mChildren[n]);
//				}
//				glPopMatrix();
//			}
//			void display() {
//				if (scene_list == 0) {
//					scene_list = glGenLists(1);
//					glNewList(scene_list, GL_COMPILE);
//
//					aiMatrix4x4 unit;
//					aiMatrix4x4::Scaling({100, 100, 100}, unit); //For maya centimeter "support"
//					aiMultiplyMatrix4(&scene->mRootNode->mTransformation, &unit);
//					recursive_render(scene, scene->mRootNode);
//					glEndList();
//				}
//				glCallList(scene_list);
//			}