// File: framebuffer.hpp - Created on 2017.12.20. 11:27 - Author: Vader

#pragma once

// libv
#include <libv/gl/enum.hpp>
#include <libv/gl/gl_fwd.hpp>
// std
#include <memory>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class Remote;
class RemoteFramebuffer;
class Renderbuffer;
class Texture;

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
	void attach2D(libv::gl::Attachment attachment, Texture texture, libv::gl::CubeSide side, int32_t level = 0) noexcept;
	void attach3D(libv::gl::Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept;
};

struct AttorneyRemoteFramebuffer {
	static void bind(const Framebuffer& framebuffer, libv::gl::GL& gl, Remote& remote_) noexcept;
	static void bind_draw(const Framebuffer& framebuffer, libv::gl::GL& gl, Remote& remote_) noexcept;
	static void bind_read(const Framebuffer& framebuffer, libv::gl::GL& gl, Remote& remote_) noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
