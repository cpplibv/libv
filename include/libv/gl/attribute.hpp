// File: program.hpp Author: Vader Created on 2016. július 8., 12:02

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/math/vec.hpp>
// std
#include <string>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/program_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct BaseAttributeCore {
protected:
	GLint location = 0;
public:
	inline auto id() const {
		return location;
	}
	inline operator GLint() const {
		return location;
	}
};

template <typename T>
struct BaseAttribute;

template <> struct BaseAttribute<bool> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 1;
};

template <> struct BaseAttribute<int> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 1;
};

template <> struct BaseAttribute<float> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::FLOAT);
	static constexpr GLint attributeSize = 1;
};

template <> struct BaseAttribute<double> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::DOUBLE);
	static constexpr GLint attributeSize = 1;
};

template <size_t N, typename T>
struct BaseAttribute<libv::vec_t<N, T>> : public BaseAttributeCore {
	static constexpr GLenum attributeType = BaseAttribute<T>::attributeType;
	static constexpr GLint attributeSize = N;
};

// AttributeLocation -------------------------------------------------------------------------------

template <typename T>
class AttributeLocation : public BaseAttribute<T> {
public:
	inline void assign(const Program& program, const char* name) {
		LIBV_GL_DEBUG_ASSERT(program.id != 0);
		this->location = glGetAttribLocation(program.id, name);
		checkGL();
	}
	inline void assign(const Program& program, const std::string& name) {
		assign(program.id, name.c_str());
	}
	AttributeLocation() = default;
	inline AttributeLocation(const Program& program, const char* name) {
		assign(program.id, name);
	}
	inline AttributeLocation(const Program& program, const std::string& name) {
		assign(program.id, name.c_str());
	}
};

// Attribute ---------------------------------------------------------------------------------------

template <typename T>
class AttributeFixLocation : public BaseAttribute<T> {
public:
	inline void assign(GLint location) {
		this->location = location;
	}
	inline AttributeFixLocation<T>& operator=(GLint location) & {
		this->location = location;
		return *this;
	}
	AttributeFixLocation() {}
	inline AttributeFixLocation(GLint location) {
		this->location = location;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
using Attribute = AttributeFixLocation<T>;

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
