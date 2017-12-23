// File:   texture.cpp Author: Vader Created on 2019. január 25., 4:40

// hpp
#include <libv/glr/texture.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/texture.hpp>
// pro
#include <libv/glr/remote.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

void RemoteTexture::create(libv::gl::GL& gl, Remote& remote_) noexcept {
	(void) gl;

	remote = libv::make_observer(remote_);
	created = true;
}

void RemoteTexture::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (!created) {
		create(gl, remote_);

		// TODO P5: Sort out this create mess, ideal solution would be to improve Image to accept a texture
		if (!dirty_image) {
			gl(texture).create();
			gl(texture).bind();
		}
	}

	if (dirty_image) {
		texture = image->createTexture();
		dirty_image = false;
	}
	dirty = false;
}

void RemoteTexture::bind(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (dirty)
		update(gl, remote_);
	else
		gl(texture).bind();
}

RemoteTexture::~RemoteTexture() noexcept {
	if (remote)
		remote->gc(texture);
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
