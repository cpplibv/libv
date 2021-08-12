// Created by Vader on 2021.08.12..

// hpp
#include <libv/security/rsa_signature.hpp>
// ext
#include <openssl/pem.h>
#include <openssl/rsa.h>


namespace libv {
namespace security {
namespace {

// -------------------------------------------------------------------------------------------------

RSA* createPrivateRSA(const std::string_view key) {
	BIO* keybio = BIO_new_mem_buf(static_cast<const void*>(key.data()), static_cast<int>(key.size()));

	if (keybio == nullptr)
		return nullptr;

	RSA* rsa = nullptr;
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, nullptr, nullptr);

	BIO_free(keybio);
	return rsa;
}

RSA* createPublicRSA(const std::string_view key) {
	BIO* keybio = BIO_new_mem_buf(static_cast<const void*>(key.data()), static_cast<int>(key.size()));

	if (keybio == nullptr)
		return nullptr;

	RSA* rsa = nullptr;
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, nullptr, nullptr);

	BIO_free(keybio);
	return rsa;
}

} // namespace -------------------------------------------------------------------------------------

std::string rsa_sign_message(std::string_view message, std::string_view private_key) {
	std::string signature;

	RSA* key = createPrivateRSA(private_key);

	if (key == nullptr)
		return signature;

	EVP_PKEY* priKey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(priKey, key);

	EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();

	if (EVP_DigestSignInit(m_RSASignCtx, nullptr, EVP_sha256(), nullptr, priKey) <= 0) {
		EVP_MD_CTX_free(m_RSASignCtx);
		EVP_PKEY_free(priKey);
		return signature;
	}

	if (EVP_DigestSignUpdate(m_RSASignCtx, message.data(), message.size()) <= 0) {
		EVP_MD_CTX_free(m_RSASignCtx);
		EVP_PKEY_free(priKey);
		return signature;
	}

	size_t signature_length;
	if (EVP_DigestSignFinal(m_RSASignCtx, nullptr, &signature_length) <= 0) {
		EVP_MD_CTX_free(m_RSASignCtx);
		EVP_PKEY_free(priKey);
		return signature;
	}

	signature.resize(signature_length);
	if (EVP_DigestSignFinal(m_RSASignCtx, reinterpret_cast<unsigned char*>(signature.data()), &signature_length) <= 0) {
		EVP_MD_CTX_free(m_RSASignCtx);
		EVP_PKEY_free(priKey);
		return signature;
	}

	EVP_MD_CTX_free(m_RSASignCtx);
	EVP_PKEY_free(priKey);
	return signature;
}

bool rsa_verify_signature(std::string_view message, std::string_view public_key, std::string_view signature) {
	RSA* key = createPublicRSA(public_key);

	if (key == nullptr)
		return false;

	EVP_PKEY* pubKey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(pubKey, key);

	EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();

	if (EVP_DigestVerifyInit(m_RSAVerifyCtx, nullptr, EVP_sha256(), nullptr, pubKey) <= 0) {
		EVP_MD_CTX_free(m_RSAVerifyCtx);
		EVP_PKEY_free(pubKey);
		return false;
	}

	if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, message.data(), message.size()) <= 0) {
		EVP_MD_CTX_free(m_RSAVerifyCtx);
		EVP_PKEY_free(pubKey);
		return false;
	}

	int auth_status = EVP_DigestVerifyFinal(m_RSAVerifyCtx, reinterpret_cast<const unsigned char*>(signature.data()), signature.size());
	EVP_MD_CTX_free(m_RSAVerifyCtx);
	EVP_PKEY_free(pubKey);

	return auth_status == 1;
}

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv