// Created by Vader on 2021.11.01..

#pragma once

// ext
#include <openssl/rsa.h>
#include <openssl/pem.h>
// std
//#include <stdio.h>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

bool generate_key(int bits) {
	int ret = 0;
	RSA* r = nullptr;
	BIGNUM* bne = nullptr;
	BIO* bp_public = nullptr;
	BIO* bp_private = nullptr;

	unsigned long e = RSA_F4;

	// 1. generate rsa key
	bne = BN_new();
	ret = BN_set_word(bne, e);
	if (ret != 1) {
		goto free_all;
	}

	r = RSA_new();
	ret = RSA_generate_key_ex(r, bits, bne, nullptr);
	if (ret != 1) {
		goto free_all;
	}

	// 2. save public key
	bp_public = BIO_new_file("public.pem", "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	if (ret != 1) {
		goto free_all;
	}

	// 3. save private key
	bp_private = BIO_new_file("private.pem", "w+");
	ret = PEM_write_bio_RSAPrivateKey(bp_private, r, nullptr, nullptr, 0, nullptr, nullptr);

	// 4. free
	free_all:

	BIO_free_all(bp_public);
	BIO_free_all(bp_private);
	RSA_free(r);
	BN_free(bne);

	return (ret == 1);
}

void foo() {
	generate_key(2048);
}

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
