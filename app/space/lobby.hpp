// Project: libv, File: app/space/lobby.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/math/vec.hpp>
//#include <libv/ui/chrono.hpp>
// std
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

// -------------------------------------------------------------------------------------------------

} // namespace app
