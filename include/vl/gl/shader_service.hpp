// File:   shader_service.hpp

#pragma once

// vl
#include <vl/cache.hpp>
#include <vl/worker_thread.hpp>

namespace vl {
namespace gl {

// ShaderService -----------------------------------------------------------------------------------

struct ShaderService {
	vl::LoaderCache<TextFile> cacheTextFile;
	vl::LoaderCache<Shader> cacheShader;
	vl::LoaderCache<ShaderProgramImpl> cacheShaderProgramImpl;

	vl::WorkerThread threadIO;
	vl::WorkerThread threadGL;
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl