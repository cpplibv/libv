// Project: libv.vm4, File: src/libv/vm4/load.cpp

// hpp
#include <libv/vm4/load_direct_gpu.hpp>
// libv
#include <libv/gl/array_buffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/vertex_array.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/serial.hpp>
// std
#include <fstream>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4/model_serial.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

DirectLoadResult load_direct_gpu_or_throw(std::ifstream is) {
	DirectLoadResult result;

	{
		libv::archive::BinaryInput ar(is);
		ar(LIBV_NVP_FORCED("header", result.header));
	}

	libv::gl::ArrayBuffer buffer;
	libv::gl::GL gl;

	void* data = gl(buffer).mapRange(0, length, libv::gl::BufferAccess::Write | libv::gl::BufferAccess::InvalidateBuffer | libv::gl::BufferAccess::Unsynchronized);
	for (const auto& attrDesc : result.header.attributes) {
		int start = 0;
		int length = 0;
		int buffer_start = start++;

		is.read(static_cast<char*>(data), length);
	}
	gl(buffer).unmap();

	gl(result.vao).bind();
	for (const auto& attrDesc : result.header.attributes) {
		int stride = 0;
		int offset = start++;
		gl(result.vao).bindAttribute(buffer, attrDesc.attr, stride, offset);
	}
	gl(result.vao).bindElements(ibo);

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
