// Project: libv, File: app/space/game/game_thread.cpp

// hpp
#include <space/game/game_thread.hpp>
// libv
#include <libv/ui/ui.hpp>
//#include <libv/utility/nexus.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

GameThread::GameThread(libv::ui::UI& ui, libv::Nexus2& nexus) :
	ui(ui),
	nexus(nexus) {
}

void GameThread::execute(std::function<void()> task) {
//void GameThread::execute(libv::unique_function<void()> task) {
	ui.execute_in_ui_loop(std::move(task));
}

// -------------------------------------------------------------------------------------------------

} // namespace space
