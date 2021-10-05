// Project: libv.security, File: src/libv/security/rsa_signature.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/security/rsa_signature.hpp>
// ext
#include <openssl/rsa.h>
// pro
#include <libv/security/rsa_key.hxx>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

// TODO P5: Add logging or exceptions to failures
//			#include <openssl/err.h>
//			ERR_get_error
//			ERR_error_string
//			ERR_print_errors_cb(+[](const char *str, size_t len, void *u) { std::cout << str << std::endl; return 0; }, nullptr);

// Reference:
// https://www.openssl.org/docs/man1.1.1/man3/RSA_private_encrypt.html
// https://www.openssl.org/docs/man1.1.1/man3/RSA_public_encrypt.html

template <auto key_func, auto encrypt_func, bool use_pkcs1_oaep>
std::string aux_rsa_encrypt(std::string_view message, std::string_view key_string) {
	std::string result;

	RSA* key = key_func(key_string);

	if (key == nullptr)
		return result;

	const auto padding = use_pkcs1_oaep ? RSA_PKCS1_OAEP_PADDING : RSA_PKCS1_PADDING; // This mode is recommended for all new applications.
	const auto padding_size = use_pkcs1_oaep ? 42 : RSA_PKCS1_PADDING_SIZE; // NOTE: 42 IS NOT A RANDOM NUMBER!: flen must be less than RSA_size(rsa) - 41 for RSA_PKCS1_OAEP_PADDING

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
		int result_code = encrypt_func(block_size_in, src + it_src, dst + it_dst, key, padding);
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
		int result_code = encrypt_func(amount, src + it_src, dst + it_dst, key, padding);
		if (result_code < 0) {
			result.clear();
			RSA_free(key);
			return result;
		}
	}

	RSA_free(key);
	return result;
}

template <auto key_func, auto decrypt_func, bool use_pkcs1_oaep>
std::string aux_rsa_decrypt(std::string_view encrypted, std::string_view key_string) {
	std::string result;

	RSA* key = key_func(key_string);

	if (key == nullptr)
		return result;

	const auto padding = use_pkcs1_oaep ? RSA_PKCS1_OAEP_PADDING : RSA_PKCS1_PADDING; // This mode is recommended for all new applications.
	const auto padding_size = use_pkcs1_oaep ? 42 : RSA_PKCS1_PADDING_SIZE; // NOTE: 42 IS NOT A RANDOM NUMBER!: flen must be less than RSA_size(rsa) - 41 for RSA_PKCS1_OAEP_PADDING

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
		int result_code = decrypt_func(block_size_out, src + it_src, dst + it_dst, key, padding);
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

std::string rsa_encrypt_public(std::string_view message, std::string_view public_key) {
	return aux_rsa_encrypt<createPublicRSA, RSA_public_encrypt, true>(message, public_key);
}

std::string rsa_decrypt_private(std::string_view encrypted, std::string_view private_key) {
	return aux_rsa_decrypt<createPrivateRSA, RSA_private_decrypt, true>(encrypted, private_key);
}

std::string rsa_encrypt_private(std::string_view message, std::string_view private_key) {
	return aux_rsa_encrypt<createPrivateRSA, RSA_private_encrypt, false>(message, private_key);
}

std::string rsa_decrypt_public(std::string_view encrypted, std::string_view public_key) {
	return aux_rsa_decrypt<createPublicRSA, RSA_public_decrypt, false>(encrypted, public_key);
}

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
