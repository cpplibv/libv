// Project: libv.glr, File: src/libv/glr/remote.hpp, Author: Császár Mátyás [Vader]

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

// =================================================================================================

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


//struct ExecutionPlan {
//	struct StateCapability {
//		libv::gl::Capability capability;
//		bool value;
//	};
//
//	struct StateTexture {
//		uint8_t channel;
//		libv::gl::Texture* texture;
//	};
//
//	struct StateUniform {
//
//	};
//
//	struct StateMesh {
//	};
//
//	struct Action {
//		uint8_t changeCapability = 0;
//		uint8_t changeTexture = 0;
//		uint8_t changeUniform = 0;
//		uint8_t changeMesh = 0;
//	};
//
//	std::vector<StateCapability> stateCapability;
//	std::vector<StateTexture> stateTexture;
//	std::vector<StateUniform> stateUniform;
//	std::vector<StateMesh> stateMesh;
//
//	std::vector<Action> queue;
//};


//	struct RenderTask {
//		uint8_t num_texture;
//		uint16_t num_uniform;
//
//		uint8_t pass;
//		uint16_t target;
//		uint16_t program;
//		uint32_t state;
//
//		float z_hint;
//
//		uint32_t num_vertex;
//		uint32_t first_vertex;
//		uint32_t vao;
//		libv::gl::Primitive primitive;
//	};
//
//	struct Task {
//		State state;
//		uint16_t program;
//
//		size_t first_texture;
//		size_t first_uniform;
//		uint8_t pass;
//		uint8_t num_texture;
//		uint16_t num_uniform;
//	};
//
//	std::vector<Task> tasks;
//
//	std::vector<std::pair<gl::TextureChannel, Texture>> storeTexture;
//
//	std::vector<double> storeUniform_double;
//	std::vector<float> storeUniform_float;
//	std::vector<int32_t> storeUniform_int32;
//	std::vector<int64_t> storeUniform_int64;
//	std::vector<uint32_t> storeUniform_uint32;
//	std::vector<uint64_t> storeUniform_uint64;

// --- OpenGL Rendering Engine ---------------------------------------------------------------------
//
// Procedural Stack -> State + Operation -> Execution plan -> Execution
//
// User facing API:
//	- input: state stack with class OpenGL structure and control flow on a OpenGL queue object
//	- thread: multiple thread, no shared data
//	- process: flattening of instruction and OpenGL calls
//	- process: presort queue object
//	- output: order independent flat state and operation set
//
// Processing ordering and optimization step:
//	- input: multiple OpenGL queue object
//	- thread: single thread processing
//	- process: merge sort queues based on states, bindings and actions
//	- process: instructions, function pointers real OpenGL call mapping
//	- output: serialized function pointers and arguments
//
// Rendering:
//	- input: serialized function pointers and arguments
//	- process: execute instructions
//
// -------------------------------------------------------------------------------------------------
