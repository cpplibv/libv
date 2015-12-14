// File: Log.cpp, Created on 2015. február 2. 5:12, Author: Vader

// hpp
#include <libv/gl/log.hpp>
// ext
#include <GL/glew.h>

namespace libv {
	namespace gl {
		namespace detail {

			void logOGLError(const char* func, const char* file, int line) {
				GLenum err = glGetError();
				if (err != GL_NO_ERROR) {
					libv::gl::log().log(VLOG_SEVERITY_ERROR, std::string("OpenGL: ") + reinterpret_cast<const char*> (gluErrorString(err)), file, func, line);
				}
			}
		} //namespace detail
	} //namespace gl
} //namespace libv