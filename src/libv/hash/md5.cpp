// Project: libv.hash, File: src/libv/hash/md5.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/hash/md5.hpp>
// ext
#include <boost/uuid/detail/md5.hpp>
// libv
#include <libv/utility/byte_swap.hpp>
// std
#include <cstring>
#include <istream>
#include <ostream>
#include <iomanip>


namespace libv {
namespace hash {

// -------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const md5& md5) {
	std::ios_base::fmtflags f(os.flags());
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

	stream.seekg(0, std::ios::end);
	const auto size = static_cast<size_t>(stream.tellg());
	stream.seekg(0, std::ios::beg);

	const auto batch_count = size / batch_size;

	boost::uuids::detail::md5 hasher;

	for (size_t i = 0; i < batch_count; i++) {
		stream.read(buffer, batch_size);
		hasher.process_bytes(buffer, batch_size);
	}

	stream.read(buffer, size - batch_count * batch_size);
	hasher.process_bytes(buffer, size - batch_count * batch_size);

	boost::uuids::detail::md5::digest_type digest;
	hasher.get_digest(digest);

	return digest_to_md5(digest);
}


// -------------------------------------------------------------------------------------------------

} // namespace hash
} // namespace libv
