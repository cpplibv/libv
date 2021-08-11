//

// ext
#include <openssl/pem.h>
#include <openssl/rsa.h>
// libv
#include <libv/utility/hex_dump.hpp>
// std
#include <iostream>
#include <string>
#include <string_view>


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

std::string signMessage(std::string_view private_key, std::string_view message) {
	std::string signature;

	EVP_PKEY* priKey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(priKey, createPrivateRSA(private_key));

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

bool verifySignature(std::string_view public_key, std::string_view message, std::string_view signature) {
	EVP_PKEY* pubKey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(pubKey, createPublicRSA(public_key));

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

int main() {
	// These are unsecure random test dev keys
	std::string private_key = R"(-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEApI+E+YKZZP2Tq5puLUYaCXcC/Axwdn0dNloJVqEKkIdh/GAV
6OvOk644nOCWigcODbPrDUoIesWVJi0HSNNRa42qZMa25W1FNMB3rzGJu9/gktwO
Sse8jNwQWPLf9J90EpDSBKoLuqZR9M0tEUQFfQKENaey+vYjTQn6Qf6mID5CiJxX
kD4h/MhFbpoDLra7lBDINKYEPyhoR4wgy5ddHHJzV+jx0ai4x2qAfHonmMh21zON
ViFvSWogzL+r116oAi3MGzGRmNMiPXW5r/RHEhJeKwUc/CLyTo7nSx9bm3SCn8Tt
ppHJXoWDFJGjRhW2vuTXuFKjOZSiqd5wSzd+qwIDAQABAoIBAGjj5gFYAkhB7VdL
JwEKltwDlM5ta13LH8yeFSe3nFFFSeqgoDaH33N4cDNmX+340zdev+sHjmIbAJeF
ygfcUiB4+uwjUT7pqFwYdfEgZUdwSuexBhcaOw+Z0X2wyZlV7ZlL3+IDNViyXLjp
8tj3f3wQF49PpiCatSOMwYxBMUiJARtmFDEvh/tuJ+WnXOtEiV6Qk6JWDeTvLrYr
J+VfaQzw4aQnHVJ++Gnwq1bio/mS80QkGnuRqn2kys6TWK7UQkDkC6WWWOUaScO4
q/Tbhj6ubCKRKKz3TZOPbwU56e2sYfD77wK7cRsoUxYXBafeMxnUP/CHHq95PYXM
Hz9BXFECgYEAzxWGwost5JMuh9sXACR8Go38iqZ8ZgFPhr9Dhzkk3bOxxneb4Qls
ZJfw9HrnLcxcB012GRGlsUq0h6ce4NGeyO7MJz7kpyChO05++9aIkfLilSyYY2oK
8BsTnXdfD//8PfWxRMizbWX+PIh5Dp4a8kP2f3A4cqBdnxoCMeTnDYkCgYEAy26U
LZuzQma2l0G6KeI+q3jTqimkF410QWb7nwRVt2CaMqh3kBU2UtgI6w26DqPH3y8X
3K89DgeJ4cJ59lF4zN+huAUiPb0pkX3mJ3Mwy4HpE8WsniCKoVAD4s++XSx4lUfb
u3S+/TqTxqPDWVi47LG/P/QTdBYkWNHJB/P3sZMCgYEAosPBqvFn/eutO/z5JBSJ
Mvn80CGxTx+imT+F3SgOEO+nQF5Mt+EHQD2olxBu2jFw+BBrTaLwP9x+7sxtWbmI
+1euQP6PC8l60LmO13S03Tox25w0npb/x98QWMk1f8btROnzDiRN2yO6Y2vE8rdU
aK0AdwQGahof2+i+ZucKsDkCgYEAr339JjsjMS0aZvG0f11FYvcg4bJ6dDb7C1Cj
0FIU/9S+MOVT/1/NNTpYty1oTCjNy6L8mswxh2DJeZAjVnKCG3rwL6d+GnSM344U
dgPRHD49q2jjuKWp8e7s60T7m7U5cM5EqDnWaO2XfczYQMNhBA9yROFxxrszL8wJ
+GpmSzECgYEAsWSraM93gALPrg6+GTqDW7qGiACqTYIcgkwSG+o7PZ1emn5BKP8m
sNETHmx4NeitreNKMsmEsyFIjXB0rGp5oIV4uEzx4E+I2qZ8WsP2lmj84zaMU357
NT2SR86+yteOyq6t76h7fnTbnc+jHzsv4ufxDWBTKv4NZw0Esw/7zEk=
-----END RSA PRIVATE KEY-----)";

	// These are unsecure random test dev keys
	std::string public_key = R"(-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEApI+E+YKZZP2Tq5puLUYa
CXcC/Axwdn0dNloJVqEKkIdh/GAV6OvOk644nOCWigcODbPrDUoIesWVJi0HSNNR
a42qZMa25W1FNMB3rzGJu9/gktwOSse8jNwQWPLf9J90EpDSBKoLuqZR9M0tEUQF
fQKENaey+vYjTQn6Qf6mID5CiJxXkD4h/MhFbpoDLra7lBDINKYEPyhoR4wgy5dd
HHJzV+jx0ai4x2qAfHonmMh21zONViFvSWogzL+r116oAi3MGzGRmNMiPXW5r/RH
EhJeKwUc/CLyTo7nSx9bm3SCn8TtppHJXoWDFJGjRhW2vuTXuFKjOZSiqd5wSzd+
qwIDAQAB
-----END PUBLIC KEY-----)";

	const auto message_original = "My secret message.\n";
//	const auto message_received = "Dirty message.\n";
	const auto message_received = message_original;

//	// -------------------------------------------------------------------------------------------------
//	const auto message_received2 = "My secret message. Dirty\n";
//	for (int i = 0; i < 100000000; ++i) {
//		const auto signature = signMessage(private_key, message_original);
//		const auto authentic = verifySignature(public_key, i % 2 == 0 ? message_received : message_received2, signature);
//		if (i % 1000 == 0)
//			std::cout << i << " " << authentic << std::endl;
//	}
//	// -------------------------------------------------------------------------------------------------

	const auto signature = signMessage(private_key, message_original);

	std::cout << "signature (" << signature.size() << " byte):\n" << libv::hex_dump_with_ascii(signature) << std::endl;

	const auto authentic = verifySignature(public_key, message_received, signature);

	std::cout << (authentic ? "Authentic" : "Not Authentic") << std::endl;

	return EXIT_SUCCESS;
}
