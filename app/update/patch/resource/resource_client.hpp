// Project: libv, File: app/update/patch/resource/resource_client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// hpp
//#include <update/patch/update_client.hpp>
// libv
//#include <libv/process/lock_file.hpp>
//#include <libv/utility/read_file.hpp>
//#include <libv/utility/write_file.hpp>
// std
#include <filesystem>
#include <memory>
//#include <string>
//#include <fstream>
// pro
//#include <update/patch/patch_applier.hpp>
//#include <update/patch/patch.hpp>





//#include <libv/serial/serialization.hpp>
//#include <libv/net/mtcp/acceptor.hpp>
//#include <libv/net/mtcp/connection.hpp>
//#include <libv/net/mtcp/endpoint.hpp>
//#include <libv/net/address.hpp>
#include <libv/net/fwd.hpp>
//#include <libv/utility/storage_size.hpp>
//
//#include <libv/state/state.hpp>

#include <vector>
// pro
//#include <update/patch/patch.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct ResourceClient {

	struct Settings {
//		size_t limit_bps_global_upload = libv::MB(10);
//		size_t limit_bps_global_download = 0;
//		size_t limit_bps_peer_upload = 0;
//		size_t limit_bps_peer_download = 0;

		size_t limit_active_peer_count = 32;
	};

//	libv::net::mtcp::ConnectionAsyncCB connection;
//	std::vector<libv::net::Address> server_addresses;

	struct Task {
		std::string identifier;
		std::filesystem::path save_filepath;
		std::filesystem::path temp_filepath;

//		size_t chunk;
//		size_t size;
	};

	std::vector<Task> queue;
	std::vector<Task> active_tasks;

public:
	ResourceClient(libv::net::Address servers);
//	void rate_limit(size_t read_bps, size_t write_bps);

	void download(std::string identifier, std::filesystem::path save_filepath);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
