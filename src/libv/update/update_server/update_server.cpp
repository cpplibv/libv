// Project: libv.update, File: src/libv/update/update_server/update_server.cpp

// hpp
#include <libv/update/update_server/update_server.hpp>
// pro
#include <libv/update/log.hpp>
#include <libv/update/update_server/update_info_database.hpp>
#include <libv/update/update_server/network_server.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

UpdateServer::UpdateServer(Settings settings_) :
	settings(std::move(settings_)),
	io_context(settings.num_thread_net),
	database(std::make_shared<UpdateInfoDatabase>()),
	server(std::make_unique<UpdateNetworkServer>(io_context, database)) {
}

UpdateServer::~UpdateServer() {
	// For the sake of forward declared ptr
}

void UpdateServer::register_update(std::string program, std::string variant, version_number source, version_number target, uint64_t size, resource_signature signature) {
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
