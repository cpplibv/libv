// Project: libv.update, File: src/libv/update/resource_client/resource_client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/fwd.hpp>
#include <libv/net/address.hpp>
// std
#include <filesystem>
#include <memory>
#include <vector>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class ResourceClient {
public:
	struct Settings {
//		size_t limit_bps_global_upload = libv::MB(10);
//		size_t limit_bps_global_download = 0;
//		size_t limit_bps_peer_upload = 0;
//		size_t limit_bps_peer_download = 0;

		int limit_active_server_peer_count = 4;

//		std::vector<libv::net::Address> servers;
		std::vector<libv::net::Address> resource_servers;
	};

private:
	std::unique_ptr<class ImplResourceClient> self;

public:
	explicit ResourceClient(libv::net::IOContext& io_context, Settings settings);
	~ResourceClient();

public:
//	void rate_limit(size_t read_bps, size_t write_bps);

	void download_file(std::string identifier, std::filesystem::path save_filepath);
	void wait();
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
