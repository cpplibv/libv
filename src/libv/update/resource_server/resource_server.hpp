// Project: libv.update, File: src/libv/update/resource_server/resource_server.hpp

#pragma once

// libv
#include <libv/mt/hardware_concurrency.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/utility/storage_size.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

enum class resource_cache_policy {
	never,
//	on_demand,
	always,
};

class ResourceServer {
public:
	struct Settings {
		libv::net::mtcp::Endpoint endpoint;
		std::size_t num_accept_backlog = 4;

		std::size_t num_thread_net = libv::mt::hardware_concurrency_or(2) + 2;
		std::size_t num_thread_fs = 2;

//		std::size_t limit_bps_global_upload = libv::MB(120);
//		std::size_t limit_bps_global_download = 0;
//		std::size_t limit_bps_peer_upload = libv::MB(10);
//		std::size_t limit_bps_peer_download = 0;

		std::size_t resource_network_chunk_size = libv::MB(1);
//		std::size_t resource_network_chunk_queue = 2;

		std::size_t limit_peer_count_active = 32; // Zero means no limit
		std::size_t limit_peer_count_queue = 32; // Zero means no limit

//		std::chrono::steady_clock::duration busy_time_min = std::chrono::seconds(10);
//		std::chrono::steady_clock::duration busy_time_max = std::chrono::seconds(30);
//
//		std::size_t kick_on_request_failure = 4;
//		std::chrono::steady_clock::duration kick_inactivity_time = std::chrono::seconds(10);
//
//		std::size_t ban_on_request_failure = 12;
//
//		std::chrono::steady_clock::duration ban_duration = std::chrono::hours(24);
//
//		std::size_t over_transfer_threshold = libv::GB(16);
//		double over_transfer_penalty = 0.5;
//
//		std::size_t on_demand_file_memory_cache_size = libv::GB(4);
	};

private:
	std::unique_ptr<class ImplResourceServer> self;

public:
	explicit ResourceServer(Settings settings_);
	~ResourceServer();

public:
	[[nodiscard]] const Settings& settings() const noexcept;

public:
	void add_file(std::string id, std::filesystem::path path, resource_cache_policy fcp = resource_cache_policy::always);
//	void add_custom(std::string id, std::function<std::shared_ptr<std::span<std::byte>>(std::string_view)>);

public:
//	void adopt_peer(libv::net::mtcp::ConnectionAsyncCB peer)

public:
	void start();
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
