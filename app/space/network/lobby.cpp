// Project: libv, File: app/space/network/lobby.cpp

// hpp
#include <space/network/lobby.hpp>
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/ui/ui.hpp>
//#include <libv/utility/nexus.hpp>
// std
//#include <cassert>
// pro
#include <space/cto.hpp>
#include <space/game_thread.hpp>
//#include <space/log.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Lobby::~Lobby() {
}

// -------------------------------------------------------------------------------------------------

void Lobby::process(CTO_ClientJoined&& message) {
	auto c = std::make_shared<Client>(message.userID, std::move(message.user_name), message.joined_at, message.version);
	clients.emplace_back(c);

	game_thread.broadcast_on_thread(OnClientJoin{std::move(c)});
}

void Lobby::process(CTO_ClientLeave&& message) {
	auto it = libv::linear_find_iterator(clients, message.userID, &Client::userID);

	if (it == clients.end()) {
		// assert(false && "Internal consistency error"); // Maybe do not hard error, this can be hostile CTO
		return;
	}

	auto client = std::move(*it);
	clients.erase(it);
	game_thread.broadcast_on_thread(OnClientLeave{std::move(client)});
}

void Lobby::process(CTO_ChatMessage&& message) {
	// Permission check
	// Identity check
	auto it = libv::linear_find_iterator(clients, message.userID, &Client::userID);

	if (it == clients.end()) {
		// assert(false && "Internal consistency error"); // Maybe do not hard error, this can be hostile CTO
		return;
	}

	const auto& entry = chat_entries.emplace_back(*it, message.sent_at, std::move(message.message));

	game_thread.broadcast_on_thread(OnChatMessage{entry});
}

void Lobby::process(CTO_LobbyStatus&& message) {
	for (auto& client : clients)
		for (const auto& status : message.pings)
			if (client->userID == status.userID) {
				client->ping = status.ping;
				break;
			}

	// TODO P1: Implement OnLobbyStatus event
//	game_thread.broadcast_on_thread(OnLobbyStatus{*it});
}

//void Lobby::process(CTO_LobbyClose&& message) {
//	(void) message;
//
//	clients.clear();
//	chat_entries.clear();
//
//	// TODO P1: Implement OnLobbyStatus event
////	game_thread.broadcast_on_thread(OnLobbyStatus{*it});
//}

void Lobby::process(SnapshotLobby&& message) {
	// TODO P2: Currently SnapshotLobby has to be the first message otherwise the onClientLeave messages are skipped
	//			So implement outer joins

	static_cast<SnapshotLobby&>(*this) = std::move(message);

	for (const auto& client : clients)
		game_thread.broadcast_on_thread(OnClientJoin{client});

	for (const auto& entry : chat_entries)
		game_thread.broadcast_on_thread(OnChatMessage{entry});
}

// -------------------------------------------------------------------------------------------------

} // namespace space
