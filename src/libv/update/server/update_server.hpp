// Project: libv.update, File: src/libv/update/client/updater.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/fwd.hpp>
#include <libv/net/io_context.hpp>
// std
//#include <filesystem>
#include <string>
// pro
#include <libv/update/version_number.hpp>
//#include <libv/update/net/client.hpp>
//#include <libv/update/client/update_check_result.hpp>
//// std
//#include <filesystem>
//#include <string>
//// pro
////#include <libv/update/patch_number.hpp>
////#include <libv/update/net/client.hpp>
////#include <libv/state/state.hpp>
//#include <libv/update/client/updater.hpp> // For UpdaterSettings
//#include <libv/net/address.hpp>
////#include <libv/net/mtcp/connection.hpp>
////#include <libv/serial/serialization.hpp>
//#include <algorithm>
//#include <ranges>
//#include <random>
//#include <libv/net/mtcp/acceptor_he.hpp>
////#include <memory>
////#include <vector>


#include <libv/update/net/update_network_server.hpp>



namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

//struct UpdaterSettings {
//	version_number current_version;
//
//	std::string program_name;
//	std::string program_variant;
//
//	std::filesystem::path path_lock_file = ".lock";
//	std::filesystem::path path_temp_folder = ".update";
////	std::filesystem::path path_temp_folder = ".update/";
//	std::filesystem::path path_executable;
//	std::filesystem::path path_root;
//
//	std::vector<libv::net::Address> update_servers;
//
//	int num_thread_net = 2;
////	int num_thread_fs = 2;
//	bool remove_patch_files_after_success = true;
//};

// -------------------------------------------------------------------------------------------------

struct UpdateServer {
//public:
//	UpdaterSettings settings;
//	msg::UpdateInfo update_info;

public:
	libv::net::IOContext io_context;
	UpdateNetworkServer server;

public:
	explicit UpdateServer(libv::net::mtcp::Endpoint endpoint, int num_io_thread);

//	~UpdateServer() {
//		io_context.join();
//	}

public:
//	void add_version(std::string program, std::string variant, version_number version);
	void add_update(std::string program, std::string variant, version_number source, version_number target, uint64_t size, uint64_t signature);
	void foo();
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
