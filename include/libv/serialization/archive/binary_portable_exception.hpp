#pragma once

// ext
#include <boost/archive/basic_archive.hpp>
#include <boost/archive/archive_exception.hpp>
// std
#include <string>

namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

using signature = uint32_t;
constexpr signature portable_binary_signature = 0x5C5A073B;

constexpr unsigned no_infnan = 64; // flag for fp serialization

const boost::archive::library_version_type archive_version(boost::archive::BOOST_ARCHIVE_VERSION()); // version of the linked boost archive library


struct PortableBinaryArchiveException : boost::archive::archive_exception {
	std::string msg;
	PortableBinaryArchiveException(const std::string& msg) :
		boost::archive::archive_exception(other_exception),
		msg(msg) { };
	const char* what() const noexcept override {
		return msg.c_str();
	}
};

struct NegativUnsignedException : PortableBinaryArchiveException {
	NegativUnsignedException() :
		PortableBinaryArchiveException("Cannot read a negative number into an unsigned type") {}
};

struct IntegerSizeException : PortableBinaryArchiveException {
	IntegerSizeException(size_t size) :
		PortableBinaryArchiveException("Requested integer size exceeds type size: ") {
		msg += std::to_string(size);
	}
};

struct IllegalFloatException : PortableBinaryArchiveException {
	template <typename T>
	IllegalFloatException(T value) :
		PortableBinaryArchiveException("Serialization of illegal floating point value: ") {
		msg += std::to_string(value);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
