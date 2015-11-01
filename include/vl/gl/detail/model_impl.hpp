// File: model.hpp, Created on 2014. november 28. 18:38, Author: Vader

#pragma once

// vl
#include <vl/vm3/model.hpp>
// std
#include <atomic>
#include <string>
#include <memory>
#include <vector>
// pro
#include <vl/gl/gl.hpp>

namespace vl {
namespace gl {
struct ModelContext;

namespace detail {

//class ModelImpl {
//private:
//	ModelContext* context_;
//	vl::vm3::Model model;
//private:
//	uint32_t vao;
//	uint32_t vbo_vertex;
//	uint32_t vbo_index;
//
//private:
//	std::string name;
//	std::atomic<bool> loaded_;
//
//private:
//	bool loadIO();
//	bool loadGL();
//	bool unloadIO();
//	bool unloadGL();
//
//public:
//	const std::string& getName() const {
//		return name;
//	}
//
//	ModelContext* getContext() const {
//		return context_;
//	}
//
//	ModelImpl(const std::string& name, ModelContext* context);
//	void render(vl::gl::GL&);
//	void renderNode(uint16_t id, vl::gl::GL&);
//	bool loaded();
//	virtual ~ModelImpl();
//};
//
//bool operator<(const std::tuple<std::string, ModelContext*>&, const ModelImpl&);
//bool operator<(const ModelImpl&, const std::tuple<std::string, ModelContext*>&);
//bool operator<(const ModelImpl&, const ModelImpl&);

} //namespace detail
} //namespace gl
} //namespace vl
