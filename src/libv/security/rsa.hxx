// Project: libv.security, File: src/libv/security/rsa_signature.cpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <openssl/pem.h>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

inline RSA* createPrivateRSA(const std::string_view key) {
	BIO* keybio = BIO_new_mem_buf(static_cast<const void*>(key.data()), static_cast<int>(key.size()));

	if (keybio == nullptr)
		return nullptr;

	RSA* rsa = nullptr;
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, nullptr, nullptr);

	BIO_free(keybio);
	return rsa;
}

inline RSA* createPublicRSA(const std::string_view key) {
	BIO* keybio = BIO_new_mem_buf(static_cast<const void*>(key.data()), static_cast<int>(key.size()));

	if (keybio == nullptr)
		return nullptr;

	RSA* rsa = nullptr;
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, nullptr, nullptr);

	BIO_free(keybio);
	return rsa;
}

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
