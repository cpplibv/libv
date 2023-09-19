// Project: libv.glr, File: src/libv/glr/queue.cpp

// hpp
#include <libv/glr/queue.hpp>
// libv
#include <libv/gl/gl.hpp>
// std
#include <optional>
#include <type_traits>
#include <variant>
//// pro
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/state.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

static inline void changeState(libv::GL& gl, State target) {
	gl.capability.blend.set(target.capabilityBlend != 0);
	gl.capability.cull.set(target.capabilityCullFace != 0);
	gl.capability.depthTest.set(target.capabilityDepthTest != 0);
	gl.capability.multisample.set(target.capabilityMultisample != 0);
	gl.capability.rasterizerDiscard.set(target.capabilityRasterizerDiscard != 0);
	gl.capability.scissorTest.set(target.capabilityScissorTest != 0);
	gl.capability.stencilTest.set(target.capabilityStencilTest != 0);
	gl.capability.textureCubeMapSeamless.set(target.capabilityTextureCubeMapSeamless != 0);

	gl.frontFace.set(target.frontFaceCCW == 0 ? libv::gl::FrontFace::CW : libv::gl::FrontFace::CCW);
	gl.cullFace.set(target.cullFace == 0 ? libv::gl::CullFace::Back : libv::gl::CullFace::Front);

	static constexpr std::array polygonModeTable{
			libv::gl::PolygonMode::Fill,
			libv::gl::PolygonMode::Line,
			libv::gl::PolygonMode::Point,
	};
	gl.polygonMode.set(polygonModeTable[target.polygonMode]);

	static constexpr std::array depthFunctionTable{
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

	static constexpr std::array blendFunctionTable{
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

// -------------------------------------------------------------------------------------------------

struct QueueTaskMesh {
	State state;
	std::shared_ptr<RemoteProgram> program;
	std::shared_ptr<RemoteMesh> mesh;

	inline void execute(libv::GL& gl, Remote& remote, State& currentState) const noexcept {
		if (currentState != state) {
			changeState(gl, state);
			currentState = state;
		}

		program->use(gl, remote);
		mesh->render(gl, remote);
	}
};

struct QueueTaskMeshVII : QueueTaskMesh {
	VertexIndex baseVertex;
	VertexIndex baseIndex;
	VertexIndex numIndices;

	inline void execute(libv::GL& gl, Remote& remote, State& currentState) const noexcept {
		if (currentState != state) {
			changeState(gl, state);
			currentState = state;
		}

		program->use(gl, remote);
		mesh->render(gl, remote, baseVertex, baseIndex, numIndices);
	}
};

struct QueueTaskMeshInstanced : QueueTaskMesh {
	int32_t instanceCount;

	inline void execute(libv::GL& gl, Remote& remote, State& currentState) const noexcept {
		if (currentState != state) {
			changeState(gl, state);
			currentState = state;
		}

		program->use(gl, remote);
		mesh->renderArraysInstanced(gl, remote, instanceCount);
	}
};

struct QueueTaskMeshFullScreen {
	State state;
	std::shared_ptr<RemoteProgram> program;

	inline void execute(libv::GL& gl, Remote& remote, State& currentState) const noexcept {
		if (currentState != state) {
			changeState(gl, state);
			currentState = state;
		}

		program->use(gl, remote);
		// TODO P1: a non zero VAO should be bound, check on it
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//		mesh->render(gl, remote);
	}
};

struct QueueTaskDispatchCompute {
	State state;
	std::shared_ptr<RemoteProgram> program;
	uint32_t group_size_x;
	uint32_t group_size_y;
	uint32_t group_size_z;

	inline void execute(libv::GL& gl, Remote& remote, State& currentState) const noexcept {
		if (currentState != state) {
			changeState(gl, state);
			currentState = state;
		}

		program->use(gl, remote);
		gl.dispatchCompute(group_size_x, group_size_y, group_size_z);
	}
};

struct QueueTaskMemoryBarrier {
	libv::gl::BarrierBit bits;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		(void) remote;
		gl.memoryBarrier(bits);
	}
};

struct QueueTaskClear {
	libv::gl::BufferBit buffers;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		// TODO P5: Color mask in not auto enabled, Stencil mask need verification
		(void) remote;

//		AttorneyRemoteFramebuffer::use_draw_buffer(*framebuffer_dst, gl, *attachment_dst);
//		AttorneyRemoteFramebuffer::reset_draw_buffer(*framebuffer_dst, gl, *attachment_dst);

		if ((buffers & libv::gl::BufferBit::DepthStencil) == libv::gl::BufferBit::DepthStencil) {
			auto cap_d = gl.capability.depthTest.enable_guard();
			auto cap_s = gl.capability.stencilTest.enable_guard();
			auto mask_d = gl.mask.depth.enable_guard();
			// auto mask_s = gl.mask.stencil.enable_guard();
			gl.clear(buffers);
		} else if ((buffers & libv::gl::BufferBit::Depth) == libv::gl::BufferBit::Depth) {
			auto cap_d = gl.capability.depthTest.enable_guard();
			auto mask_d = gl.mask.depth.enable_guard();
			gl.clear(buffers);
		} else if ((buffers & libv::gl::BufferBit::Stencil) == libv::gl::BufferBit::Stencil) {
			auto cap_s = gl.capability.stencilTest.enable_guard();
			// auto mask_s = gl.mask.stencil.enable_guard();
			gl.clear(buffers);
		} else {
			gl.clear(buffers);
		}
	}
};

struct QueueTaskCallback {
	State state;
	std::function<void(libv::GL&)> callback;

	inline void execute(libv::GL& gl, Remote& remote, State& currentState) const noexcept {
		if (currentState != state) {
			changeState(gl, state);
			currentState = state;
		}

		(void) remote;
		callback(gl);
	}
};

struct QueueTaskCallbackProgram {
	State state;
	std::function<void(libv::GL&)> callback;
	std::shared_ptr<RemoteProgram> program;

	inline void execute(libv::GL& gl, Remote& remote, State& currentState) const noexcept {
		if (currentState != state) {
			changeState(gl, state);
			currentState = state;
		}

		program->use(gl, remote);
		callback(gl);
	}
};

struct QueueTaskClearColor {
	libv::vec4f color;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		(void) remote;
		gl.clearColor(color);
	}
};

