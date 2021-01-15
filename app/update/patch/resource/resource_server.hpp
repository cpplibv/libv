// Project: libv, File: app/update/patch/resource/resource_server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/fwd.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/utility/storage_size.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

enum class FileCachePolicy {
//	never,
//	on_demand,
	always,
};

struct ResourceServer {

	struct Settings {
		size_t num_threads_fs = 2;
		size_t num_threads_net = 4;

//		size_t limit_bps_global_upload = libv::MB(120);
//		size_t limit_bps_global_download = 0;
//		size_t limit_bps_peer_upload = libv::MB(10);
//		size_t limit_bps_peer_download = 0;

		size_t resource_network_chunk_size = libv::KB(512);

		libv::net::mtcp::Endpoint acceptor_endpoint;
		size_t acceptor_connection_backlog = 3;

		size_t limit_peer_count_active = 32; // Zero means no limit
		size_t limit_peer_count_queue = 32; // Zero means no limit

//		std::chrono::steady_clock::duration busy_time_min = std::chrono::seconds(10);
//		std::chrono::steady_clock::duration busy_time_max = std::chrono::seconds(30);
//
//		size_t kick_on_request_failure = 4;
//		std::chrono::steady_clock::duration kick_inactivity_time = std::chrono::seconds(10);
//
//		size_t ban_on_request_failure = 12;
//
//		std::chrono::steady_clock::duration ban_duration = std::chrono::hours(24);
//
//		size_t over_transfer_threshold = libv::GB(16);
//		double over_transfer_penalty = 0.5;
//
//		size_t on_demand_file_memory_cache_size = libv::GB(4);
	};

private:
	std::unique_ptr<class ImplResourceServer> self;

public:
	explicit ResourceServer(Settings settings_);
	~ResourceServer();

public:
	[[nodiscard]] const Settings& settings() const noexcept;

public:
	void add_file(std::string id, std::filesystem::path path, FileCachePolicy fcp = FileCachePolicy::always);
//	void add_custom(std::string id, std::function<std::shared_ptr<std::span<std::byte>>(std::string_view)>);

public:
	void listen(libv::net::IOContext& io_context, const libv::net::mtcp::Endpoint& endpoint);
//	void adopt_peer(libv::net::mtcp::ConnectionAsyncCB peer)
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
