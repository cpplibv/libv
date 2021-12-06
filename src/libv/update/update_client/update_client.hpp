// Project: libv.update, File: src/libv/update/update_client/update_client.hpp

#pragma once

// libv
#include <libv/net/io_context.hpp>
// std
#include <filesystem>
#include <string>
// pro
#include <libv/update/version_number.hpp>
//#include <libv/update/net/client.hpp>
#include <libv/update/update_client/update_check_result.hpp>
//#include <libv/update/common/protocol.hpp> // <<< Hide it from updater.hpp


// std
#include <filesystem>
#include <string>
// pro
//#include <libv/update/patch_number.hpp>
//#include <libv/update/net/client.hpp>
//#include <libv/state/state.hpp>
//#include <libv/update/client/update_client.hpp> // For UpdaterSettings
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

class UpdateStatus {
//	std::mutex mutex;
	class UpdateClient& client;

public:
	bool progress_for(std::chrono::steady_clock::duration duration) {
//		std::this_thread::sleep_for(duration);

		return false;
	}

	std::size_t progress_current();
	std::size_t progress_total();
	int operation();
	std::string operation_file();
};

class UpdateClient {
public:
	struct Settings {
		version_number current_version;

		std::string program_name;
		std::string program_variant;

		std::filesystem::path path_lock_file = ".lock";
		std::filesystem::path path_temp_folder = ".update";
//		std::filesystem::path path_temp_folder = ".update/";
		std::filesystem::path path_executable;
		std::filesystem::path path_root;

		std::vector<libv::net::Address> update_servers;
		std::vector<libv::net::Address> resource_servers;

		std::size_t num_thread_fs = 2;
		std::size_t num_thread_net = 4;

//		std::size_t limit_bps_global_download = 0;

		bool remove_patch_files_after_success = true;
	};

private:
	std::unique_ptr<class ImplUpdateClient> self;

public:
	explicit UpdateClient(Settings settings);
	~UpdateClient();

public:
	[[nodiscard]] update_check_result check_for_update();
	void update();
//	UpdateStatus update();

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

} // namespace update
} // namespace libv
