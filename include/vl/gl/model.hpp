// File: model.hpp, Created on 2014. november 28. 17:44, Author: Vader

#pragma once

// vl
#include <vl/resource.hpp>
// std
#include <memory>
// pro
#include <vl/gl/detail/model_impl.hpp>
#include <vl/gl/gl_contexts.hpp>

namespace vl {
namespace gl {

class GL;

class ModelManager : public vl::ResourceManager<detail::ModelImpl> {
public:
	ModelContext* context;
	ModelManager(ModelContext* context);
};

class Model : public vl::Resource<detail::ModelImpl> {
public:
	void render(vl::gl::GL& gl);
public:
	Model(ModelManager& resourceManager, const std::string& filePath);
};

} //namespace gl
} //namespace vl