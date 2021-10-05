// Project: libv.security, File: src/libv/security/rsa_signature.cpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <openssl/pem.h>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

template <bool is_public_key>
RSA* aux_create_rsa_key(const std::string_view key) {
	BIO* keybio = BIO_new_mem_buf(static_cast<const void*>(key.data()), static_cast<int>(key.size()));

	if (keybio == nullptr)
		return nullptr;

	RSA* rsa = nullptr;
	rsa = is_public_key ?
			PEM_read_bio_RSA_PUBKEY(keybio, &rsa, nullptr, nullptr) :
			PEM_read_bio_RSAPrivateKey(keybio, &rsa, nullptr, nullptr);

	BIO_free(keybio);
	return rsa;
}

inline RSA* createPrivateRSA(std::string_view private_key) {
	return aux_create_rsa_key<false>(private_key);
}

inline RSA* createPublicRSA(std::string_view public_key) {
	return aux_create_rsa_key<true>(public_key);
}

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
