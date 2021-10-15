// Project: libv.gl, File: src/libv/gl/query.hpp

#pragma once

// ext
#include <GL/glew.h>
// std
#include <string_view>
// libv
#include <libv/utility/to_underlying.hpp>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/query_object.hpp>


namespace libv {
namespace gl {

// Query -------------------------------------------------------------------------------------------

class AccessQuery {
private:
	Query& object;

public:
	inline AccessQuery(Query& object) noexcept :
		object(object) { }

public:
	inline void create(QueryType type) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glGenQueries(1, &object.id);
		object.type = type;
		checkGL();
	}

	inline void destroy() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteQueries(1, &object.id);
		object.id = 0;
		checkGL();
	}

public:
	inline void begin() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBeginQuery(libv::to_underlying(object.type), object.id);
		checkGL();
	}

	inline void end() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glEndQuery(libv::to_underlying(object.type));
		checkGL();
	}

	[[nodiscard]] inline bool has_result() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		GLint has_result;
		glGetQueryObjectiv(object.id, GL_QUERY_RESULT_AVAILABLE, &has_result);
		checkGL();
		return has_result == GL_TRUE;
	}

	[[nodiscard]] inline int32_t result() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		GLint result;
		glGetQueryObjectiv(object.id, GL_QUERY_RESULT, &result);
		checkGL();
		return result;
	}

	//	OpenGL 4.4+ only, use has_result + result instead
	//	[[nodiscard]] inline int32_t result_no_wait() noexcept {
	//		LIBV_GL_DEBUG_ASSERT(object.id != 0);
	//		GLint result;
	//		glGetQueryObjectiv(object.id, GL_QUERY_RESULT_NO_WAIT, &result);
	//		checkGL();
	//		return result;
	//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
