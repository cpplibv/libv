// File: model.hpp, Created on 2014. november 28. 18:38, Author: Vader

#pragma once

// vl
#include <vl/vm3/model.hpp>
// std
#include <atomic>
#include <string>
#include <memory>
#include <vector>
// pro
#include <vl/gl/gl.hpp>
#include <vl/gl/uniform.hpp>

// -------------------------------------------------------------------------------------------------

namespace boost {
namespace filesystem {
struct path;
} //namespace filesystem
namespace asio {
struct const_buffer;
} //namespace asio
} //namespace boost

// -------------------------------------------------------------------------------------------------

namespace vl {
namespace gl {

// -------------------------------------------------------------------------------------------------

constexpr const char DEFAULT_MODEL_NAME[] = "--UNNAMED--";

// -------------------------------------------------------------------------------------------------

class Model {
private:
	uint32_t vao;
	uint32_t vbo_vertex;
	uint32_t vbo_index;

private:
	vl::vm3::Model model;
	std::string name;

private:
//	vl::gl::Uniform<glm::mat4> uniformMVPmat;
//	vl::gl::Uniform<glm::mat4> uniformMmat;

public:
	Model(const boost::asio::const_buffer& data, const std::string& name = DEFAULT_MODEL_NAME);
	Model(const boost::filesystem::path& filePath);
	Model(const boost::filesystem::path& filePath, const std::string& name);
	Model(const char* data, const size_t size, const std::string& name = DEFAULT_MODEL_NAME);

	void init(const char* data, const size_t size);

	virtual ~Model();

private:
	void loadGL();
	void unloadGL();

public:
	inline const std::string& getName() const {
		return name;
	}
	void render(vl::gl::GL&);
	void renderNode(uint32_t id, vl::gl::GL&);
	bool loaded();
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl
