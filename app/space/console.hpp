// Project: libv, File: app/space/console.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/math/vec.hpp>
//#include <libv/math/vec_serial.hpp>
//#include <libv/meta/reflection_access.hpp>
//#include <libv/serial/enable.hpp>
//#include <libv/serial/types/std_string.hpp>
//#include <libv/utility/to_underlying.hpp>
// std
//#include <cstdint>
//#include <string>
//#include <string_view>
//#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

//class ImplConsole;

//struct Console {
//	std::shared_ptr<ImplConsole> self;
//
//	inline Console(const Console&) noexcept = default;
//	inline Console& operator=(const Console&) & noexcept = default;
//	inline Console(Console&&) noexcept = default;
//	inline Console& operator=(Console&&) & noexcept = default;
//
////	void execute(std::string command);
//	void execute(std::string_view command) {
////		if (command == "clear_fleets") {
////			// clear_fleets
////			playout_delay_buffer.queue<app::CommandClearFleets>();
////
////		} else if (command.starts_with("name ")) {
////			// host <name>
////			player.name = arg(1);
////
////		} else if (command.starts_with("host ")) {
////			// host <port>
////			client.reset();
////			server.emplace(arg(1), playout_delay_buffer);
////
////		} else if (command.starts_with("join ")) {
////			// join <address> <port>
////			server.reset();
////			client.emplace(arg(1), arg(2), player.name, playout_delay_buffer);
////		}
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace app
