// Project: libv.update, File: src/libv/update/resource_server/resource_server_state.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/resource_server/resource_server_state.lpp>
// libv
//#include <libv/mt/worker_thread_pool.hpp>
#include <libv/algo/linear_find.hpp>
// pro
//#include <libv/update/log.hpp>
//#include <libv/update/resource_server/resource_file.lpp>
//#include <libv/update/resource_server/resource_peer.lpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

void ServerState::join(libv::net::mtcp::Connection<ResourcePeer> peer) {
	const auto lock = std::unique_lock(mutex);

	if (active_peers.size() < settings_.limit_peer_count_active || settings_.limit_peer_count_active == 0) {
		active_peers.emplace(peer);
		return;
	}

	if (queued_peers.size() < settings_.limit_peer_count_queue || settings_.limit_peer_count_queue == 0) {
		queued_peers.emplace_back(peer);
		return;
	}

//	peer.connection().send_async(codec.encode(ResponseBusy(load_trend.busy_time())));
}

//	void ServerState::inactivity_scan() {
//		const auto now = std::chrono::system_clock::now();
////		const auto limit =
////		for (const auto& peer : active_peers)
////			if (peer->last_activity + settings.kick_inactivity_time > now)
////				erase(peer);
//		io_context.execute_async([this] {
//			inactivity_scan();
//		});
//	}

void ServerState::leave(libv::net::mtcp::Connection<ResourcePeer> peer) {
	const auto lock = std::unique_lock(mutex);

	const auto ita = active_peers.find(peer);
	if (ita != active_peers.end()) {
		// Active peer disconnecting
		active_peers.erase(peer);

		if (!queued_peers.empty()) {
			const auto active_peer_limit_reached = active_peers.size() >= settings_.limit_peer_count_active && settings_.limit_peer_count_active != 0;
			if (!active_peer_limit_reached) {
				active_peers.emplace(queued_peers.front());
				queued_peers.pop_front();
			}
		}
	} else {
		// Queued peer disconnecting
		queued_peers.erase(libv::linear_find_iterator(queued_peers, peer));
	}
}

void ServerState::disconnect_all() {
	const auto lock = std::unique_lock(mutex);

	for (const auto& peer : queued_peers)
		peer.connection().cancel_and_disconnect_async();

	for (const auto& peer : active_peers)
		peer.connection().cancel_and_disconnect_async();
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
