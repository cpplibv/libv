// Project: libv.update, File: src/libv/update/client/updateServer.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/server/update_server.hpp>
// libv
#include <libv/net/mtcp/endpoint.hpp>
// std
//#include <filesystem>
//#include <memory>
//#include <string>
//#include <vector>
// pro
#include <libv/update/net/update_network_server.hpp>




//#include <filesystem>
//#include <fstream>
//#include <memory>
//#include <string>
//#include <libv/net/address.hpp>
//#include <libv/net/mtcp/acceptor.hpp>
//#include <libv/net/mtcp/connection_he.hpp>
//#include <libv/process/lock_file.hpp>
//#include <libv/serial/serial.hpp>
//#include <libv/state/state.hpp>
//#include <libv/update/client/patch_applier.hpp>
//#include <libv/update/log.hpp>
//#include <libv/update/net/protocol.hpp>
//#include <libv/update/net/updateServer_network_client.hpp>
//#include <libv/update/patch.hpp>
//#include <libv/update/patch_applier.hpp>
//#include <libv/utility/enum.hpp>
//#include <libv/utility/read_file.hpp>
//#include <libv/utility/storage_size.hpp>
//#include <libv/utility/uniform_distribution.hpp>
//#include <libv/utility/write_file.hpp>


//bool exists_throw(const std::filesystem::path& path) {
//	std::error_code ec;
//
//	const auto exists = std::filesystem::exists(path, ec);
//	if (ec)
//		throw io_exception(path, ec);
//
//	return exists;
//}

namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

//void UpdateServer::update(
//		std::filesystem::path root_dir,
//		std::vector<std::filesystem::path> patches,
//		version_number current,
//		version_number target) {
//
//	version_current = current;
//	version_target = target;
//
////	filepath_root_dir = root_dir;
////	filepath_lock = root_dir / settings.filename_lock;
////	filepath_journey = root_dir / settings.filename_lock;
//}

UpdateServer::UpdateServer(libv::net::mtcp::Endpoint endpoint, int num_io_thread) :
	io_context(num_io_thread),
	server(io_context) {
	server.listen(endpoint, 4); // <<< backlog var
}

//void UpdateServer::add_version(std::string program, std::string variant, version_number version) {
//
//}

void UpdateServer::add_update(std::string program, std::string variant, version_number source, version_number target, uint64_t size, uint64_t signature) {

}

void UpdateServer::foo() {
//	UpdateNetworkServer connection(io_context, server_address);

//	log_update.trace("Checking update for program: [{}] variant: [{}] version: [{}]", settings.program_name, settings.program_variant, libv::to_value(settings.current_version));
//	log_update.trace("Connecting to update server server: {} (Attempt {})", server_address, i + 1);
//	log_update.trace("Waiting {} ms before retrying ({})...", wait.count(), i + 1);

//	patch = Patch::load_from_file(filepath);
//	patch = Patch::load_header_from_file(filepath);

//	patch = Patch::load_header_from_file(filepath);
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
