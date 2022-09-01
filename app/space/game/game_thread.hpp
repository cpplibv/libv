// Project: libv, File: app/space/game/game_thread.hpp

#pragma once

// libv
#include <libv/ui/fwd.hpp>
//#include <libv/utility/nexus_fwd.hpp>
//#include <libv/utility/unique_function.hpp>
#include <libv/utility/nexus.hpp>
//// std
//#include <functional>


namespace space {

// -------------------------------------------------------------------------------------------------

class GameThread {
private:
	libv::ui::UI& ui;
	libv::Nexus2& nexus;

public:
	GameThread(libv::ui::UI& ui, libv::Nexus2& nexus);

public:
	void execute(std::function<void()> task);
//	void execute(libv::unique_function<void()> task);

	template <typename T>
	void broadcast_on_thread(T&& event) {
		execute([&n = nexus, e = std::forward<T>(event)] mutable {
			n.broadcast_global(std::forward<T>(e));
		});
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
