// Project: libv, File: app/space/game_thread.hpp

#pragma once

// libv
#include <libv/ui/fwd.hpp>
//#include <libv/utility/nexus_fwd.hpp>
#include <libv/utility/nexus.hpp>
// std
#include <functional>


namespace app {

// -------------------------------------------------------------------------------------------------

class GameThread {
private:
	libv::ui::UI& ui;
	libv::Nexus& nexus;

public:
	GameThread(libv::ui::UI& ui, libv::Nexus& nexus);

public:
	void execute(std::function<void()> task);

	template <typename T>
	void broadcast_on_thread(T&& event) {
		execute([&n = nexus, e = std::forward<T>(event)] mutable {
			n.broadcast(std::forward<T>(e));
		});
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
