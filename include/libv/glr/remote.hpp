// File: remote.hpp - Created on 2017.10.28. 07:14 - Author: Vader

#pragma once

// libv
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/buffer_object.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/gl/texture_object.hpp>
#include <libv/gl/vertex_array_object.hpp>
// std
#include <thread>
#include <vector>
// pro
#include <libv/glr/queue.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

//struct Batch {
//	uint16_t num_texture;
//	uint16_t num_uniform;
//	uint16_t num_vao;
////	size_t start;
////	size_t size;
//
//	// Pass
//	//	UIProgramProxy program;
//	// Uniforms
//	// std::vector<unsiforms> uniforms;
//	// VAO
//};

//struct BatchProcess {
//	std::vector<Batch> batches;
//
//	// Pass passes;
//	std::vector<UIProgramProxy> program;
//	std::vector<libv::gl::TextureChannel, UITextureProxy> textures;
//	//	std::vector<UIUniform> uniform;
//	std::vector<libv::gl::VertexArray> vao;
//};
//
//struct ExecutionQueue {
//	std::vector<BatchProcess> processes;
//};

// -------------------------------------------------------------------------------------------------

struct Remote {
private:
public:
	libv::gl::GL gl;

private:
	std::vector<Queue> queues;

private:
	std::vector<libv::gl::Buffer> gc_buffers;
	std::vector<libv::gl::Program> gc_program;
	std::vector<libv::gl::Texture> gc_textures;
	std::vector<libv::gl::VertexArray> gc_vertexArrays;

private:
	std::thread::id contextThreadID;
	bool initalized = false;

public:
	Remote();
	~Remote();

public:
	inline void gc(libv::gl::Buffer object) {
		gc_buffers.emplace_back(object);
	}
	inline void gc(libv::gl::Program object) {
		gc_program.emplace_back(object);
	}
	inline void gc(libv::gl::Texture object) {
		gc_textures.emplace_back(object);
	}
	inline void gc(libv::gl::VertexArray object) {
		gc_vertexArrays.emplace_back(object);
	}

public:
	Queue queue();
	void queue(Queue&& queue);
	void clear();

public:
	void enableTrace();
	void enableDebug();

public:
	void create();
	void render();
	void destroy();
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
