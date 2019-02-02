// File: queue.hpp - Created on 2017.11.25. 05:06 - Author: Vader

#pragma once

// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/matrix_stack.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
#include <libv/utility/overload.hpp>
// std
#include <cstdint>
#include <functional>
#include <variant>
// pro
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/state.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/glr/texture.hpp>


namespace libv {
namespace glr {

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

class Remote;

using SequenceNumber = uint32_t;

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

struct QueueTaskMesh {
	SequenceNumber sequenceNumber;
	State state;
	Program program;
	Mesh mesh;

	QueueTaskMesh(SequenceNumber sequenceNumber, State state, Program program, Mesh&& mesh) :
		sequenceNumber(sequenceNumber), state(state), program(std::move(program)), mesh(std::move(mesh)) { }
};

struct QueueTaskClear {
	SequenceNumber sequenceNumber;
	libv::gl::BufferBit buffers;
};

struct QueueTaskClearColor {
	SequenceNumber sequenceNumber;
	libv::vec4f color;
};

struct QueueTaskUniformBlock {
	SequenceNumber sequenceNumber;
	UniformBlockUniqueView_std140 view;

	QueueTaskUniformBlock(SequenceNumber sequenceNumber, UniformBlockUniqueView_std140&& view) :
		sequenceNumber(sequenceNumber), view(std::move(view)) { }
};

struct QueueTaskTexture {
	SequenceNumber sequenceNumber;
	Texture texture;
	libv::gl::TextureChannel channel;

	QueueTaskTexture(SequenceNumber sequenceNumber, Texture&& texture, libv::gl::TextureChannel channel) :
		sequenceNumber(sequenceNumber), texture(std::move(texture)), channel(channel) { }
};

struct Queue {
public:
	StateStack state;
	libv::gl::MatrixStack<libv::mat4f> model;
	libv::gl::MatrixStack<libv::mat4f> view;
	libv::gl::MatrixStack<libv::mat4f> projection;

private:
	std::stack<Program, std::vector<Program>> programStack;

private:
	std::vector<std::variant<QueueTaskMesh, QueueTaskUniformBlock, QueueTaskTexture, QueueTaskClear, QueueTaskClearColor>> tasks;
	SequenceNumber sequenceNumber = 0;
	State currentState;

public:
	Queue() {
		programStack.emplace();
	}

public:
	// Registers a sequence point as a sorting fence
	void sequencePoint() {
		sequenceNumber++;
	}

private:
//	void sort() {
//		// Preferred sorting order would be:
//		// pass >> target >> capabilities >> programs >> texture >> uniform >> z_hint >> buffer
//	}

public:
	inline const libv::mat4f mvp() const {
		return projection * view * model;
	}

	inline const libv::vec3f eye() const {
		return libv::vec::xyz(view.top().inverse_copy()[3]);
	}

	inline const libv::mat4f getMatrixTexture() const {
		return libv::mat4f{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f};
	}

	void setClearColor(const float r, const float g, const float b, const float a) {
		setClearColor(libv::vec4f{r, g, b, a});
	}

	void setClearColor(const libv::vec4f rgba) {
		sequencePoint();
		tasks.emplace_back(QueueTaskClearColor{sequenceNumber, rgba});
	}

	void clearColor() {
		if (!tasks.empty() && std::holds_alternative<QueueTaskClear>(tasks.back())) {
			auto& buff = std::get<QueueTaskClear>(tasks.back()).buffers;
			buff = buff | libv::gl::BufferBit::Color;
		} else {
			sequencePoint();
			tasks.emplace_back(QueueTaskClear{sequenceNumber, libv::gl::BufferBit::Color});
			sequencePoint();
		}
	}

	void clearDepth() {
		if (!tasks.empty() && std::holds_alternative<QueueTaskClear>(tasks.back())) {
			auto& buff = std::get<QueueTaskClear>(tasks.back()).buffers;
			buff = buff | libv::gl::BufferBit::Depth;
		} else {
			sequencePoint();
			tasks.emplace_back(QueueTaskClear{sequenceNumber, libv::gl::BufferBit::Depth});
			sequencePoint();
		}
	}

public:
	template <typename T>
	void uniform(const Uniform_t<T> uniform, const T& value) {
		programStack.top().uniform(uniform, value);
	}

