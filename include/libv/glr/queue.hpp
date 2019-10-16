// File: queue.hpp - Created on 2017.11.25. 05:06 - Author: Vader

#pragma once

// libv
#include <libv/gl/framebuffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/matrix_stack.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
#include <libv/utility/overload.hpp>
// std
#include <cstdint>
#include <functional>
#include <optional>
#include <variant>
// pro
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/state.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_buffer.hpp>


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
	std::shared_ptr<RemoteProgram> program;
	std::shared_ptr<RemoteMesh> mesh;

	QueueTaskMesh(
			SequenceNumber sequenceNumber,
			State state,
			const std::shared_ptr<RemoteProgram>& program,
			const std::shared_ptr<RemoteMesh>& mesh) :
		sequenceNumber(sequenceNumber),
		state(state),
		program(program),
		mesh(mesh) { }
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

struct QueueTaskViewport {
	SequenceNumber sequenceNumber;
	libv::vec2i position;
	libv::vec2i size;

	QueueTaskViewport(SequenceNumber sequenceNumber, libv::vec2i position, libv::vec2i size) :
		sequenceNumber(sequenceNumber), position(position), size(size) { }
};

struct QueueTaskFramebuffer {
	SequenceNumber sequenceNumber;
	std::optional<Framebuffer> framebuffer;
	enum class Mode {
		both,
		draw,
		read,

		default_both,
		default_draw,
		default_read,
	} mode;

	// C++20: aggregate ctors
	QueueTaskFramebuffer(SequenceNumber sequenceNumber, Framebuffer&& framebuffer, Mode mode) :
		sequenceNumber(sequenceNumber), framebuffer(std::move(framebuffer)), mode(mode) { }

	QueueTaskFramebuffer(SequenceNumber sequenceNumber, Mode mode) :
		sequenceNumber(sequenceNumber), mode(mode) { }
};

struct QueueTaskBlit {
	SequenceNumber sequenceNumber;
	std::optional<Framebuffer> framebuffer_src;
	std::optional<Framebuffer> framebuffer_dst;
	libv::vec2i src_pos;
	libv::vec2i src_size;
	libv::vec2i dst_pos;
	libv::vec2i dst_size;
	libv::gl::BufferBit mask;
	libv::gl::MagFilter filter;

	inline void execute(libv::gl::GL& gl, Remote& remote) const noexcept {
		auto previous_read = gl.framebuffer_read();
		auto previous_draw = gl.framebuffer_draw();

		if (!framebuffer_dst && !framebuffer_src) {
			gl.framebuffer_default();

		} else if (!framebuffer_dst) {
			AttorneyRemoteFramebuffer::bind_read(*framebuffer_src, gl, remote);
			gl.framebuffer_default_draw();

		} else if (!framebuffer_src) {
			gl.framebuffer_default_read();
			AttorneyRemoteFramebuffer::bind_draw(*framebuffer_dst, gl, remote);

		} else {
			AttorneyRemoteFramebuffer::bind_read(*framebuffer_src, gl, remote);
			AttorneyRemoteFramebuffer::bind_draw(*framebuffer_dst, gl, remote);
		}

		gl.blit(src_pos, src_size, dst_pos, dst_size, mask, filter);

		gl.framebuffer_read(previous_read);
		gl.framebuffer_draw(previous_draw);
	}
};

struct Queue {
public:
	StateStack state;
	libv::gl::MatrixStack<libv::mat4f> model;
	libv::gl::MatrixStack<libv::mat4f> view;
	libv::gl::MatrixStack<libv::mat4f> projection;

private:
	std::stack<std::shared_ptr<RemoteProgram>, std::vector<std::shared_ptr<RemoteProgram>>> programStack;

private:
	std::vector<std::variant<QueueTaskMesh, QueueTaskUniformBlock, QueueTaskTexture, QueueTaskClear, QueueTaskClearColor, QueueTaskViewport, QueueTaskFramebuffer, QueueTaskBlit>> tasks;
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

private:
	template <typename T, typename... Args>
	inline void add(Args&&... args) {
		tasks.emplace_back(std::in_place_type<T>, T{std::forward<Args>(args)...});
	}

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
	void viewport(libv::vec2i position, libv::vec2i size) {
		tasks.emplace_back(std::in_place_type<QueueTaskViewport>, sequenceNumber, position, size);
	}

public:
	void framebuffer(Framebuffer framebuffer) {
		sequencePoint();
		tasks.emplace_back(std::in_place_type<QueueTaskFramebuffer>, sequenceNumber, std::move(framebuffer), QueueTaskFramebuffer::Mode::both);
		sequencePoint();
	}

