// Project: libv.security, File: src/libv/security/hash_sha256.cpp

// hpp
#include <libv/security/hash_sha256.hpp>
// ext
#include <openssl/sha.h>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

SHA256 hash_sha256(std::span<const std::byte> data) {
	SHA256 result;

	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, data.data(), data.size());
	SHA256_Final(reinterpret_cast<unsigned char*>(&result.value), &sha256);

	return result;
}

SHA256 hash_sha256(std::string_view data) {
	return hash_sha256(std::as_bytes(std::span(data)));
}

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
