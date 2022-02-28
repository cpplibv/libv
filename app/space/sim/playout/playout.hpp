// Project: libv, File: app/space/sim/playout/playout.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/serial/archive/binary_fwd.hpp>
// std
#include <memory>
#include <mutex>
#include <vector>
//#include <random>
// pro
#include <space/network/codec.hpp>
#include <space/network/network_client.hpp>
#include <space/network/network_server.hpp>
#include <space/sim/engine/chrono.hpp>
#include <space/sim/simulation.hpp> // Only to access the process member function


namespace space {

// -------------------------------------------------------------------------------------------------

class Playout {
	using apply_func_t = void(*)(Simulation&, void*);

	struct StateChangeEntry {
		//		TickIndex frameIndex;
		// TODO P1: Temp shared_ptr usage, replace it with a cache local solution (packed variant_queue), (shared_ptr used for deleter)
		std::shared_ptr<void> command;
		apply_func_t apply_func;
	};

	template <typename CTO> struct ApplyFunc {
//		static void call(Universe& universe, void* cto) {
//			// !!!
//			universe.process(std::move(*reinterpret_cast<CTO*>(cto)));
		static void call(Simulation& simulation, void* cto) {
			simulation.process(std::move(*reinterpret_cast<CTO*>(cto)));
		}
	};

private:
	//TickIndex currentTickIndex;
	mutable std::mutex mutex;
	std::vector<StateChangeEntry> stateChangeEntries; // TODO P1: Use some kind of ring buffer (or deque like)

private:
	NetworkClient* network_client = nullptr;
	NetworkServer* network_server = nullptr;

//private:
public:
	std::unique_ptr<Simulation> simulation;

public:
	explicit Playout();
	explicit Playout(NetworkClient& network_client);
	explicit Playout(NetworkServer& network_server);
	virtual	~Playout();

public:
	template <typename CTO, typename... Args>
	void process(Args&&... args);

	template <typename CTO>
	void process_from_network(CTO&& cto);

//	void process_snapshot(std::shared_ptr<libv::archive::BinaryInput>&& iar);

public:
	void saveSimulationSnapshot(libv::archive::BinaryOutput& ar, SnapshotType mode) const;
	void loadSimulationSnapshot(libv::archive::BinaryInput& ar, SnapshotType mode);

public:
	void update(time_duration delta_time);
};

// -------------------------------------------------------------------------------------------------

template <typename CTO, typename... Args>
void Playout::process(Args&&... args) {
	if (network_server) {
		auto cto = std::make_shared<CTO>(std::forward<Args>(args)...);
		network_server->broadcast(network_encode(*cto));

		auto lock = std::unique_lock(mutex);
		stateChangeEntries.emplace_back(std::move(cto), ApplyFunc<CTO>::call);

	} else if (network_client) {
		CTO cto{std::forward<Args>(args)...};
		network_client->send(network_encode(cto));

	} else {
		auto cto = std::make_shared<CTO>(std::forward<Args>(args)...);
		auto lock = std::unique_lock(mutex);
		stateChangeEntries.emplace_back(std::move(cto), ApplyFunc<CTO>::call);
	}
}

template <typename CTO>
void Playout::process_from_network(CTO&& cto) {
	auto cto_sp = std::make_shared<CTO>(std::move(cto));
	auto lock = std::unique_lock(mutex);
	stateChangeEntries.emplace_back(std::move(cto_sp), ApplyFunc<CTO>::call);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
