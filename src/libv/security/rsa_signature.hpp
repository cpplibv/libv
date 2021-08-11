// Project: libv.security, File: src/libv/security/rsa_signature.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string>
#include <string_view>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string rsa_sign_message(std::string_view message, std::string_view private_key);

[[nodiscard]] bool rsa_verify_signature(std::string_view message, std::string_view public_key, std::string_view signature);

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
