// File: Model.cpp, Created on 2014. december 6. 22:44, Author: Vader

// hpp
#include "vl/gl/model.hpp"
// vl
#include <vl/vm3/model.hpp>

namespace vl {
namespace gl {

ModelManager::ModelManager(ModelContext* context) :
	context(context) { }

Model::Model(ModelManager& resourceManager, const std::string& filePath) :
	vl::Resource<detail::ModelImpl>(resourceManager, filePath, resourceManager.context) { }

void Model::render() {
	if (data().loaded())
		data().render();
}

} //namespace gl
} //namespace vl
