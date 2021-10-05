// Project: libv.security, File: src/libv/security/rsa_signature.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string>
#include <string_view>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string rsa_encrypt_public(std::string_view message, std::string_view public_key);
[[nodiscard]] std::string rsa_decrypt_private(std::string_view encrypted, std::string_view private_key);

[[nodiscard]] std::string rsa_encrypt_private(std::string_view message, std::string_view private_key);
[[nodiscard]] std::string rsa_decrypt_public(std::string_view encrypted, std::string_view public_key);

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