struct QueueTaskUniformBlockUnique {
	UniformBlockUniqueView_std140 view;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		view.remote->bind(gl, remote, view.binding, view.block);
	}
};

struct QueueTaskUniformBlockShared {
	UniformBlockSharedView_std140 view;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		view.remote()->bind(gl, remote, view.binding(), view.block());
	}
};

struct QueueTaskUniformBlockStream {
	UniformBlockStreamView_std140 view;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		view.remote->bind(gl, remote, view.binding, view.block);
	}
};

struct QueueTaskTexture {
	Texture texture;
	libv::gl::TextureUnit unit;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		// TODO P4: Could be optimized with gl addition: Only change activeTexture if binding is different
		//		gl.bind(channel, object); BUT sync_might_bind might interfere
		gl.activeTexture(unit);
		auto object = AttorneyRemoteTexture::sync_might_bind(texture, gl, remote);
		gl.bind(object);
	}
};

struct QueueTaskBindImageTexture {
	uint32_t unit;
	Texture texture;
	int32_t level;
	int32_t layer;
	libv::gl::BufferAccessFull access;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		auto object = AttorneyRemoteTexture::sync_might_bind(texture, gl, remote);
		if (layer < 0)
			gl.bindImageTexture(unit, object, level, access, texture.format());
		else
			gl.bindImageTexture(unit, object, level, layer, access, texture.format());
	}
};

