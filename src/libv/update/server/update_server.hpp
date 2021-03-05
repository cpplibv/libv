// Project: libv.update, File: src/libv/update/client/updater.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/address.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/endpoint.hpp>
// std
#include <memory>
#include <string>
// pro
#include <libv/update/update_signature.hpp>
#include <libv/update/version_number.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct UpdateServerSettings {
	libv::net::mtcp::Endpoint endpoint;

	int num_thread_net = 8;
	int num_accept_backlog = 4;

	std::vector<libv::net::Address> resource_servers;
};

// -------------------------------------------------------------------------------------------------

class UpdateServer {
private:
	UpdateServerSettings settings;

private:
	libv::net::IOContext io_context;
	std::shared_ptr<class UpdateInfoDictionary> dictionary;
	std::unique_ptr<class UpdateNetworkServer> server;

public:
	explicit UpdateServer(UpdateServerSettings settings);
	~UpdateServer();

public:
	void register_update(std::string program, std::string variant, version_number source, version_number target, uint64_t size, update_signature signature);

	void debug_dump();
	void start();
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
