// Project: libv.update, File: app/update/common/config.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
//#include <cstdint>
#include <string_view>
// libv
#include <libv/update/version_number.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

//static constexpr std::string_view default_address = "localhost";
//
//static constexpr uint16_t default_port = 25090;
//static constexpr uint16_t default_net_thread = 4;

static constexpr auto current_version = libv::update::version_number(1);

static constexpr auto program_name = std::string_view("app.update");
static constexpr auto program_variant = std::string_view("dev");

// -------------------------------------------------------------------------------------------------

} // namespace app