struct QueueTaskViewport {
	libv::vec2i position;
	libv::vec2i size;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
		(void) remote;
		gl.viewport(position, size);
	}
};

struct QueueTaskFramebuffer {
	std::optional<Framebuffer> framebuffer;
	enum class Mode {
		both,
		draw,
		read,

		default_both,
		default_draw,
		default_read,
	} mode;

	QueueTaskFramebuffer(Framebuffer&& framebuffer, Mode mode) :
		framebuffer(std::move(framebuffer)), mode(mode) { }

	QueueTaskFramebuffer(Mode mode) :
		mode(mode) { }

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
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
	std::optional<Framebuffer> framebuffer_src;
	std::optional<Framebuffer> framebuffer_dst;
	std::optional<libv::gl::Attachment> attachment_src;
	std::optional<libv::gl::Attachment> attachment_dst;
	libv::vec2i src_pos;
	libv::vec2i src_size;
	libv::vec2i dst_pos;
	libv::vec2i dst_size;
	libv::gl::BufferBit mask;
	libv::gl::MagFilter filter;

	inline void execute(libv::GL& gl, Remote& remote) const noexcept {
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

		if (attachment_src && framebuffer_src)
			AttorneyRemoteFramebuffer::use_read_buffer(*framebuffer_src, gl, *attachment_src);
		if (attachment_dst && framebuffer_dst)
			AttorneyRemoteFramebuffer::use_draw_buffer(*framebuffer_dst, gl, *attachment_dst);

		gl.blit(src_pos, src_size, dst_pos, dst_size, mask, filter);

		if (attachment_src && framebuffer_src)
			AttorneyRemoteFramebuffer::reset_read_buffer(*framebuffer_src, gl);
		if (attachment_dst && framebuffer_dst)
			AttorneyRemoteFramebuffer::reset_draw_buffer(*framebuffer_dst, gl);

		gl.framebuffer_read(previous_read);
		gl.framebuffer_draw(previous_draw);
	}
};

// =================================================================================================

class ImplQueue {
public:
	std::stack<std::shared_ptr<RemoteProgram>, std::vector<std::shared_ptr<RemoteProgram>>> programStack;

	libv::vec2i current_viewport_position;
	libv::vec2i current_viewport_size;

	std::vector<std::variant<
			QueueTaskCallback,
			QueueTaskCallbackProgram,
			QueueTaskClear,
			QueueTaskClearColor,
			QueueTaskViewport,
			QueueTaskMesh,
			QueueTaskMeshVII,
			QueueTaskMeshInstanced,
			QueueTaskMeshFullScreen,
			QueueTaskUniformBlockUnique,
			QueueTaskUniformBlockShared,
			QueueTaskUniformBlockStream,
			QueueTaskTexture,
			QueueTaskBindImageTexture,
			QueueTaskDispatchCompute,
			QueueTaskMemoryBarrier,
			QueueTaskFramebuffer,
			QueueTaskBlit
	>> tasks;

	State currentState{};

	libv::GL& gl;

public:
	ImplQueue(gl::GL& gl) :
		gl(gl) {
		programStack.emplace();
		auto current_viewport = gl.getViewport();
		current_viewport_size = current_viewport.size;
		current_viewport_position = current_viewport.position;
	}

public:
	template <typename T, typename... Args>
	inline void add(Args&&... args) {
		tasks.emplace_back(std::in_place_type<T>, T{std::forward<Args>(args)...});
	}
};

// =================================================================================================

Queue::Queue(Remote& remote) :
	self(std::make_unique<ImplQueue>(remote.gl())) {
}

Queue::Queue(Queue&&) noexcept = default;

Queue::~Queue() {
}

// -------------------------------------------------------------------------------------------------

//	void Queue::sort() {
//		// Preferred sorting order would be:
//		// pass >> target >> capabilities >> programs >> texture >> uniform >> z_hint >> buffer
//	}

// -------------------------------------------------------------------------------------------------

