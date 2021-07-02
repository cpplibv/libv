// Project: libv, File: app/space/icons.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/math/vec.hpp>
//#include <libv/ui/chrono.hpp>
// std
#include <string>
#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

struct SpaceSession {
	struct ChatEntries {
		std::string sender;
		std::string message;
	};

//	std::vector<Player> players;
	std::vector<ChatEntries> chat_entries;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
