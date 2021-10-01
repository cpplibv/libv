// Project: libv, File: app/space/playout.cpp

// hpp
#include <space/playout.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void Playout::update(Universe& universe) {
	auto lock = std::unique_lock(mutex);

	for (auto& entry : stateChangeEntries)
		entry.apply_func(universe, entry.command.get());

	stateChangeEntries.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace app
