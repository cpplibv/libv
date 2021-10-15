// Project: libv, File: app/space/network/lobby.hpp

#pragma once

// libv
//#include <libv/math/vec.hpp>
//#include <libv/ui/chrono.hpp>
// std
#include <chrono>
#include <string>
#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Lobby {
	struct ChatEntry {
		std::string sender;
		std::string message;
	};

//	struct Player {
//		std::string name;
//	};

//	std::vector<Player> players;
	std::vector<ChatEntry> chat_entries;
};

//struct MPLobbyStatus {
//	struct ConnectedClients {
//		std::chrono::system_clock::time_point connected_at;
////		Connection connection;
////		float ping;
////		std::string name;
//	};
//
//	std::vector<ConnectedClients> clients;
//
//public:
//	void on_connect();
//	void on_disconnect();
//	void on_update();
//};

// -------------------------------------------------------------------------------------------------

} // namespace app
