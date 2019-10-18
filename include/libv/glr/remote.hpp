// File: remote.hpp - Created on 2017.10.28. 07:14 - Author: Vader

#pragma once

// libv
#include <libv/gl/enum.hpp>
#include <libv/gl/gl_fwd.hpp>
#include <libv/math/vec.hpp>
// std
#include <memory>


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

class ImplRemote;
class Queue;
class DestroyQueues;

struct Remote {
private:
	std::unique_ptr<ImplRemote> self;

public:
	Remote();
	~Remote();

public:
	Queue queue();
	void queue(Queue&& queue);
	DestroyQueues& destroyQueues() noexcept;
	void clear() noexcept;

public:
	void enableTrace() noexcept;
	void enableDebug() noexcept;

public:
	void readPixels(libv::vec2i pos, libv::vec2i size, libv::gl::ReadFormat format, libv::gl::DataType type, void* data) noexcept;
	libv::gl::GL& gl() noexcept;

public:
	void create();
	void execute();
	void destroy();
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