	void framebuffer_draw(Framebuffer framebuffer) {
		sequencePoint();
		tasks.emplace_back(std::in_place_type<QueueTaskFramebuffer>, sequenceNumber, std::move(framebuffer), QueueTaskFramebuffer::Mode::draw);
		sequencePoint();
	}

	void framebuffer_read(Framebuffer framebuffer) {
		sequencePoint();
		tasks.emplace_back(std::in_place_type<QueueTaskFramebuffer>, sequenceNumber, std::move(framebuffer), QueueTaskFramebuffer::Mode::read);
		sequencePoint();
	}

	void framebuffer_deafult() {
		sequencePoint();
		tasks.emplace_back(std::in_place_type<QueueTaskFramebuffer>, sequenceNumber, QueueTaskFramebuffer::Mode::default_both);
		sequencePoint();
	}

	void framebuffer_draw_deafult() {
		sequencePoint();
		tasks.emplace_back(std::in_place_type<QueueTaskFramebuffer>, sequenceNumber, QueueTaskFramebuffer::Mode::default_draw);
		sequencePoint();
	}

	void framebuffer_read_deafult() {
		sequencePoint();
		tasks.emplace_back(std::in_place_type<QueueTaskFramebuffer>, sequenceNumber, QueueTaskFramebuffer::Mode::default_read);
		sequencePoint();
	}

public:
	void blit(Framebuffer src, Framebuffer dst, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
		sequencePoint();
		add<QueueTaskBlit>(sequenceNumber, std::move(src), std::move(dst), src_pos, src_size, dst_pos, dst_size, mask, filter);
		sequencePoint();
	}

	void blit_to_default(Framebuffer src, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
		sequencePoint();
		add<QueueTaskBlit>(sequenceNumber, std::move(src), std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
		sequencePoint();
	}

	void blit_from_default(Framebuffer dst, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
		sequencePoint();
		add<QueueTaskBlit>(sequenceNumber, std::nullopt, std::move(dst), src_pos, src_size, dst_pos, dst_size, mask, filter);
		sequencePoint();
	}

	void blit_default(libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
		sequencePoint();
		add<QueueTaskBlit>(sequenceNumber, std::nullopt, std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
		sequencePoint();
	}

public:
	template <typename T>
	void uniform(const Uniform_t<T> uniform, const T& value) {
		programStack.top()->uniformStream.set(uniform.location, value);
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
	void program(const Program& program_) {
		programStack.top() = AttorneyProgramRemote::remote(program_);
	}
	void render(const Mesh& mesh) {
		programStack.top()->uniformStream.endBatch();

		tasks.emplace_back(std::in_place_type<QueueTaskMesh>,
				sequenceNumber,
				state.state(),
				programStack.top(),
				AttorneyMeshRemote::remote(mesh));
	}

private:
	static inline void changeState(libv::gl::GL& gl, State target) {
		gl.capability.blend.set(target.capabilityBlend != 0);
		gl.capability.cullFace.set(target.capabilityCullFace != 0);
		gl.capability.depthTest.set(target.capabilityDepthTest != 0);
		gl.capability.multisample.set(target.capabilityMultisample != 0);
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

					task.program->use(gl, remote);
					task.mesh->render(gl, remote);
				},
				[&gl, &remote](const QueueTaskUniformBlock& task) {
					task.view.remote->bind(gl, remote, task.view.binding, task.view.block);
				},
				[&gl, &remote](const QueueTaskTexture& task) {
					gl.activeTexture(task.channel);

					auto object = AttorneyRemoteTexture::sync_might_bind(task.texture, gl, remote);
					gl.bind(object);
				},
				[&gl](const QueueTaskClearColor& task) {
					gl.clearColor(task.color);
				},
				[&gl](const QueueTaskClear& task) {
					gl.clear(task.buffers);
				},
				[&gl](const QueueTaskViewport& task) {
					gl.viewport(task.position, task.size);
				},
				[&gl, &remote](const QueueTaskFramebuffer& task) {
					switch (task.mode) {
					case QueueTaskFramebuffer::Mode::both:
						return AttorneyRemoteFramebuffer::bind(*task.framebuffer, gl, remote);

					case QueueTaskFramebuffer::Mode::draw:
						return AttorneyRemoteFramebuffer::bind_draw(*task.framebuffer, gl, remote);

					case QueueTaskFramebuffer::Mode::read:
						return AttorneyRemoteFramebuffer::bind_read(*task.framebuffer, gl, remote);

					case QueueTaskFramebuffer::Mode::default_both:
						return gl.framebuffer_default();

					case QueueTaskFramebuffer::Mode::default_draw:
						return gl.framebuffer_default_draw();

					case QueueTaskFramebuffer::Mode::default_read:
						return gl.framebuffer_default_read();
					}
				},
				[&gl, &remote](const QueueTaskBlit& task) {
					task.execute(gl, remote);
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
