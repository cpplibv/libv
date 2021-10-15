// Project: libv.update, File: src/libv/update/resource_client/resource_client.cpp

// hpp
#include <libv/update/resource_client/resource_client.hpp>
// libv
#include <libv/net/fwd.hpp>
//#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/connection_he.hpp>
// std
#include <memory>
// pro
#include <libv/update/log.hpp>
#include <libv/update/resource_client/task_pool.hxx>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct ImplResourceClient {
	libv::net::IOContext& io_context;
	ResourceClient::Settings settings;

	std::shared_ptr<TaskPool> tasks;

public:
	explicit inline ImplResourceClient(libv::net::IOContext& io_context, ResourceClient::Settings settings_) :
		io_context(io_context),
		settings(std::move(settings_)),
		tasks(std::make_shared<TaskPool>(io_context, settings.resource_servers)) {
	}
};

// -------------------------------------------------------------------------------------------------

ResourceClient::ResourceClient(libv::net::IOContext& io_context, ResourceClient::Settings settings) :
	self(std::make_unique<ImplResourceClient>(io_context, std::move(settings))) {
}

ResourceClient::~ResourceClient() {
	// For the sake of forward declared unique_ptr
}

void ResourceClient::download_file(std::string identifier, std::filesystem::path save_file) {
	log_update.trace("Downloading {}...\n\tto file: {}", identifier, save_file.generic_string());
	self->tasks->queue_task_file(std::move(identifier), std::move(save_file));
}

void ResourceClient::wait() {
	self->tasks->wait();
//	const auto success = self->tasks->wait();
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
