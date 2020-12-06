// Project: libv.hash, File: src/libv/hash/md5.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/hash/md5.hpp>
// ext
#include <boost/uuid/detail/md5.hpp>
// libv
#include <libv/utility/byte_swap.hpp>
#include <libv/utility/concat.hpp>
// std
#include <cstring>
#include <fstream>
#include <iomanip>
#include <istream>
#include <ostream>


namespace libv {
namespace hash {

// -------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const md5& md5) {
	std::ios::fmtflags f(os.flags());
	os << std::hex << std::setfill('0') << std::uppercase;

	for (auto i : md5.value)
		os << std::setw(2) << static_cast<uint32_t>(i);

	os.flags(f);
	return os;
}

// -------------------------------------------------------------------------------------------------

static inline md5 digest_to_md5(const boost::uuids::detail::md5::digest_type& value) {
	md5 result;

	for (size_t i = 0; i < std::size(value); i++) {
		const auto temp = libv::host_to_network(value[i]);
		std::memcpy(&result.value[i * sizeof(temp)], &temp, sizeof(temp));
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

md5 hash_md5(const std::string_view data) {
	boost::uuids::detail::md5 hasher;
	hasher.process_bytes(data.data(), data.size());

	boost::uuids::detail::md5::digest_type digest;
	hasher.get_digest(digest);

	return digest_to_md5(digest);
}

md5 hash_md5(const std::span<const std::byte> data) {
	return hash_md5(std::string_view(reinterpret_cast<const char*>(data.data()), data.size()));
}

md5 hash_md5(std::istream& stream) {
	static constexpr size_t batch_size = 16 * 1024;
	char buffer[batch_size];

	boost::uuids::detail::md5 hasher;

	while(!stream.eof()) {
		stream.read(buffer, batch_size);
		hasher.process_bytes(buffer, stream.gcount());
	}

	// Clear fail bit as we intentionally tried to over-read
	if (stream.rdstate() == (std::istream::eofbit | std::istream::failbit))
		stream.clear(std::istream::eofbit);

	boost::uuids::detail::md5::digest_type digest;
	hasher.get_digest(digest);

	return digest_to_md5(digest);
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] md5 hash_file_md5_or_throw(const std::filesystem::path& filepath) {
	std::ifstream file(filepath, std::ios::binary | std::ios::in);

	if (!file)
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to open file: ", filepath.generic_string()));

	auto result = hash_md5(file);

	if (file.fail())
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to read file: ", filepath.generic_string()));

	return result;
}

[[nodiscard]] md5 hash_file_md5(const std::filesystem::path& filepath, std::error_code& ec) {
	md5 result;
	std::ifstream file(filepath, std::ios::binary | std::ios::in);

	if (!file) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		return result;
	}

	result = hash_md5(file);

	if (file.fail()) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		return result;
	}

	ec.clear();
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace hash
} // namespace libv
