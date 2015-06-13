// File: model_context.hpp, Created on 2015. március 12. 7:27, Author: Vader

#pragma once

#include <vl/worker_thread.hpp>

namespace vl {
namespace gl {

struct ModelContext {
	vl::Context& gl;
	vl::Context& io;

	ModelContext(vl::Context& gl, vl::Context& io) : gl(gl), io(io) { }
};

struct TextureContext {
	vl::Context& gl;
	vl::Context& io;

	TextureContext(vl::Context& gl, vl::Context& io) : gl(gl), io(io) { }
};

} //namespace gl
} //namespace vl
