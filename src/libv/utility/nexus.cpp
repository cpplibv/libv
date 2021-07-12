// Project: libv.utility, File: src/libv/utility/nexus.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/utility/nexus.hpp>
// libv
#include <libv/algo/erase_all_unstable.hpp>
#include <libv/algo/erase_unstable.hpp>
#include <libv/algo/linear_contains.hpp>
// std
#include <cassert>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

class ImplNexus {
public:
	using track_ptr = Nexus::track_ptr;
	using key_type = Nexus::key_type;

	struct Target {
		track_ptr owner;
		std::function<void(const void*)> callback;
	};

public:
	std::recursive_mutex mutex;
	std::unordered_map<key_type, std::vector<Target>> channels;
	std::unordered_map<track_ptr, std::vector<key_type>> channel_memberships;
};

// -------------------------------------------------------------------------------------------------

Nexus::Nexus() :
	self(std::make_shared<ImplNexus>()) { }

Nexus::~Nexus() {
	// For the sake of forward declared unique_ptr
}

void Nexus::aux_connect(track_ptr owner, key_type event_type, std::function<void(const void*)> func) {
	auto lock = std::unique_lock(self->mutex);

	auto& memberships = self->channel_memberships[owner];
	if (!libv::linear_contains(memberships, event_type))
		memberships.emplace_back(event_type);

	self->channels[event_type].emplace_back(owner, std::move(func));
}

void Nexus::aux_broadcast(key_type event_type, const void* event_ptr) const {
	auto lock = std::unique_lock(self->mutex);

	const auto it = self->channels.find(event_type);
	if (it != self->channels.end())
		for (const auto& target : it->second)
			target.callback(event_ptr);
}

void Nexus::aux_disconnect(track_ptr owner, key_type event_type) {
	auto lock = std::unique_lock(self->mutex);

	// Remove entry from: channel_memberships
	const auto ms_it = self->channel_memberships.find(owner);
	if (ms_it == self->channel_memberships.end())
		return; // Could happen when conditional subscription and unconditional unsubscription is used, Sounds reasonable, Therefore it is allowed

	libv::erase_unstable(ms_it->second, event_type);
	if (ms_it->second.empty())
		self->channel_memberships.erase(ms_it);

	// Remove entry from: channels
	const auto ch_it = self->channels.find(event_type);
	if (ch_it == self->channels.end()) {
		assert(false && "Internal consistency violation"); // channel_memberships indicated, but channels is missing a member (The find check on channel_memberships would have early exited otherwise)
		return;
	}

	libv::erase_all_unstable(ch_it->second, owner, &ImplNexus::Target::owner);
	if (ch_it->second.empty())
		self->channels.erase(ch_it);
}

void Nexus::aux_disconnect_all(track_ptr owner) {
	auto lock = std::unique_lock(self->mutex);

	const auto ms_it = self->channel_memberships.find(owner);
	if (ms_it == self->channel_memberships.end())
		return; // Could happen when conditional subscription and unconditional unsubscription is used, Sounds reasonable, Therefore it is allowed

	for (const auto event_type : ms_it->second) {
		const auto ch_it = self->channels.find(event_type);
		if (ch_it == self->channels.end()) {
			assert(false && "Internal consistency violation"); // channel_memberships indicated, but channels is missing a member
			continue;
		}

		libv::erase_all_unstable(ch_it->second, owner, &ImplNexus::Target::owner);
		if (ch_it->second.empty())
			self->channels.erase(ch_it);
	}

	self->channel_memberships.erase(ms_it);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
