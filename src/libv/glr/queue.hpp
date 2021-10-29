// Project: libv.glr, File: src/libv/glr/queue.hpp

#pragma once

// libv
#include <libv/gl/enum.hpp>
#include <libv/gl/gl_fwd.hpp>
#include <libv/gl/matrix_stack.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
// std
#include <functional>
#include <memory>
// pro
#include <libv/glr/state.hpp>
#include <libv/glr/uniform.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class Framebuffer;
class ImplQueue;
class Mesh;
class Program;
class Remote;
class Texture;
class UniformBlockStreamView_std140;
class UniformBlockUniqueView_std140;

struct Queue {
public:
	StateStack state;
	libv::gl::MatrixStack<libv::mat4f> model;
	libv::gl::MatrixStack<libv::mat4f> view;
	libv::gl::MatrixStack<libv::mat4f> projection;

private:
	std::unique_ptr<ImplQueue> self;

public:
	Queue(Remote& remote);
	Queue(Queue&&) noexcept;
	~Queue();

public:
	[[nodiscard]] inline libv::mat4f mvp() const noexcept {
		return projection * view * model;
	}

	[[nodiscard]] inline libv::vec3f eye() const noexcept {
		return libv::vec::xyz(view.top().inverse_copy()[3]);
	}

public:
	void sequencePoint() noexcept; /// Registers a sequence point as a sorting fence

public:
	void callback(std::function<void(libv::gl::GL&)>);
	libv::gl::GL& out_of_order_gl() noexcept;

public:
	inline void setClearColor(const float r, const float g, const float b, const float a) {
		setClearColor(libv::vec4f{r, g, b, a});
	}
	void setClearColor(const libv::vec4f rgba);

