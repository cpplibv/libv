// File:   mesh.cpp Author: Vader Created on 2019. január 25., 6:27

// hpp
#include <libv/glr/queue.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/meta/similar.hpp>
// std
#include <optional>
#include <variant>
//// pro
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/state.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

using SequenceNumber = uint32_t;

// -------------------------------------------------------------------------------------------------

struct QueueTaskMesh {
	SequenceNumber sequenceNumber;
	State state;
	std::shared_ptr<RemoteProgram> program;
	std::shared_ptr<RemoteMesh> mesh;

	inline void changeState(libv::gl::GL& gl, State target) const noexcept {
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

	inline void execute(libv::gl::GL& gl, Remote& remote, State currentState) const noexcept {
		if (currentState != state) {
			changeState(gl, state);
			currentState = state;
		}

		program->use(gl, remote);
		mesh->render(gl, remote);
	}
};

struct QueueTaskClear {
	SequenceNumber sequenceNumber;
	libv::gl::BufferBit buffers;

	inline void execute(libv::gl::GL& gl, Remote& remote) const noexcept {
		(void) remote;
		gl.clear(buffers);
	}
};

struct QueueTaskClearColor {
	SequenceNumber sequenceNumber;
	libv::vec4f color;

	inline void execute(libv::gl::GL& gl, Remote& remote) const noexcept {
		(void) remote;
		gl.clearColor(color);
	}
};

struct QueueTaskUniformBlock {
	SequenceNumber sequenceNumber;
	UniformBlockUniqueView_std140 view;

	inline void execute(libv::gl::GL& gl, Remote& remote) const noexcept {
		view.remote->bind(gl, remote, view.binding, view.block);
	}
};

struct QueueTaskTexture {
	SequenceNumber sequenceNumber;
	Texture texture;
	libv::gl::TextureChannel channel;

	inline void execute(libv::gl::GL& gl, Remote& remote) const noexcept {
		gl.activeTexture(channel);
		auto object = AttorneyRemoteTexture::sync_might_bind(texture, gl, remote);
		gl.bind(object);
	}
};

struct QueueTaskViewport {
	SequenceNumber sequenceNumber;
	libv::vec2i position;
	libv::vec2i size;

	inline void execute(libv::gl::GL& gl, Remote& remote) const noexcept {
		(void) remote;
		gl.viewport(position, size);
	}
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

	QueueTaskFramebuffer(SequenceNumber sequenceNumber, Framebuffer&& framebuffer, Mode mode) :
		sequenceNumber(sequenceNumber), framebuffer(std::move(framebuffer)), mode(mode) { }

	QueueTaskFramebuffer(SequenceNumber sequenceNumber, Mode mode) :
		sequenceNumber(sequenceNumber), mode(mode) { }

	inline void execute(libv::gl::GL& gl, Remote& remote) const noexcept {
		switch (mode) {
		case QueueTaskFramebuffer::Mode::both:
			return AttorneyRemoteFramebuffer::bind(*framebuffer, gl, remote);

		case QueueTaskFramebuffer::Mode::draw:
			return AttorneyRemoteFramebuffer::bind_draw(*framebuffer, gl, remote);

		case QueueTaskFramebuffer::Mode::read:
			return AttorneyRemoteFramebuffer::bind_read(*framebuffer, gl, remote);

		case QueueTaskFramebuffer::Mode::default_both:
			return gl.framebuffer_default();

		case QueueTaskFramebuffer::Mode::default_draw:
			return gl.framebuffer_default_draw();

		case QueueTaskFramebuffer::Mode::default_read:
			return gl.framebuffer_default_read();
		}
	}
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

// -------------------------------------------------------------------------------------------------

struct ImplQueue {
	std::stack<std::shared_ptr<RemoteProgram>, std::vector<std::shared_ptr<RemoteProgram>>> programStack;

	std::vector<std::variant<
			QueueTaskMesh, QueueTaskUniformBlock, QueueTaskTexture,
			QueueTaskClear, QueueTaskClearColor, QueueTaskViewport,
			QueueTaskFramebuffer, QueueTaskBlit
	>> tasks;

	SequenceNumber sequenceNumber = 0;
	State currentState;

