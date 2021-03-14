// Project: libv.update, File: src/libv/update/resource_client/resource_client.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/resource_client/resource_client.hpp>
// libv

// std
#include <deque>
#include <fstream>
// pro
#include <libv/update/log.hpp>
#include <libv/update/resource_client/resource_client_connection.lpp>
#include <libv/update/resource_client/task_pool.lpp>


//#include <filesystem>
//#include <memory>
//#include <string>
//#include <vector>



#include <libv/update/resource_client/resource_file.lpp>
#include <libv/update/common/protocol_res.hpp>


#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/connection_he.hpp>


#include <libv/utility/read_file.hpp>
#include <libv/utility/hex_dump.hpp>
#include <utility>



namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct ImplResourceClient {
	libv::net::IOContext& io_context;
	ResourceClient::Settings settings;

	std::shared_ptr<TaskPool> tasks;
//	boost::container::flat_set<Connection> connections;

	// <<< Storing connections here is an object leak, kind of, not but really
	std::vector<libv::net::mtcp::Connection<ResourceClientConnection>> connections;

public:
	explicit inline ImplResourceClient(libv::net::IOContext& io_context, ResourceClient::Settings settings) :
		io_context(io_context),
		settings(std::move(settings)),
		tasks(std::make_shared<TaskPool>()) {

		for (const auto& server : settings.resource_servers)
			connections.emplace_back(io_context, tasks, server);
	}
};

// -------------------------------------------------------------------------------------------------

ResourceClient::ResourceClient(libv::net::IOContext& io_context, ResourceClient::Settings settings) :
	self(std::make_unique<ImplResourceClient>(io_context, std::move(settings))) {
}

ResourceClient::~ResourceClient() {
	// For the sake of forward declared unique_ptr
}

void ResourceClient::download_file(std::string identifier, std::filesystem::path save_file, size_t size_hint) {
//	identifier;
//	save_file;
//	size_hint;

	auto temp_file = save_file;
	temp_file += ".temp";

	log_update.trace("Downloading {}...\n\t{}\n\t{}", identifier, save_file, temp_file);
	self->tasks->queue_task(ResourceTask{identifier, std::move(save_file), std::move(temp_file), size_hint});
}

void ResourceClient::wait() {
	self->tasks->wait();
//	{
//		try {
//			const auto f = libv::read_file_str_or_throw("resource_test_file");
//			const auto s = libv::hex_dump_with_ascii(f);
//			log_update.debug("Before ({}):\n{}\n", f.size(), s);
//		} catch (...) {
//			log_update.debug("No file beforehand");
//		}
//	} {
//		ResourceFile tmp_test_file("resource_test_file");
//		tmp_test_file.create(60);
//		std::string_view data = "Instructions unclear";
//		tmp_test_file.write(30, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
//		tmp_test_file.write(10, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
//		tmp_test_file.write(40, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
//		tmp_test_file.write(20, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
//		tmp_test_file.write(0, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
////		tmp_test_file.close();
//	} {
//		const auto f = libv::read_file_str_or_throw("resource_test_file");
//		const auto s = libv::hex_dump_with_ascii(f);
//		log_update.debug("After ({}):\n{}\n", f.size(), s);
//	}

	0;
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
