// Project: libv, File: app/space/playout.cpp

// hpp
#include <space/playout.hpp>
// libv
#include <libv/serial/archive/binary.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

//void Playout::save(libv::archive::Binary::output& ar) const {
//	const_cast<Playout*>(this)->serialize(ar);
//}
//
//void Playout::load(libv::archive::Binary::input& ar) {
//	serialize(ar);
//}

// -------------------------------------------------------------------------------------------------

void Playout::update(Universe& universe) {
	auto lock = std::unique_lock(mutex);

	for (auto& entry : stateChangeEntries)
		entry.apply_func(universe, entry.command.get());

	stateChangeEntries.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace space
