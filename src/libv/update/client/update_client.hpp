// Project: libv.update, File: src/libv/update/client/update_client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <string>
// pro
#include <libv/update/version_number.hpp>
//#include <libv/update/patch_number.hpp>
//#include <libv/update/common/client.hpp>








//

//#include <libv/state/state.hpp>

#include <libv/update/client/updater.hpp> // For UpdaterSettings
#include <libv/net/address.hpp>
//#include <libv/net/mtcp/connection.hpp>
//#include <libv/serial/serialization.hpp>


#include <algorithm>
#include <ranges>
#include <random>
//#include <memory>
//#include <vector>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct UpdateClient {
	version_number version_current;
	UpdaterSettings settings;

	std::filesystem::path filepath_root_dir;
	std::filesystem::path filepath_executable;

	std::vector<libv::net::Address> server_addresses;

public:
	void init(
			std::filesystem::path root_dir,
			std::filesystem::path executable,
			std::vector<libv::net::Address> servers,
			version_number current
	) {
		filepath_root_dir = std::move(root_dir);
		filepath_executable = std::move(executable);
		server_addresses = std::move(servers);
		version_current = current;

		std::random_device rng; // NOTE: Using high quality random order (only a few call anyways)
		std::ranges::shuffle(server_addresses, rng);
	}

	void check_for_update() {
		for (size_t i = 0; i < 3; i++) {
			for (const auto& server_address : server_addresses) {

//				connection.connect(server_address);
////				connection.connect(server_address, std::chrono::seconds{10});
//
//				connection.send(codec.encode(msg_ReportVersion{current_version}));
//				connection.receive(1);
//
//				handler = libv::overload(
//					[](const msg_VersionNotSupported&) {
//
//					},
//					[](const msg_VersionUpToDate&) {
//
//					},
//					[](const msg_UpdateInfo& info) {
//
//					}
//				);
			}

//			if (made progress)
//				i = 0;
		}
	}

	void execute_update_migration_process() {

	}

	void update_to_latest() {
//		connection.send(codec.encode(msg_RequestPatch{...}));
//
//		on_msg_PatchNotSupported() {
//
//		}
//		on_msg_PatchChunk() {
//
//		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
