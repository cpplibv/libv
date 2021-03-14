// Project: libv.update, File: src/libv/update/resource_server/resource_server_state.lpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/mtcp/connection_he.hpp>
// std
#include <memory>
#include <mutex>
#include <deque>
#include <unordered_map>
#include <unordered_set>
// pro
#include <libv/update/resource_server/resource_server.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class ResourceFile;
class ResourcePeer;

// -------------------------------------------------------------------------------------------------

class ServerState {
private:
	std::mutex mutex;

	const ResourceServer::Settings& settings_;

	std::unordered_set<libv::net::mtcp::Connection<ResourcePeer>> active_peers;
	std::deque<libv::net::mtcp::Connection<ResourcePeer>> queued_peers;

	std::unordered_map<std::string, std::shared_ptr<ResourceFile>> resources;

public:
	explicit inline ServerState(const ResourceServer::Settings& settings) :
		settings_(settings) {}

public:
	[[nodiscard]] constexpr inline const ResourceServer::Settings& settings() const noexcept {
		return settings_;
	}

	[[nodiscard]] inline auto get_resource(const std::string& name, uint64_t offset, uint64_t amount) {
		(void) offset; (void) amount; // TODO P2: Partial resource loading

		struct Result {
			bool invalid_request = true;
			bool not_found = true;
			std::shared_ptr<ResourceFile> resource;
		};

		const auto it = resources.find(name);

		if (it == resources.end())
			return Result(false, true, nullptr);
		else
			return Result(false, false, it->second);
	}

public:
	void add(std::string name, std::shared_ptr<ResourceFile> resource) {
		resources.emplace(std::move(name), std::move(resource));
	}

public:
	void join(libv::net::mtcp::Connection<ResourcePeer> peer);
//	void inactivity_scan();
	void leave(libv::net::mtcp::Connection<ResourcePeer> peer);
	void disconnect_all();
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
