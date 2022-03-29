// Project: libv.glr, File: src/libv/glr/renderbuffer.cpp

// hpp
#include <libv/glr/renderbuffer.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/renderbuffer.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
// pro
#include <libv/glr/destroy_queue.hpp>
#include <libv/glr/remote.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct RemoteRenderbuffer {
	RemoteRenderbufferHead head; // NOTE: head must be the first member to support inline aliasing access

	libv::gl::Format format;
	libv::vec2i size;
	int32_t samples = 0;

	libv::observer_ptr<DestroyQueues> remote = nullptr;

public:
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	~RemoteRenderbuffer() noexcept;
};

// -------------------------------------------------------------------------------------------------

void RemoteRenderbuffer::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (remote == nullptr) {
		remote = make_observer_ptr(&remote_.destroyQueues());

		gl(head.renderbuffer).create();
	}

	gl(head.renderbuffer).bind();
	if (samples == 0)
		gl(head.renderbuffer).storage(format, size);
	else
		gl(head.renderbuffer).storage_ms(format, size, samples);

	head.dirty = false;
}

RemoteRenderbuffer::~RemoteRenderbuffer() noexcept {
	if (remote)
		remote->gc(head.renderbuffer);
}

// -------------------------------------------------------------------------------------------------

Renderbuffer::Renderbuffer() noexcept :
	remote(std::make_shared<RemoteRenderbuffer>()) {
}

void Renderbuffer::storage(libv::gl::Format format, libv::vec2i size) noexcept {
	remote->head.dirty = true;
	remote->format = format;
	remote->size = size;
	remote->samples = 0;
}

void Renderbuffer::storage_ms(libv::gl::Format format, libv::vec2i size, int32_t samples) noexcept {
	remote->head.dirty = true;
	remote->format = format;
	remote->size = size;
	remote->samples = samples;
}

void Renderbuffer::sync(libv::gl::GL& gl, Remote& remote_) const noexcept {
	remote->update(gl, remote_);
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
