// Project: libv.utility, File: src/libv/utility/nexus.cpp

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

// hpp
#include <libv/utility/nexus.hpp>
// libv
#include <libv/algo/erase_all_unstable.hpp>
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/algo/erase_unstable.hpp>
#include <libv/algo/linear_contains.hpp>
#include <libv/utility/hash.hpp>
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
	// For the sake of forward declared ptr
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

// === Nexus 2 =====================================================================================

struct ChannelKey {
	Nexus2::track_ptr channel_owner;
	Nexus2::key_type type;

	[[nodiscard]] constexpr inline bool operator==(const ChannelKey&) const noexcept = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv

LIBV_MAKE_HASHABLE(::libv::ChannelKey, t.channel_owner, t.type);

namespace libv {

// -------------------------------------------------------------------------------------------------

class ImplNexus2 {
public:
	using track_ptr = Nexus2::track_ptr;
	using key_type = Nexus2::key_type;

	struct Target {
		track_ptr slot_owner;
		std::function<void(const void*)> callback;
	};

public:
	std::recursive_mutex mutex;
	std::unordered_map<ChannelKey, std::vector<Target>> channels;
	std::unordered_map<track_ptr, std::vector<ChannelKey>> memberships;
	// NOTE: unordered_map's iterator stability is relayed upon during multiple .erase(it) calls
};

// =================================================================================================

Nexus2::Nexus2() :
	self(std::make_shared<ImplNexus2>()) { }

Nexus2::~Nexus2() {
	// For the sake of forward declared ptr
}

void Nexus2::aux_connect(track_ptr channel_owner, track_ptr slot_owner, key_type event_type, std::function<void(const void*)> func) {
	const auto channel_key = ChannelKey{channel_owner, event_type};

	auto lock = std::unique_lock(self->mutex);

	auto& memberships_of_channel = self->memberships[channel_owner];
	if (!libv::linear_contains(memberships_of_channel, channel_key))
		memberships_of_channel.emplace_back(channel_key);

	auto& memberships_of_slot = self->memberships[slot_owner];
	if (!libv::linear_contains(memberships_of_slot, channel_key))
		memberships_of_slot.emplace_back(channel_key);

	self->channels[channel_key].emplace_back(slot_owner, std::move(func));
}

void Nexus2::aux_connect_and_call(track_ptr channel_owner, track_ptr slot_owner, key_type event_type, std::function<void(const void*)> func, const void* event_ptr) {
	// NOTE: func cannot be moved into aux_connect as func should be called without the mutex held
	//			this could be solved by relying on the unordered_map value stability, but there are plans to move
	//			to a different hash container
	aux_connect(channel_owner, slot_owner, event_type, func);
	func(event_ptr);
}

void Nexus2::aux_broadcast(track_ptr channel_owner, key_type event_type, const void* event_ptr) const {
	const auto channel_key = ChannelKey{channel_owner, event_type};

	auto lock = std::unique_lock(self->mutex);

	const auto it = self->channels.find(channel_key);
	if (it != self->channels.end())
		for (const auto& target : it->second)
			target.callback(event_ptr);
}

void Nexus2::aux_disconnect_all(track_ptr owner) {
	auto lock = std::unique_lock(self->mutex);

	const auto ms_it = self->memberships.find(owner);
	if (ms_it == self->memberships.end())
		return; // Could happen

	for (const auto& channel_key : ms_it->second) {
		const auto ch_it = self->channels.find(channel_key);
		assert(ch_it != self->channels.end() && "Internal consistency violation"); // memberships indicated, but channels is missing a member (The find check on memberships would have early exited otherwise)

		const auto is_channel_owner_in_channel = channel_key.channel_owner == owner;

		if (is_channel_owner_in_channel) {
			for (const auto& target : ch_it->second) {
				const auto ms_slot_it = self->memberships.find(target.slot_owner);
				assert(ms_slot_it != self->memberships.end() && "Internal consistency violation"); // memberships indicated, but channels is missing a member (The find check on memberships would have early exited otherwise)

				if (target.slot_owner == owner)
					// Skip attempting to remove channel's membership, outer loop will take care of it
					// This skip is necessary to not violate membership invariants with erase
					// Also erase could invalidate the erase_if_unstable iterator
					continue;

				libv::erase_unstable(ms_slot_it->second, channel_key);
				if (ms_slot_it->second.empty())
					self->memberships.erase(ms_slot_it);
			}

			self->channels.erase(ch_it);

		} else {
			libv::erase_all_unstable(ch_it->second, owner, &ImplNexus2::Target::slot_owner);

			if (ch_it->second.empty()) {
				self->channels.erase(ch_it);
				const auto ms_slot_it = self->memberships.find(channel_key.channel_owner);
				assert(ms_slot_it != self->memberships.end() && "Internal consistency violation");

				libv::erase_unstable(ms_slot_it->second, channel_key);
				if (ms_slot_it->second.empty())
					self->memberships.erase(ms_slot_it);
			}
		}
	}

	self->memberships.erase(ms_it);
}

void Nexus2::aux_disconnect_channel(track_ptr channel_owner, key_type event_type) {
	const auto channel_key = ChannelKey{channel_owner, event_type};

	auto lock = std::unique_lock(self->mutex);

	const auto ms_it = self->memberships.find(channel_owner);
	if (ms_it == self->memberships.end())
		return; // Could happen

	const auto ch_it = self->channels.find(channel_key);
	assert(ch_it != self->channels.end() && "Internal consistency violation"); // memberships indicated, but channels is missing a member (The find check on memberships would have early exited otherwise)

	for (const auto& target : ch_it->second) {
		const auto ms_slot_it = self->memberships.find(target.slot_owner);
		assert(ms_slot_it != self->memberships.end() && "Internal consistency violation"); // memberships indicated, but channels is missing a member (The find check on memberships would have early exited otherwise)

		libv::erase_unstable(ms_slot_it->second, channel_key);
		if (ms_slot_it->second.empty())
			self->memberships.erase(ms_slot_it);
	}

	self->channels.erase(ch_it);

	libv::erase_unstable(ms_it->second, channel_key);
	if (ms_it->second.empty())
		self->memberships.erase(ms_it);
}

void Nexus2::aux_disconnect_channel_all(track_ptr channel_owner) {
	auto lock = std::unique_lock(self->mutex);

	const auto ms_it = self->memberships.find(channel_owner);
	if (ms_it == self->memberships.end())
		return; // Could happen

	libv::erase_if_unstable(ms_it->second, [&](const ChannelKey& channel_key) {
		if (channel_key.channel_owner != channel_owner)
			return false; // Skip memberships where owner is not the channel

		const auto ch_it = self->channels.find(channel_key);
		assert(ch_it != self->channels.end() && "Internal consistency violation"); // memberships indicated, but channels is missing a member (The find check on memberships would have early exited otherwise)

		for (const auto& target : ch_it->second) {
			if (target.slot_owner == channel_owner)
				// Skip attempting to remove channel_owner's membership, outer loop will take care of it
				// This skip is necessary to not violate membership invariants with erase
				// Also erase could invalidate the erase_if_unstable iterator
				continue;

			const auto ms_slot_it = self->memberships.find(target.slot_owner);
			assert(ms_slot_it != self->memberships.end() && "Internal consistency violation"); // memberships indicated, but channels is missing a member (The find check on memberships would have early exited otherwise)

			libv::erase_unstable(ms_slot_it->second, channel_key);
			if (ms_slot_it->second.empty())
				self->memberships.erase(ms_slot_it);
		}

		self->channels.erase(ch_it);
		return true;
	});

	if (ms_it->second.empty())
		self->memberships.erase(ms_it);
}

void Nexus2::aux_disconnect_slot(track_ptr slot_owner, key_type event_type) {
	auto lock = std::unique_lock(self->mutex);

	const auto ms_it = self->memberships.find(slot_owner);
	if (ms_it == self->memberships.end())
		return; // Could happen

	libv::erase_if_unstable(ms_it->second, [&](const ChannelKey& channel_key) {
		if (channel_key.type != event_type)
			return false;

		const auto ch_it = self->channels.find(channel_key);
		assert(ch_it != self->channels.end() && "Internal consistency violation");

		libv::erase_all_unstable(ch_it->second, slot_owner, &ImplNexus2::Target::slot_owner);

		if (ch_it->second.empty()) {
			self->channels.erase(ch_it);
			const auto ms_slot_it = self->memberships.find(channel_key.channel_owner);
			assert(ms_slot_it != self->memberships.end() && "Internal consistency violation");

			libv::erase_unstable(ms_slot_it->second, channel_key);
			if (ms_slot_it->second.empty())
				self->memberships.erase(ms_slot_it);

			return true; // Channel became empty, remove membership even if it was the channel
		}

		return channel_key.channel_owner != slot_owner; // If it was a slot in the channel, remove membership
	});

	if (ms_it->second.empty())
		self->memberships.erase(ms_it);
}

void Nexus2::aux_disconnect_slot_all(track_ptr slot_owner) {
	auto lock = std::unique_lock(self->mutex);

	const auto ms_it = self->memberships.find(slot_owner);
	if (ms_it == self->memberships.end())
		return; // Could happen

	libv::erase_if_unstable(ms_it->second, [&](const ChannelKey& channel_key) {
		const auto ch_it = self->channels.find(channel_key);
		assert(ch_it != self->channels.end() && "Internal consistency violation");

		libv::erase_all_unstable(ch_it->second, slot_owner, &ImplNexus2::Target::slot_owner);

		if (ch_it->second.empty()) {
			self->channels.erase(ch_it);
			const auto ms_slot_it = self->memberships.find(channel_key.channel_owner);
			assert(ms_slot_it != self->memberships.end() && "Internal consistency violation");

			libv::erase_unstable(ms_slot_it->second, channel_key);
			if (ms_slot_it->second.empty())
				self->memberships.erase(ms_slot_it);

			return true; // Channel became empty, remove membership even if it was the channel
		}

		return channel_key.channel_owner != slot_owner; // If it was a slot in the channel, remove membership
	});

	if (ms_it->second.empty())
		self->memberships.erase(ms_it);
}

std::size_t Nexus2::num_channel() const noexcept {
	auto lock = std::unique_lock(self->mutex);
	return self->channels.size();
}

std::size_t Nexus2::num_tracked() const noexcept {
	auto lock = std::unique_lock(self->mutex);
	return self->memberships.size();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
