// Project: libv.update, File: src/libv/update/client/update_client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/io_context.hpp>
// std
#include <filesystem>
#include <string>
// pro
#include <libv/update/version_number.hpp>
//#include <libv/update/net/client.hpp>
#include <libv/update/client/update_check_result.hpp>
//#include <libv/update/common/protocol.hpp> // <<< Hide it from updater.hpp


// std
#include <filesystem>
#include <string>
// pro
//#include <libv/update/patch_number.hpp>
//#include <libv/update/net/client.hpp>
//#include <libv/state/state.hpp>
#include <libv/update/client/update_client.hpp> // For UpdaterSettings
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

struct UpdateClientSettings {
	version_number current_version;

	std::string program_name;
	std::string program_variant;

	std::filesystem::path path_lock_file = ".lock";
	std::filesystem::path path_temp_folder = ".update";
//	std::filesystem::path path_temp_folder = ".update/";
	std::filesystem::path path_executable;
	std::filesystem::path path_root;

	std::vector<libv::net::Address> update_servers;

	int num_thread_net = 2;
//	int num_thread_fs = 2;
	bool remove_patch_files_after_success = true;
};

// -------------------------------------------------------------------------------------------------

struct UpdateClient {
	std::unique_ptr<class ImplUpdateClient> self;

public:
	explicit UpdateClient(UpdateClientSettings settings);
	~UpdateClient();

public:
	[[nodiscard]] update_check_result check_for_update();
	void update();

public:
//	void init(
//			std::filesystem::path root_dir,
//			std::filesystem::path executable,
//			std::vector<libv::net::Address> servers,
//			version_number current
//	) {
//		path_root_dir = std::move(root_dir);
//		path_executable = std::move(executable);
//		server_addresses = std::move(servers);
//		version_current = current;
//
//		std::random_device rng; // Using high quality random order
//		std::ranges::shuffle(server_addresses, rng);
//	}
//
//	void check_for_update() {
//		for (size_t i = 0; i < 3; i++) {
//			for (const auto& server_address : server_addresses) {
//				libv::net::mtcp::ConnectionAsyncCB connection;
//
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
//						[](const msg_VersionNotSupported&) {
//
//						},
//						[](const msg_VersionUpToDate&) {
//
//						},
//						[](const msg_UpdateInfo& info) {
//
//						}
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
//			}
//		}
//	}

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

//	void update(
//			std::filesystem::path root_dir,
//			std::vector<std::filesystem::path> patches,
//			version_number current,
//			version_number target);
//	void migrate(version_number migrate_on, std::function<void()> callback);
};

//void foo() {
//	Updater updater;
//	updater.update();
//	updater.migrate(0, []() {
//
//	});
//	updater.migrate(2, []() {
//
//	});
//	updater.migrate(16, []() {
//
//	});
//}

// -------------------------------------------------------------------------------------------------

//struct UpdateChecker {
//	std::vector<libv::net::Address> server_addresses;
//
//public:
//	UpdateChecker(std::vector<libv::net::Address> servers);
//	std::vector<std::string> check(version_number version_current);
//};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
