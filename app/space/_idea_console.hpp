// Project: libv, File: app/space/_idea_console.hpp

//#pragma once
//
//// libv
////#include <libv/math/vec.hpp>
////#include <libv/math/vec_serial.hpp>
////#include <libv/meta/reflection_access.hpp>
////#include <libv/serial/enable.hpp>
////#include <libv/serial/types/std_string.hpp>
////#include <libv/utility/to_underlying.hpp>
//// std
//#include <string_view>
//#include <memory>
//
//
//namespace space {
//
//// -------------------------------------------------------------------------------------------------
//
//class ImplConsole;
//
//class Console {
//private:
//	std::shared_ptr<ImplConsole> self;
//
//public:
//	Console();
//	~Console();
//
//public:
//	void execute(std::string_view command) {
////		if (command == "clear_fleets") {
////			// clear_fleets
////			playout_delay_buffer.queue<CommandClearFleets>();
////
////		} else if (command.starts_with("name ")) {
////			// host <name>
////			user.name = arg(1);
////
////		} else if (command.starts_with("host ")) {
////			// host <port>
////			client.reset();
////			server.emplace(arg(1), playout_delay_buffer);
////
////		} else if (command.starts_with("join ")) {
////			// join <address> <port>
////			server.reset();
////			client.emplace(arg(1), arg(2), user.name, playout_delay_buffer);
////		}
//	}
//};
//
// -------------------------------------------------------------------------------------------------
//
//
//Console::Console() :
//	self(std::make_unique<ImplConsole>()) {
//}
//
//Console::~Console() {
//	// For the sake of forward declared types
//}
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace space
