//

#include <libv/ctrl/binding_register.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/utility/chrono.hpp>
// #include <libv/ctrl/fwd.hpp>
#include <star/game/control/requests.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct DummyConsole {
public:
	static void console_commands() {
		const auto act = [&](std::string&& name, auto&& fnOrPrototype) {
			commands.emplace(std::move(name), fnOrPrototype);
		};

		act("pause", RequestTimeTogglePause{});
		act("gameSpeed", []{int speed} { return RequestTimeSpeed{speed}; });
		act("slowMode", []{bool value} { return RequestTimeDebugSlowMode{value}; });
		act("stepFrame", RequestTimeDebugStepFrame{});
	}
};

// -------------------------------------------------------------------------------------------------

struct DummyNetwork {
public:
	std::map<...> commands;
	template <typename T>
	void reg() {
		commands.empalace([]{
			auto cmd = parsePacketToCommand();
			if (checkPeerPermissions(cmd))
				return outcome::peerIsStupid;

			nexus.boardcast_channel(gameSessionChannel, cmd);
		});
	}
	static void console_commands() {
		reg<RequestTimeTogglePause>();
		reg<RequestTimeSpeed>();
		reg<RequestTimeDebugSlowMode>();
		reg<RequestTimeDebugStepFrame>();
	}

	void processMessage(std::string x) {
		commands[x.id](x);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace star