	template <typename T, typename... Args>
	inline void add(Args&&... args) {
		tasks.emplace_back(std::in_place_type<T>, T{std::forward<Args>(args)...});
	}
};

// -------------------------------------------------------------------------------------------------

Queue::Queue() :
	self(std::make_unique<ImplQueue>()) {
	self->programStack.emplace();
}

Queue::Queue(Queue&&) noexcept = default;

Queue::~Queue() {
}

// -------------------------------------------------------------------------------------------------

void Queue::sequencePoint() noexcept {
	self->sequenceNumber++;
}

//	void Queue::sort() {
//		// Preferred sorting order would be:
//		// pass >> target >> capabilities >> programs >> texture >> uniform >> z_hint >> buffer
//	}

// -------------------------------------------------------------------------------------------------

void Queue::setClearColor(const libv::vec4f rgba) {
	sequencePoint();
	self->add<QueueTaskClearColor>(self->sequenceNumber, rgba);
}

void Queue::clearColor() {
	if (!self->tasks.empty() && std::holds_alternative<QueueTaskClear>(self->tasks.back())) {
		auto& buff = std::get<QueueTaskClear>(self->tasks.back()).buffers;
		buff = buff | libv::gl::BufferBit::Color;
	} else {
		sequencePoint();
		self->add<QueueTaskClear>(self->sequenceNumber, libv::gl::BufferBit::Color);
		sequencePoint();
	}
}

void Queue::clearDepth() {
	if (!self->tasks.empty() && std::holds_alternative<QueueTaskClear>(self->tasks.back())) {
		auto& buff = std::get<QueueTaskClear>(self->tasks.back()).buffers;
		buff = buff | libv::gl::BufferBit::Depth;
	} else {
		sequencePoint();
		self->add<QueueTaskClear>(self->sequenceNumber, libv::gl::BufferBit::Depth);
		sequencePoint();
	}
}

// -------------------------------------------------------------------------------------------------

void Queue::viewport(libv::vec2i position, libv::vec2i size) {
	self->add<QueueTaskViewport>(self->sequenceNumber, position, size);
}

// -------------------------------------------------------------------------------------------------

void Queue::framebuffer(Framebuffer framebuffer) {
	sequencePoint();
	self->add<QueueTaskFramebuffer>(self->sequenceNumber, std::move(framebuffer), QueueTaskFramebuffer::Mode::both);
	sequencePoint();
}

void Queue::framebuffer_draw(Framebuffer framebuffer) {
	sequencePoint();
	self->add<QueueTaskFramebuffer>(self->sequenceNumber, std::move(framebuffer), QueueTaskFramebuffer::Mode::draw);
	sequencePoint();
}

void Queue::framebuffer_read(Framebuffer framebuffer) {
	sequencePoint();
	self->add<QueueTaskFramebuffer>(self->sequenceNumber, std::move(framebuffer), QueueTaskFramebuffer::Mode::read);
	sequencePoint();
}

void Queue::framebuffer_deafult() {
	sequencePoint();
	self->add<QueueTaskFramebuffer>(self->sequenceNumber, QueueTaskFramebuffer::Mode::default_both);
	sequencePoint();
}

void Queue::framebuffer_draw_deafult() {
	sequencePoint();
	self->add<QueueTaskFramebuffer>(self->sequenceNumber, QueueTaskFramebuffer::Mode::default_draw);
	sequencePoint();
}

void Queue::framebuffer_read_deafult() {
	sequencePoint();
	self->add<QueueTaskFramebuffer>(self->sequenceNumber, QueueTaskFramebuffer::Mode::default_read);
	sequencePoint();
}

// -------------------------------------------------------------------------------------------------

void Queue::blit(Framebuffer src, Framebuffer dst, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	sequencePoint();
	self->add<QueueTaskBlit>(self->sequenceNumber, std::move(src), std::move(dst), src_pos, src_size, dst_pos, dst_size, mask, filter);
	sequencePoint();
}

void Queue::blit_to_default(Framebuffer src, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	sequencePoint();
	self->add<QueueTaskBlit>(self->sequenceNumber, std::move(src), std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
	sequencePoint();
}

void Queue::blit_from_default(Framebuffer dst, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	sequencePoint();
	self->add<QueueTaskBlit>(self->sequenceNumber, std::nullopt, std::move(dst), src_pos, src_size, dst_pos, dst_size, mask, filter);
	sequencePoint();
}

void Queue::blit_default(libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	sequencePoint();
	self->add<QueueTaskBlit>(self->sequenceNumber, std::nullopt, std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
	sequencePoint();
}

// -------------------------------------------------------------------------------------------------

void Queue::uniform(const Uniform_t<libv::gl::TextureChannel> uniform, libv::gl::TextureChannel value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<bool> uniform, bool value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<int32_t> uniform, int32_t value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<uint32_t> uniform, uint32_t value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<int64_t> uniform, int64_t value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<uint64_t> uniform, uint64_t value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<float> uniform, float value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<double> uniform, double value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec2b> uniform, libv::vec2b value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec3b> uniform, libv::vec3b value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec4b> uniform, libv::vec4b value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec2i> uniform, libv::vec2i value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec3i> uniform, libv::vec3i value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec4i> uniform, libv::vec4i value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec2ui> uniform, libv::vec2ui value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec3ui> uniform, libv::vec3ui value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec4ui> uniform, libv::vec4ui value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec2l> uniform, libv::vec2l value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec3l> uniform, const libv::vec3l& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec4l> uniform, const libv::vec4l& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec2ul> uniform, libv::vec2ul value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec3ul> uniform, const libv::vec3ul& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec4ul> uniform, const libv::vec4ul& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec2f> uniform, libv::vec2f value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec3f> uniform, libv::vec3f value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec4f> uniform, libv::vec4f value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec2d> uniform, libv::vec2d value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec3d> uniform, const libv::vec3d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::vec4d> uniform, const libv::vec4d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat2x2f> uniform, const libv::mat2x2f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat3x2f> uniform, const libv::mat3x2f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat4x2f> uniform, const libv::mat4x2f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat2x2d> uniform, const libv::mat2x2d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat3x2d> uniform, const libv::mat3x2d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat4x2d> uniform, const libv::mat4x2d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat2x3f> uniform, const libv::mat2x3f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat3x3f> uniform, const libv::mat3x3f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat4x3f> uniform, const libv::mat4x3f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat2x3d> uniform, const libv::mat2x3d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat3x3d> uniform, const libv::mat3x3d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat4x3d> uniform, const libv::mat4x3d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat2x4f> uniform, const libv::mat2x4f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat3x4f> uniform, const libv::mat3x4f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat4x4f> uniform, const libv::mat4x4f& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat2x4d> uniform, const libv::mat2x4d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat3x4d> uniform, const libv::mat3x4d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}
void Queue::uniform(const Uniform_t<libv::mat4x4d> uniform, const libv::mat4x4d& value) {
	self->programStack.top()->uniformStream.set(uniform.location, value);
}

// -------------------------------------------------------------------------------------------------

void Queue::uniform(UniformBlockUniqueView_std140&& view) {
	self->add<QueueTaskUniformBlock>(self->sequenceNumber, std::move(view));
}

void Queue::texture(Texture texture, const libv::gl::TextureChannel channel) {
	self->add<QueueTaskTexture>(self->sequenceNumber, std::move(texture), channel);
}

void Queue::program(Program program_) {
	self->programStack.top() = AttorneyProgramRemote::remote(program_);
}

void Queue::render(Mesh mesh) {
	self->programStack.top()->uniformStream.endBatch();
	self->add<QueueTaskMesh>(self->sequenceNumber, state.state(), self->programStack.top(), AttorneyMeshRemote::remote(mesh));
}

// -------------------------------------------------------------------------------------------------

void Queue::execute(libv::gl::GL& gl, Remote& remote) {
	for (const auto& task_variant : self->tasks) {
		auto execution = [&](const auto& task) {
			if constexpr (libv::similar_v<QueueTaskMesh, decltype(task)>)
				task.execute(gl, remote, self->currentState);
			else
				task.execute(gl, remote);
		};

		std::visit(execution, task_variant);
	}
	self->tasks.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
