// File: service.hpp

#pragma once

// vl
#include <vl/cache.hpp>
#include <vl/worker_thread.hpp>
#include <vl/gl/detail/texture_impl.hpp> // TODO P1: So yeh... i dont like this.

namespace vl {
namespace gl {

// ServiceGL -----------------------------------------------------------------------------------

struct ServiceGL {
	ServiceGL(vl::WorkerThread* threadGL) :
		threadGL(threadGL) { }

	vl::WorkerThread* threadGL;
};

// ServiceIO -----------------------------------------------------------------------------------

struct ServiceIO {
	ServiceIO(vl::WorkerThread* threadIO) :
		threadIO(threadIO) { }

	vl::WorkerThread* threadIO;
};

// ServiceShader -----------------------------------------------------------------------------------

class TextFile;
class Shader;
class ShaderProgramImpl;

struct ServiceShader : ServiceIO, ServiceGL {
	ServiceShader(vl::WorkerThread* threadIO, vl::WorkerThread* threadGL) :
		ServiceIO(threadIO), ServiceGL(threadGL) { }

	vl::LoaderCache<TextFile> cacheTextFile;
	vl::LoaderCache<Shader> cacheShader;
	vl::LoaderCache<ShaderProgramImpl> cacheShaderProgramImpl;
};

// ServiceTexture ----------------------------------------------------------------------------------

struct ServiceTexture : ServiceIO, ServiceGL {
	ServiceTexture(vl::WorkerThread* threadIO, vl::WorkerThread* threadGL) :
		ServiceIO(threadIO), ServiceGL(threadGL) { }

	vl::LoaderCache<detail::TextureImpl> cacheTextureImpl;
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl