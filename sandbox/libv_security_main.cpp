// Project: libv.security, File: sandbox/libv_security_main.cpp

// libv
#include <libv/security/hash_sha256.hpp>
#include <libv/security/rsa_encrypt.hpp>
#include <libv/security/rsa_signature.hpp>
#include <libv/utility/hex_dump.hpp>
// std
#include <iostream>
#include <string>


// -------------------------------------------------------------------------------------------------

// This is an unsecure (exposed in a public repo) 2048 key pair and used only for development/testing
const std::string_view private_key_2048 = R"(-----BEGIN RSA PRIVATE KEY-----
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

// This is an unsecure (exposed in a public repo) 2048 key pair and used only for development/testing
const std::string_view public_key_2048 = R"(-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEApI+E+YKZZP2Tq5puLUYa
CXcC/Axwdn0dNloJVqEKkIdh/GAV6OvOk644nOCWigcODbPrDUoIesWVJi0HSNNR
a42qZMa25W1FNMB3rzGJu9/gktwOSse8jNwQWPLf9J90EpDSBKoLuqZR9M0tEUQF
fQKENaey+vYjTQn6Qf6mID5CiJxXkD4h/MhFbpoDLra7lBDINKYEPyhoR4wgy5dd
HHJzV+jx0ai4x2qAfHonmMh21zONViFvSWogzL+r116oAi3MGzGRmNMiPXW5r/RH
EhJeKwUc/CLyTo7nSx9bm3SCn8TtppHJXoWDFJGjRhW2vuTXuFKjOZSiqd5wSzd+
qwIDAQAB
-----END PUBLIC KEY-----)";

// This is an unsecure (exposed in a public repo) 4096 key pair and used only for development/testing
const std::string_view private_key_4096 = R"(-----BEGIN RSA PRIVATE KEY-----
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
-----END RSA PRIVATE KEY-----)";

// This is an unsecure (exposed in a public repo) 4096 key pair and used only for development/testing
const std::string_view public_key_4096 = R"(-----BEGIN PUBLIC KEY-----
MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAxNuKgIjZVsbZxflW0T6z
GFye8zNuNSlBSTvBlTwT8JOp6qb3L8h2mML5Ju3fnjsGs99SCtSdSvIGYqitegCg
H0pT5j3Uu4lUWLp+mLFoFCg+PIvQAgQdC0zB3gaqHrmb8LeT4sVwnWtlzmKxw8ls
jxvJs0oTun5zf8c+wWvLCK4gfkOJrDmTvycdqUjJjLX0Pwj8TouANMzKJ6vaEMEO
rA1GtJr6k2iPraQsAJ4c6oGpzUrJbTWLNXzRX8cz368uwoS+C6D89nqhQYxcg/gE
HLmha79Q2B5g+4PVOQ+8vCVdc0VPXdxszf0TqSHfZQkW12FIjBgQWT8e5phjwGPG
oqlOz/SSW90u1Wuay6utao9RVEAzd04/EpHe4VGlCVWAwB/j7Ae2o6z/XnttTMFr
Vy3HUxn3E90badCVTFPCSWYmTe5HZPV3NoDOXWiiDX2o2Zqfwt+61QGcN682zzSk
Vd2TgYnhhlKI7o8XaIfmJiEHF/l4LJZJ+QCTzHScyS+uxkg0/N7/xRfeW6mKJd6N
jSgA5drNbeOHvSXkkImPvteCaD3TX6MuR7U6+5gqFLxxuauOH3u3B+zZb2hqUG/8
7Qdf7ZiaMAiZWSHKALx4oXSFf+XXCZFK0/GUrqKDBPSOiPg6Szn7qTRwR5MZyJ4d
CEzZByI7LZzSdU13nm4oDa0CAwEAAQ==
-----END PUBLIC KEY-----)";

const std::string_view message_short_original = "My secret message.\n";
const std::string_view message_short_received = "Dirty message.\n";

const std::string_view message_long_original = R"(
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
- Do you know what it means when something chases you very slowly? - What? - It means there's a reason that they don't have to run.
)";

int main() {
//	// -------------------------------------------------------------------------------------------------
//	const auto message_received2 = "My secret message. Dirty\n";
//	for (int i = 0; i < 100000000; ++i) {
//		const auto signature = libv::security::rsa_sign_message(message_original, private_key);
//		const auto authentic = libv::security::rsa_verify_signature(i % 2 == 0 ? message_received : message_received2, public_key, signature);
//		if (i % 1000 == 0)
//			std::cout << i << " " << authentic << std::endl;
//	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;

	const auto signature = libv::security::rsa_sign_message(message_short_original, private_key_4096);

	std::cout << "Signature (" << signature.size() << " byte):\n" << libv::hex_dump_with_ascii(signature) << std::endl;

	const auto authentic0 = libv::security::rsa_verify_signature(message_short_original, public_key_4096, signature);
	const auto authentic1 = libv::security::rsa_verify_signature(message_short_received, public_key_4096, signature);
	std::cout << (authentic0 ? "Authentic" : "Not Authentic") << std::endl;
	std::cout << (authentic1 ? "Authentic" : "Not Authentic") << std::endl;

	std::cout << "-----------------------------------------------------------------------------" << std::endl;

	const auto sha256 = libv::security::hash_sha256(message_short_original);
	std::cout << "SHA256 (" << std::size(sha256.value) << " byte):\n" << libv::hex_dump(sha256.value) << std::endl;

	std::cout << "-----------------------------------------------------------------------------" << std::endl;

	std::cout << "Original (" << message_long_original.size() << " byte):\n" << message_long_original << std::endl;
	const auto encrypted0 = libv::security::rsa_encrypt_public(message_long_original, public_key_2048);
	std::cout << "Encrypted (" << encrypted0.size() << " byte):\n" << libv::hex_dump(encrypted0) << std::endl;
	const auto decrypted0 = libv::security::rsa_decrypt_private(encrypted0, private_key_2048);
	std::cout << "Decrypted (" << decrypted0.size() << " byte):\n" << decrypted0 << std::endl;

	std::cout << "-----------------------------------------------------------------------------" << std::endl;

	std::cout << "Original (" << message_long_original.size() << " byte):\n" << message_long_original << std::endl;
	const auto encrypted1 = libv::security::rsa_encrypt_private(message_long_original, private_key_2048);
	std::cout << "Encrypted (" << encrypted1.size() << " byte):\n" << libv::hex_dump(encrypted1) << std::endl;
	const auto decrypted1 = libv::security::rsa_decrypt_public(encrypted1, public_key_2048);
	std::cout << "Decrypted (" << decrypted1.size() << " byte):\n" << decrypted1 << std::endl;

	return EXIT_SUCCESS;
}