	void clearColor();
	void clearDepth();

public:
	// void line_width(float width); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLineWidth.xhtml
	// void point_size(float size); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPointSize.xhtml

public:
	void viewport(libv::vec2i position, libv::vec2i size);
	libv::vec2i viewport_position() const;
	libv::vec2i viewport_size() const;

public:
	void framebuffer(Framebuffer framebuffer);
	void framebuffer_draw(Framebuffer framebuffer);
	void framebuffer_read(Framebuffer framebuffer);
	void framebuffer_default();
	void framebuffer_draw_default();
	void framebuffer_read_default();

public:
	void blit(Framebuffer src, Framebuffer dst, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter);
	void blit_to_default(Framebuffer src, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter);
	void blit_from_default(Framebuffer dst, libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter);
	void blit_default(libv::vec2i src_pos, libv::vec2i src_size, libv::vec2i dst_pos, libv::vec2i dst_size, libv::gl::BufferBit mask, libv::gl::MagFilter filter);

public:
	void uniform(const Uniform_t<libv::gl::TextureChannel> uniform, libv::gl::TextureChannel value);
	void uniform(const Uniform_t<bool> uniform, bool value);
	void uniform(const Uniform_t<int32_t> uniform, int32_t value);
	void uniform(const Uniform_t<uint32_t> uniform, uint32_t value);
	void uniform(const Uniform_t<int64_t> uniform, int64_t value);
	void uniform(const Uniform_t<uint64_t> uniform, uint64_t value);
	void uniform(const Uniform_t<float> uniform, float value);
	void uniform(const Uniform_t<double> uniform, double value);
	void uniform(const Uniform_t<libv::vec2b> uniform, libv::vec2b value);
	void uniform(const Uniform_t<libv::vec3b> uniform, libv::vec3b value);
	void uniform(const Uniform_t<libv::vec4b> uniform, libv::vec4b value);
	void uniform(const Uniform_t<libv::vec2i> uniform, libv::vec2i value);
	void uniform(const Uniform_t<libv::vec3i> uniform, libv::vec3i value);
	void uniform(const Uniform_t<libv::vec4i> uniform, libv::vec4i value);
	void uniform(const Uniform_t<libv::vec2ui> uniform, libv::vec2ui value);
	void uniform(const Uniform_t<libv::vec3ui> uniform, libv::vec3ui value);
	void uniform(const Uniform_t<libv::vec4ui> uniform, libv::vec4ui value);
	void uniform(const Uniform_t<libv::vec2l> uniform, libv::vec2l value);
	void uniform(const Uniform_t<libv::vec3l> uniform, const libv::vec3l& value);
	void uniform(const Uniform_t<libv::vec4l> uniform, const libv::vec4l& value);
	void uniform(const Uniform_t<libv::vec2ul> uniform, libv::vec2ul value);
	void uniform(const Uniform_t<libv::vec3ul> uniform, const libv::vec3ul& value);
	void uniform(const Uniform_t<libv::vec4ul> uniform, const libv::vec4ul& value);
	void uniform(const Uniform_t<libv::vec2f> uniform, libv::vec2f value);
	void uniform(const Uniform_t<libv::vec3f> uniform, libv::vec3f value);
	void uniform(const Uniform_t<libv::vec4f> uniform, libv::vec4f value);
	void uniform(const Uniform_t<libv::vec2d> uniform, libv::vec2d value);
	void uniform(const Uniform_t<libv::vec3d> uniform, const libv::vec3d& value);
	void uniform(const Uniform_t<libv::vec4d> uniform, const libv::vec4d& value);
	void uniform(const Uniform_t<libv::mat2x2f> uniform, const libv::mat2x2f& value);
	void uniform(const Uniform_t<libv::mat3x2f> uniform, const libv::mat3x2f& value);
	void uniform(const Uniform_t<libv::mat4x2f> uniform, const libv::mat4x2f& value);
	void uniform(const Uniform_t<libv::mat2x2d> uniform, const libv::mat2x2d& value);
	void uniform(const Uniform_t<libv::mat3x2d> uniform, const libv::mat3x2d& value);
	void uniform(const Uniform_t<libv::mat4x2d> uniform, const libv::mat4x2d& value);
	void uniform(const Uniform_t<libv::mat2x3f> uniform, const libv::mat2x3f& value);
	void uniform(const Uniform_t<libv::mat3x3f> uniform, const libv::mat3x3f& value);
	void uniform(const Uniform_t<libv::mat4x3f> uniform, const libv::mat4x3f& value);
	void uniform(const Uniform_t<libv::mat2x3d> uniform, const libv::mat2x3d& value);
	void uniform(const Uniform_t<libv::mat3x3d> uniform, const libv::mat3x3d& value);
	void uniform(const Uniform_t<libv::mat4x3d> uniform, const libv::mat4x3d& value);
	void uniform(const Uniform_t<libv::mat2x4f> uniform, const libv::mat2x4f& value);
	void uniform(const Uniform_t<libv::mat3x4f> uniform, const libv::mat3x4f& value);
	void uniform(const Uniform_t<libv::mat4x4f> uniform, const libv::mat4x4f& value);
	void uniform(const Uniform_t<libv::mat2x4d> uniform, const libv::mat2x4d& value);
	void uniform(const Uniform_t<libv::mat3x4d> uniform, const libv::mat3x4d& value);
	void uniform(const Uniform_t<libv::mat4x4d> uniform, const libv::mat4x4d& value);

	void uniform(UniformBlockStreamView_std140 view);
	void uniform(UniformBlockUniqueView_std140&& view);
	void texture(Texture texture, const libv::gl::TextureChannel channel);
//	void pop_program(Program program) {}
//	void push_program() {}
//	void push_program(Program program) {}
//	void push_program_guard() {}
//	void push_program_guard(Program program) {}
	void program(Program program_);
	void render(Mesh mesh);
	void render(Mesh mesh, uint32_t baseVertex, uint32_t baseIndex, uint32_t numIndices);
	void render_full_screen();

public:
	void execute(libv::gl::GL& gl, Remote& remote);
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
