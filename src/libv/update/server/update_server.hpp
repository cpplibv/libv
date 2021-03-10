// Project: libv.update, File: src/libv/update/server/update_server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/mt/hardware_concurrency.hpp>
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

class UpdateServer {
public:
	struct Settings {
		libv::net::mtcp::Endpoint endpoint;
		size_t num_accept_backlog = 4;

		size_t num_thread_net = libv::mt::hardware_concurrency_or(2) + 2;

		std::vector<libv::net::Address> resource_servers;
	};

private:
	Settings settings;

private:
	libv::net::IOContext io_context;
	std::shared_ptr<class UpdateInfoDatabase> database;
	std::unique_ptr<class UpdateNetworkServer> server;

public:
	explicit UpdateServer(Settings settings);
	~UpdateServer();

public:
	void register_update(std::string program, std::string variant, version_number source, version_number target, uint64_t size, update_signature signature);

	void debug_dump();
	void start();
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