void Queue::callback(std::function<void(libv::GL&)> func) {
	self->add<QueueTaskCallback>(state.state(), std::move(func));
}

void Queue::callbackProgram(std::function<void(libv::GL&)> func) {
	self->programStack.top()->uniformStream.endBatch();
	self->add<QueueTaskCallbackProgram>(state.state(), std::move(func), self->programStack.top());
}

libv::GL& Queue::out_of_order_gl() noexcept {
	return self->gl;
}

// -------------------------------------------------------------------------------------------------

void Queue::setClearColor(const libv::vec4f rgba) {
	self->add<QueueTaskClearColor>(rgba);
}

void Queue::clearColor() {
	if (auto* task = self->tasks.empty() ? nullptr : std::get_if<QueueTaskClear>(&self->tasks.back())) {
		task->buffers = task->buffers | libv::gl::BufferBit::Color;
	} else {
		self->add<QueueTaskClear>(libv::gl::BufferBit::Color);
	}
}

void Queue::clearColorDepth() {
	if (auto* task = self->tasks.empty() ? nullptr : std::get_if<QueueTaskClear>(&self->tasks.back())) {
		task->buffers = task->buffers | libv::gl::BufferBit::Color | libv::gl::BufferBit::Depth;
	} else {
		self->add<QueueTaskClear>(libv::gl::BufferBit::Color | libv::gl::BufferBit::Depth);
	}
}

void Queue::clearDepth() {
	if (auto* task = self->tasks.empty() ? nullptr : std::get_if<QueueTaskClear>(&self->tasks.back())) {
		task->buffers = task->buffers | libv::gl::BufferBit::Depth;
	} else {
		self->add<QueueTaskClear>(libv::gl::BufferBit::Depth);
	}
}

// -------------------------------------------------------------------------------------------------

void Queue::viewport(libv::vec2i position, libv::vec2i size) {
	self->current_viewport_position = position;
	self->current_viewport_size = size;

	if (auto* task = self->tasks.empty() ? nullptr : std::get_if<QueueTaskViewport>(&self->tasks.back())) {
		task->size = size;
		task->position = position;
	} else {
		self->add<QueueTaskViewport>(position, size);
	}
}

libv::vec2i Queue::viewport_position() const {
	return self->current_viewport_position;
}

libv::vec2i Queue::viewport_size() const {
	return self->current_viewport_size;
}

// -------------------------------------------------------------------------------------------------

void Queue::framebuffer(Framebuffer framebuffer) {
	self->add<QueueTaskFramebuffer>(std::move(framebuffer), QueueTaskFramebuffer::Mode::both);
}

void Queue::framebuffer_draw(Framebuffer framebuffer) {
	self->add<QueueTaskFramebuffer>(std::move(framebuffer), QueueTaskFramebuffer::Mode::draw);
}

void Queue::framebuffer_read(Framebuffer framebuffer) {
	self->add<QueueTaskFramebuffer>(std::move(framebuffer), QueueTaskFramebuffer::Mode::read);
}

void Queue::framebuffer_default() {
	self->add<QueueTaskFramebuffer>(QueueTaskFramebuffer::Mode::default_both);
}

void Queue::framebuffer_draw_default() {
	self->add<QueueTaskFramebuffer>(QueueTaskFramebuffer::Mode::default_draw);
}

void Queue::framebuffer_read_default() {
	self->add<QueueTaskFramebuffer>(QueueTaskFramebuffer::Mode::default_read);
}

// -------------------------------------------------------------------------------------------------

