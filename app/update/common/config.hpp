// Project: libv.update, File: app/update/common/config.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/address.hpp>
#include <libv/update/version_number.hpp>
// std
#include <cstdint>
#include <string_view>


namespace app {

// -------------------------------------------------------------------------------------------------

static constexpr uint16_t default_resource_port = 25080;
static constexpr uint16_t default_update_port = 25090;

static constexpr auto current_version = libv::update::version_number(1);

static constexpr auto program_name = std::string_view("app.update");
static constexpr auto program_variant = std::string_view("dev");

static const auto update_servers = std::vector<libv::net::Address>{
		{"localhost", app::default_update_port + 0},
//		{"rs0.corruptedai.com", app::default_update_port + 0},
//		{"rs1.corruptedai.com", app::default_update_port + 1},
//		{"rs2.corruptedai.com", app::default_update_port + 2},
//		{"rs3.corruptedai.com", app::default_update_port + 3},
//		{"rs4.corruptedai.com", app::default_update_port + 4},
};

static const auto resource_servers = std::vector<libv::net::Address>{
		{"localhost", app::default_update_port + 0},
//		{"rs0.corruptedai.com", app::default_update_port + 0},
//		{"rs1.corruptedai.com", app::default_update_port + 1},
//		{"rs2.corruptedai.com", app::default_update_port + 2},
//		{"rs3.corruptedai.com", app::default_update_port + 3},
//		{"rs4.corruptedai.com", app::default_update_port + 4},
};

// -------------------------------------------------------------------------------------------------

} // namespace app
