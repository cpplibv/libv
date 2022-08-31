// Project: libv.vm4, File: src/libv/vm4/load.hpp

#pragma once

// libv
#include <libv/gl/vertex_array_object.hpp>
// std
//#include <span>
//#include <string_view>
#include <iosfwd>
// pro
#include <libv/vm4/fwd.hpp>
#include <libv/vm4/model.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

struct AttributeDesc {

};

struct ModelHeader {
	std::vector<AttributeDesc> attributes;
};

struct DirectLoadResult {
//	ModelDescription model;
//	ModelHeader model;
	ModelHeader header;
	Model model;
	libv::gl::VertexArray vao;
};

[[nodiscard]] DirectLoadResult load_direct_gpu_or_throw(std::ifstream& data);

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