void Queue::blit(Framebuffer src, Framebuffer dst, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	self->add<QueueTaskBlit>(std::move(src), std::move(dst), std::nullopt, std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
}

void Queue::blit(Framebuffer src, Framebuffer dst, libv::gl::Attachment src_att, libv::vec2i src_pos, libv::vec2i src_size, libv::gl::Attachment dst_att, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	self->add<QueueTaskBlit>(std::move(src), std::move(dst), src_att, dst_att, src_pos, src_size, dst_pos, dst_size, mask, filter);
}

void Queue::blit_to_default(Framebuffer src, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	self->add<QueueTaskBlit>(std::move(src), std::nullopt, std::nullopt, std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
}

void Queue::blit_to_default(Framebuffer src, libv::gl::Attachment src_att, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	self->add<QueueTaskBlit>(std::move(src), std::nullopt, src_att, std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
}

void Queue::blit_from_default(Framebuffer dst, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	self->add<QueueTaskBlit>(std::nullopt, std::move(dst), std::nullopt, std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
}

void Queue::blit_default(libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter) {
	self->add<QueueTaskBlit>(std::nullopt, std::nullopt, std::nullopt, std::nullopt, src_pos, src_size, dst_pos, dst_size, mask, filter);
}

// -------------------------------------------------------------------------------------------------

void Queue::uniform(const Uniform_t<libv::gl::TextureUnit> uniform, libv::gl::TextureUnit value) {
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
	self->add<QueueTaskUniformBlockUnique>(std::move(view));
}

void Queue::uniform(UniformBlockSharedView_std140 view) {
	self->add<QueueTaskUniformBlockShared>(std::move(view));
}

void Queue::uniform(UniformBlockStreamView_std140 view) {
	self->add<QueueTaskUniformBlockStream>(std::move(view));
}

void Queue::texture(Texture texture, const libv::gl::TextureUnit channel) {
	self->add<QueueTaskTexture>(std::move(texture), channel);
}

void Queue::program(Program program_) {
	self->programStack.top() = AttorneyProgramRemote::remote(program_);
}

void Queue::render(Mesh mesh) {
	self->programStack.top()->uniformStream.endBatch();
	self->add<QueueTaskMesh>(state.state(), self->programStack.top(), AttorneyMeshRemote::remote(mesh));
}

void Queue::render(Mesh mesh, VertexIndex baseVertex, VertexIndex baseIndex, VertexIndex numIndices) {
	self->programStack.top()->uniformStream.endBatch();
	self->add<QueueTaskMeshVII>(state.state(), self->programStack.top(), AttorneyMeshRemote::remote(mesh), baseVertex, baseIndex, numIndices);
}

void Queue::render_instanced(Mesh mesh, int32_t count) {
	self->programStack.top()->uniformStream.endBatch();
	self->add<QueueTaskMeshInstanced>(state.state(), self->programStack.top(), AttorneyMeshRemote::remote(mesh), count);
}

void Queue::render_full_screen() {
	self->programStack.top()->uniformStream.endBatch();
	self->add<QueueTaskMeshFullScreen>(state.state(), self->programStack.top());
}

void Queue::bindImageTexture(uint32_t unit, Texture texture, int32_t level, libv::gl::BufferAccessFull access) {
	self->add<QueueTaskBindImageTexture>(unit, std::move(texture), level, -1, access);
}

void Queue::bindImageTexture(uint32_t unit, Texture texture, int32_t level, int32_t layer, libv::gl::BufferAccessFull access) {
	self->add<QueueTaskBindImageTexture>(unit, std::move(texture), level, layer, access);
}

void Queue::dispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) {
	self->programStack.top()->uniformStream.endBatch();
	self->add<QueueTaskDispatchCompute>(state.state(), self->programStack.top(), num_groups_x, num_groups_y, num_groups_z);
}

void Queue::memoryBarrier(libv::gl::BarrierBit bits) {
	self->add<QueueTaskMemoryBarrier>(bits);
}

// -------------------------------------------------------------------------------------------------

void Queue::execute(libv::GL& gl, Remote& remote) {
	for (const auto& task_variant : self->tasks) {
		auto execution = [&]<typename T>(const T& task) {
			if constexpr ( requires { task.execute(gl, remote, self->currentState); } )
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
