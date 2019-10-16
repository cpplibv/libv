// File: program.hpp - Created on 2017.12.20. 11:27 - Author: Vader

#pragma once

// libv
#include <libv/gl/enum.hpp>
#include <libv/gl/gl_fwd.hpp>
#include <libv/gl/image_fwd.hpp>
#include <libv/gl/renderbuffer_object.hpp>
#include <libv/math/vec.hpp>
// std
#include <memory>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class AttorneyRemoteRenderbuffer;
class Remote;
class RemoteRenderbuffer;
template <auto Format>
struct Renderbuffer_t;

// -------------------------------------------------------------------------------------------------

struct RemoteRenderbufferHead {
	libv::gl::Renderbuffer renderbuffer;
	bool dirty = true;
};

class Renderbuffer {
	friend class AttorneyRemoteRenderbuffer;

private:
	using FS = libv::gl::FormatSized;
	using FC = libv::gl::FormatCompressed;
	using FD = libv::gl::FormatDepth;
	using FDS = libv::gl::FormatDepthStencil;

public:
	using R8          = Renderbuffer_t<FS::R8>;
	using R8_G8       = Renderbuffer_t<FS::RG8>;
	using R8_G8_B8    = Renderbuffer_t<FS::RGB8>;
	using R8_G8_B8_A8 = Renderbuffer_t<FS::RGBA8>;
	using R16F        = Renderbuffer_t<FS::R16F>;
	using R32F        = Renderbuffer_t<FS::R32F>;

	using D16         = Renderbuffer_t<FD::DEPTH_COMPONENT16>;
	using D24         = Renderbuffer_t<FD::DEPTH_COMPONENT24>;
	using D32         = Renderbuffer_t<FD::DEPTH_COMPONENT32>;
	using D32F        = Renderbuffer_t<FD::DEPTH_COMPONENT32F>;

	using D24_S8      = Renderbuffer_t<FDS::DEPTH24_STENCIL8>;

private:
	std::shared_ptr<RemoteRenderbuffer> remote;

public:
	Renderbuffer() noexcept;

public:
	void storage(libv::gl::Format format, libv::vec2i size) noexcept;
	void storage_ms(libv::gl::Format format, libv::vec2i size, int32_t samples) noexcept;

private:
	void sync(libv::gl::GL& gl, Remote& remote_) const noexcept;
};

class AttorneyRemoteRenderbuffer {
	friend class Queue;
	friend class RemoteFramebuffer;

	[[nodiscard]] static inline libv::gl::Renderbuffer sync(const Renderbuffer& renderbuffer, libv::gl::GL& gl, Remote& remote_) noexcept {
		// NOTE: Valid aliasing, verified by the standard, head is the first member
		auto& head = *static_cast<RemoteRenderbufferHead*>(static_cast<void*>(renderbuffer.remote.get()));
		if (head.dirty)
			renderbuffer.sync(gl, remote_);
		return head.renderbuffer;
	}
};

// -------------------------------------------------------------------------------------------------

template <auto Format>
struct Renderbuffer_t : public Renderbuffer {
	inline void storage(libv::vec2i size) noexcept {
		static_cast<Renderbuffer&>(*this).storage(Format, size);
	}

	inline void storage_ms(libv::vec2i size, int32_t samples) noexcept {
		static_cast<Renderbuffer&>(*this).storage_ms(Format, size, samples);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
