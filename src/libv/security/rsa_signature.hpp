// Created by Vader on 2021.08.12..

#pragma once

// std
#include <string>
#include <string_view>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

std::string rsa_sign_message(std::string_view message, std::string_view private_key);

bool rsa_verify_signature(std::string_view message, std::string_view public_key, std::string_view signature);

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
