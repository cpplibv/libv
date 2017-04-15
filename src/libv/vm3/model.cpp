// File: Model.cpp, Created on 2015. január 17. 7:21, Author: Vader

// hpp
#include <libv/vm3/serialization/model.hpp>
// ext
#include <libv/serialization/archive/portable_iarchive.hpp>
#include <libv/serialization/archive/portable_oarchive.hpp>
// std
#include <iostream>

namespace libv {
namespace vm3 {

Model::Model() : name("--unknown--") { }

struct MemoryBuffer : std::streambuf {
	MemoryBuffer(char* begin, char* end) {
		this->setg(begin, begin, end);
	}

	MemoryBuffer(char* begin, const size_t size) {
		this->setg(begin, begin, begin + size);
	}
};

bool Model::load(const char* data, const size_t size) {
	MemoryBuffer buff(const_cast<char*>(data), size);
	std::istream is(&buff);

	// TODO P4: VM3_MODEL_MAGIC_HEADER with version number
	//	char magicBtye[sizeof (VM3_MODEL_MAGIC_BYTE)];
	//	const auto streamPosition = is.tellg();
	//	is.read(magicBtye, sizeof (magicBtye));
	//
	//	if (std::strcmp(magicBtye, VM3_MODEL_MAGIC_BYTE) != 0) {
	//		is.seekg(streamPosition);
	//		return false;
	//	}

	libv::archive::portable_iarchive ar(is);
	ar >> LIBV_NVP_NAMED("model", *this);
	return true; //<<< Model load fail
}

bool Model::save(std::ostream& os) const {
	// TODO P4: VM3_MODEL_MAGIC_HEADER with version number
	//	os << VM3_MODEL_MAGIC_HEADER;
	libv::archive::portable_oarchive ar(os);
	ar << LIBV_NVP_NAMED("model", *this);

	return true; //<<< Model save fail
}

bool Model::save(std::ostream&& os) const {
	return save(os);
}

} // namespace gl
} // namespace vlib
