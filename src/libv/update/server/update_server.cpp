// Project: libv.update, File: src/libv/update/server/update_server.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/server/update_server.hpp>
// pro
#include <libv/update/log.hpp>
#include <libv/update/server/update_info_database.hpp>
#include <libv/update/server/network_server.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

UpdateServer::UpdateServer(UpdateServerSettings settings_) :
	settings(std::move(settings_)),
	io_context(settings.num_thread_net),
	database(std::make_shared<UpdateInfoDatabase>()),
	server(std::make_unique<UpdateNetworkServer>(io_context, database)) {
}

UpdateServer::~UpdateServer() {
	// For the sake of forward declared unique_ptr
}

void UpdateServer::register_update(std::string program, std::string variant, version_number source, version_number target, uint64_t size, update_signature signature) {
	database->register_update(std::move(program), std::move(variant), source, target, size, signature);
}

void UpdateServer::debug_dump() {
	database->debug_dump();
}

void UpdateServer::start() {
	log_update.info("Starting server on: {}...", settings.endpoint);
	debug_dump();

	server->listen(settings.endpoint, settings.num_accept_backlog);
	server->start();
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
