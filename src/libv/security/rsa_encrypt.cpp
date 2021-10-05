// Project: libv.security, File: src/libv/security/rsa_signature.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/security/rsa_signature.hpp>
// ext
#include <openssl/rsa.h>
// pro
#include <libv/security/rsa.hxx>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

std::string rsa_encrypt_message(std::string_view message, std::string_view public_key) {
	std::string result;

	RSA* key = createPublicRSA(public_key);

	if (key == nullptr)
		return result;

	const auto padding = RSA_PKCS1_OAEP_PADDING; // This mode is recommended for all new applications.
	const auto padding_size = 42; // NOTE: 42 IS NOT A RANDOM NUMBER!: flen must be less than RSA_size(rsa) - 41 for RSA_PKCS1_OAEP_PADDING

	const auto block_size_out = RSA_size(key);
	const auto block_size_in = block_size_out - padding_size;
	const auto block_full_count = message.size() / block_size_in;
	const auto has_partial_block = block_full_count * block_size_in < message.size();
	const auto block_count = block_full_count + (has_partial_block ? 1 : 0);

	result.resize(block_count * block_size_out);

	auto src = reinterpret_cast<const unsigned char*>(message.data());
	auto dst = reinterpret_cast<unsigned char*>(result.data());
	auto it_src = size_t{0};
	auto it_dst = size_t{0};

	for (size_t i = 0 ; i < block_full_count ; ++i) {
		int result_code = RSA_public_encrypt(block_size_in, src + it_src, dst + it_dst, key, padding);
		if (result_code < 0) {
			result.clear();
			RSA_free(key);
			return result;
		}

		it_src += block_size_in;
		it_dst += block_size_out;
	}
	if (has_partial_block) {
		const auto amount = static_cast<int>(message.size() - it_src);
		int result_code = RSA_public_encrypt(amount, src + it_src, dst + it_dst, key, padding);
		if (result_code < 0) {
			result.clear();
			RSA_free(key);
			return result;
		}
	}

	RSA_free(key);
	return result;
}

std::string rsa_decrypt_message(std::string_view encrypted, std::string_view private_key) {
	std::string result;

	RSA* key = createPrivateRSA(private_key);

	if (key == nullptr)
		return result;

	const auto padding = RSA_PKCS1_OAEP_PADDING; // This mode is recommended for all new applications.
	const auto padding_size = 42; // NOTE: 42 IS NOT A RANDOM NUMBER!: flen must be less than RSA_size(rsa) - 41 for RSA_PKCS1_OAEP_PADDING

	const auto block_size_out = RSA_size(key);
	const auto block_size_in = block_size_out - padding_size;
	const auto block_count = encrypted.size() / block_size_in;

	result.resize(block_count * block_size_in);

	const auto src = reinterpret_cast<const unsigned char*>(encrypted.data());
	const auto dst = reinterpret_cast<unsigned char*>(result.data());
	auto it_src = size_t{0};
	auto it_dst = size_t{0};
	const auto end_src = encrypted.size();

	while (it_src < end_src) {
		int result_code = RSA_private_decrypt(block_size_out, src + it_src, dst + it_dst, key, padding);
		if (result_code < 0) {
			result.clear();
			RSA_free(key);
			return result;
		}

		it_src += block_size_out;
		it_dst += result_code;
	}

	result.erase(result.begin() + it_dst, result.end()); // Discard the unused end of the buffer

	RSA_free(key);
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
