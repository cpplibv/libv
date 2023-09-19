// Project: libv.glr, File: src/libv/glr/framebuffer.hpp

#pragma once

// libv
#include <libv/gl/enum.hpp>
#include <libv/gl/fwd.hpp>
// std
#include <memory>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class Remote;
class RemoteFramebuffer;
class Renderbuffer;
class Texture;

// -------------------------------------------------------------------------------------------------

//struct RemoteFramebufferHead {
//	libv::gl::Framebuffer framebuffer;
//	bool dirty = true;
//};

struct Framebuffer {
	friend class AttorneyRemoteFramebuffer;

private:
	// TODO P2: switch to intrusive pointer
	std::shared_ptr<RemoteFramebuffer> remote;

public:
	Framebuffer() noexcept;

public:
	void attach(libv::gl::Attachment attachment, Renderbuffer renderbuffer) noexcept;
	void attach1D(libv::gl::Attachment attachment, Texture texture, int32_t level = 0) noexcept;
	void attach2D(libv::gl::Attachment attachment, Texture texture, int32_t level = 0) noexcept;
	void attach2D(libv::gl::Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept;
	void attach2D(libv::gl::Attachment attachment, Texture texture, libv::gl::CubeSide side, int32_t level = 0) noexcept;
	void attach3D(libv::gl::Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept;

public:
	libv::gl::Framebuffer& out_of_order_gl();
};

struct AttorneyRemoteFramebuffer {
	static void bind(const Framebuffer& framebuffer, libv::GL& gl, Remote& remote_) noexcept;
	static void bind_draw(const Framebuffer& framebuffer, libv::GL& gl, Remote& remote_) noexcept;
	static void bind_read(const Framebuffer& framebuffer, libv::GL& gl, Remote& remote_) noexcept;

	static void use_draw_buffer(const Framebuffer& framebuffer, libv::GL& gl, libv::gl::Attachment attachment) noexcept;
	static void use_read_buffer(const Framebuffer& framebuffer, libv::GL& gl, libv::gl::Attachment attachment) noexcept;
	static void reset_draw_buffer(const Framebuffer& framebuffer, libv::GL& gl) noexcept;
	static void reset_read_buffer(const Framebuffer& framebuffer, libv::GL& gl) noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