	void uniform(UniformBlockUniqueView_std140&& view) {
		tasks.emplace_back(std::in_place_type<QueueTaskUniformBlock>, sequenceNumber, std::move(view));
	}
	void texture(Texture texture, const libv::gl::TextureChannel channel) {
		tasks.emplace_back(std::in_place_type<QueueTaskTexture>, sequenceNumber, std::move(texture), channel);
	}
//	void pop_program(Program program) {}
//	void push_program() {}
//	void push_program(Program program) {}
//	void push_program_guard() {}
//	void push_program_guard(Program program) {}
	void program(Program program_) {
		programStack.top() = std::move(program_);
	}
	void render(Mesh mesh) {
		auto& remoteProgram = AttorneyProgramRemote::remote(programStack.top());
		remoteProgram.uniformStream.endBatch();

		tasks.emplace_back(std::in_place_type<QueueTaskMesh>,
				sequenceNumber, state.state(), programStack.top(), std::move(mesh));
	}

private:
	inline static void changeState(libv::gl::GL& gl, State target) {
		gl.capability.blend.set(target.capabilityBlend != 0);
		gl.capability.cullFace.set(target.capabilityCullFace != 0);
		gl.capability.depthTest.set(target.capabilityDepthTest != 0);
		gl.capability.rasterizerDiscard.set(target.capabilityRasterizerDiscard != 0);
		gl.capability.scissorTest.set(target.capabilityScissorTest != 0);
		gl.capability.stencilTest.set(target.capabilityStencilTest != 0);
		gl.capability.textureCubeMapSeamless.set(target.capabilityTextureCubeMapSeamless != 0);

		gl.frontFace.set(target.frontFaceCCW == 0 ? libv::gl::FrontFace::CW : libv::gl::FrontFace::CCW);
		gl.cullFace.set(target.cullFace == 0 ? libv::gl::CullFace::Back : libv::gl::CullFace::Front);

		std::array polygonModeTable{
			libv::gl::PolygonMode::Fill,
			libv::gl::PolygonMode::Line,
			libv::gl::PolygonMode::Point,
		};
		gl.polygonMode.set(polygonModeTable[target.polygonMode]);

		std::array depthFunctionTable{
			libv::gl::TestFunction::LEqual,
			libv::gl::TestFunction::GEqual,
			libv::gl::TestFunction::Less,
			libv::gl::TestFunction::Greater,
			libv::gl::TestFunction::Equal,
			libv::gl::TestFunction::NotEqual,
			libv::gl::TestFunction::Always,
			libv::gl::TestFunction::Never,
		};
		gl.depthFunction.set(depthFunctionTable[target.depthFunction]);

		std::array blendFunctionTable{
			libv::gl::BlendFunction::ConstantAlpha,
			libv::gl::BlendFunction::ConstantColor,
			libv::gl::BlendFunction::DestinationAlpha,
			libv::gl::BlendFunction::DestinationColor,
			libv::gl::BlendFunction::One,
			libv::gl::BlendFunction::One_Minus_ConstantAlpha,
			libv::gl::BlendFunction::One_Minus_ConstantColor,
			libv::gl::BlendFunction::One_Minus_DestinationAlpha,
			libv::gl::BlendFunction::One_Minus_DestinationColor,
			libv::gl::BlendFunction::One_Minus_Source1Alpha,
			libv::gl::BlendFunction::One_Minus_Source1Color,
			libv::gl::BlendFunction::One_Minus_SourceAlpha,
			libv::gl::BlendFunction::One_Minus_SourceColor,
			libv::gl::BlendFunction::Source1Alpha,
			libv::gl::BlendFunction::Source1Color,
			libv::gl::BlendFunction::SourceAlpha,
			libv::gl::BlendFunction::SourceAlphaSaturate,
			libv::gl::BlendFunction::SourceColor,
			libv::gl::BlendFunction::Zero,
		};
		gl.blendFunction.set(blendFunctionTable[target.blendSrcFunction], blendFunctionTable[target.blendDstFunction]);

		gl.clipPlanes.set(target.clipPlanes);

		gl.mask.depth.set(target.maskDepth);
	}

public:
	void execute(libv::gl::GL& gl, Remote& remote) {
		for (const auto& task_variant : tasks) {

			auto execution = libv::overload(
				[&gl, &remote, this](const QueueTaskMesh& task) {
					if (currentState != task.state) {
						changeState(gl, task.state);
						currentState = task.state;
					}

					auto& remoteProgram = AttorneyProgramRemote::remote(task.program);
					remoteProgram.use(gl, remote);

					auto& remoteMesh = AttorneyMeshRemote::remote(task.mesh);
					remoteMesh.render(gl, remote);
				},
				[&gl, &remote](const QueueTaskUniformBlock& task) {
					task.view.remote->bind(gl, remote, task.view.binding, task.view.block);
				},
				[&gl, &remote](const QueueTaskTexture& task) {
					gl.activeTexture(task.channel);

					auto& remoteTexture = AttorneyTextureRemote::remote(task.texture);
					remoteTexture.bind(gl, remote);
				},
				[&gl](const QueueTaskClearColor& task) {
					gl.clearColor(task.color);
				},
				[&gl](const QueueTaskClear& task) {
					gl.clear(task.buffers);
				});

			std::visit(execution, task_variant);
		}
		tasks.clear();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv

// =================================================================================================

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
