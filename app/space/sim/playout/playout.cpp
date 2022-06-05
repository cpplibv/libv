// Project: libv, File: app/space/sim/playout/playout.cpp

// hpp
#include <space/sim/playout/playout.hpp>
//// libv
//#include <libv/serial/archive/binary.hpp>
// pro
#include <space/sim/simulation.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Playout::Playout() :
	simulation(std::make_unique<Simulation>()) {
}

Playout::Playout(NetworkClient& network_client) :
	network_client(&network_client),
	simulation(std::make_unique<Simulation>()) {
}

Playout::Playout(NetworkServer& network_server) :
	network_server(&network_server),
	simulation(std::make_unique<Simulation>()) {
}

Playout::~Playout() {
	// For the sake of forward declared ptr
}

// -------------------------------------------------------------------------------------------------

//void Playout::process_snapshot(std::shared_ptr<libv::archive::BinaryInput>&& iar) {
////		SnapshotArchive<libv::archive::BinaryInput> snapshot_ar{*iar, context, SnapshotType::shared};
////		simulation.loadSnapshot(*iar);
//	0;
//}

void Playout::saveSimulationSnapshot(libv::archive::BinaryOutput& ar, SnapshotType mode) const {
	auto lock = std::unique_lock(mutex);

	simulation->saveSnapshot(ar, mode);
}

void Playout::loadSimulationSnapshot(libv::archive::BinaryInput& ar, SnapshotType mode) {
	auto lock = std::unique_lock(mutex);

	simulation->loadSnapshot(ar, mode);
}

// -------------------------------------------------------------------------------------------------

void Playout::update(time_duration delta_time) {
	auto lock = std::unique_lock(mutex);

	for (auto& entry : stateChangeEntries)
		entry.apply_func(*simulation, entry.command.get());

	stateChangeEntries.clear();

	// TODO P1: app.space: Sim time not delta time, or playout licensed tick count
	simulation->update(delta_time);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
