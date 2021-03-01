// Project: libv, File: app/update/patch/update_client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <string>
// pro
#include <update/patch/version_number.hpp>
//#include <update/patch/patch_number.hpp>
#include <update/common/client.hpp>








//

//#include <libv/state/state.hpp>

#include <update/patch/updater.hpp> // For UpdaterSettings
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

		std::random_device rng; // Using high quality random order
		std::ranges::shuffle(server_addresses, rng);
	}

	void check_for_update() {
		for (size_t i = 0; i < 3; i++) {
			for (const auto& server_address : server_addresses) {
//				libv::net::mtcp::ConnectionAsyncCB connection;

////				connection.handle_connect()
////				connection.handle_disconnect()
////				connection.handle_error()
////				connection.handle_send()
////				connection.handle_receive()
//
//				connection.connect(server_address);
////				connection.connect(server_address, std::chrono::seconds{10});
//
//				connection.send(codec.encode(msg_ReportVersion{current_version}));
//				connection.receive(1);
//
//				connection_ended.wait();
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
//
////				on_msg_VersionNotSupported() {
////
////				}
////				on_msg_VersionUpToDate() {
////
////				}
////				on_msg_UpdateInfo() {
////
////				}
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
