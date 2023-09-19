// Project: libv.gl, File: src/libv/gl/Fence.hpp

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/fence_object.hpp>


namespace libv {
namespace gl {

// Fence -------------------------------------------------------------------------------------------

class AccessFence {
private:
	Fence& object;

public:
	inline AccessFence(Fence& object) noexcept :
		object(object) { }

public:
	inline void create() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		object.id = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		checkGL();
	}

	[[nodiscard]] inline bool ready() const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		GLint value = 0;
		GLsizei count = 0;
		glGetSynciv(static_cast<GLsync>(object.id), GL_SYNC_STATUS, sizeof(GLint), &count, &value);
		checkGL();
		return value == GL_SIGNALED;
	}

	inline bool clientWait() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		while (true) {
			auto status = glClientWaitSync(static_cast<GLsync>(object.id), GL_SYNC_FLUSH_COMMANDS_BIT, 1000'000);
			checkGL();
			if (status == GL_ALREADY_SIGNALED)
				return true;
			if (status == GL_CONDITION_SATISFIED)
				return false;
			if (status == GL_TIMEOUT_EXPIRED)
				continue;
			return false;
		}
	}

	inline void wait() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glWaitSync(static_cast<GLsync>(object.id), 0, GL_TIMEOUT_IGNORED);
		checkGL();
	}

	inline void destroy() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteSync(static_cast<GLsync>(object.id));
		object.id = nullptr;
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
