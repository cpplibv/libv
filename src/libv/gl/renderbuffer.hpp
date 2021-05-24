// Project: libv.gl, File: src/libv/gl/renderbuffer.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
// ext
#include <GL/glew.h>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/renderbuffer_object.hpp>
#include <libv/gl/texture_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class AccessRenderbuffer {
private:
	Renderbuffer& object;

public:
	AccessRenderbuffer(Renderbuffer& object) :
		object(object) { }

public:
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glGenRenderbuffers(1, &object.id);
		checkGL();
	}

	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteRenderbuffers(1, &object.id);
		object.id = 0;
		checkGL();
	}

public:
	inline void bind() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
	    glBindRenderbuffer(GL_RENDERBUFFER, object.id);
		checkGL();
	}

	inline void unbind() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
	    glBindRenderbuffer(GL_RENDERBUFFER, 0);
		checkGL();
	}

	[[nodiscard]] inline auto bind_guard() noexcept {
		return bind(), libv::guard([this] { unbind(); });
	}

public:
	inline void storage(Format format, libv::vec2i size) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glRenderbufferStorage(GL_RENDERBUFFER, format.format, size.x, size.y);
		checkGL();
	}

	inline void storage_ms(Format format, libv::vec2i size, int32_t samples) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format.format, size.x, size.y);
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
