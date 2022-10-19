// Project: libv.utility, File: src/libv/utility/nexus.cpp

// hpp
#include <libv/utility/nexus.hpp>
// libv
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/algo/erase_stable.hpp>
#include <libv/algo/erase_unstable.hpp>
#include <libv/algo/linear_contains.hpp>
#include <libv/utility/hash.hpp>
//#include <libv/mt/mutex_spinlock.hpp> // Would lead to cyclic dependency between mt <-> utility
// std
#include <cassert>
#include <mutex>
#include <unordered_map>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct NexusChannelKey {
	Nexus::channel_ptr channel_owner;
	Nexus::type_uid type;

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const NexusChannelKey&) const noexcept = default;
};

struct NexusObjectKeyView {
	std::string_view name;
	Nexus::type_uid type;
};

struct NexusObjectKey {
	std::string name;
	Nexus::type_uid type;

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const NexusObjectKey&) const noexcept = default;
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const NexusObjectKeyView& rhs) const noexcept {
		return type == rhs.type && name == rhs.name;
	}
};

} // namespace libv --------------------------------------------------------------------------------

LIBV_MAKE_HASHABLE(::libv::NexusChannelKey, t.channel_owner, t.type);
LIBV_MAKE_HASHABLE(::libv::NexusObjectKey, t.name, t.type);
LIBV_MAKE_HASHABLE(::libv::NexusObjectKeyView, t.name, t.type);

namespace libv { // --------------------------------------------------------------------------------

struct NexusObjectKeyHasher {
	using is_transparent = void;

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE std::size_t operator()(const NexusObjectKey& var) const noexcept {
		return std::hash<NexusObjectKey>{}(var);
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE std::size_t operator()(const NexusObjectKeyView& var) const noexcept {
		return std::hash<NexusObjectKeyView>{}(var);
	}
};

// -------------------------------------------------------------------------------------------------

struct ImplNexus {
	using channel_ptr = Nexus::channel_ptr;
	using slot_ptr = Nexus::slot_ptr;
	using channel_or_slot_ptr = Nexus::channel_or_slot_ptr;
	using type_uid = Nexus::type_uid;

public:
	struct Target {
		slot_ptr slot_owner;
		libv::unique_function<void(channel_ptr, const void*)> callback;
	};

	std::recursive_mutex channel_m;
	std::unordered_map<NexusChannelKey, std::vector<Target>> channels;
	std::unordered_map<channel_or_slot_ptr, std::vector<NexusChannelKey>> memberships;
	// NOTE: unordered_map's iterator stability is relayed upon during multiple .erase(it) calls

public:
	using key_type = NexusObjectKey;

	std::mutex objects_m;
	std::unordered_map<key_type, Nexus::object_ptr, NexusObjectKeyHasher, std::equal_to<>> objects;
};

// =================================================================================================

Nexus::Nexus() :
	self(std::make_shared<ImplNexus>()) { }

// -------------------------------------------------------------------------------------------------

void Nexus::aux_object_view_set(Nexus::type_uid object_type, Nexus::object_ptr ptr) {
	auto lock = std::unique_lock(self->objects_m);
	self->objects.insert_or_assign(NexusObjectKey{"", object_type}, ptr);
}

void Nexus::aux_object_view_set(std::string name, Nexus::type_uid object_type, Nexus::object_ptr ptr) {
	auto lock = std::unique_lock(self->objects_m);
	self->objects.insert_or_assign(NexusObjectKey{std::move(name), object_type}, ptr);
}

Nexus::object_ptr Nexus::aux_object_view_get(Nexus::type_uid object_type) const noexcept {
	auto lock = std::unique_lock(self->objects_m);
	const auto it = self->objects.find(NexusObjectKeyView{"", object_type});
	return it == self->objects.end() ? nullptr : it->second;
}

Nexus::object_ptr Nexus::aux_object_view_get(const std::string_view name, Nexus::type_uid object_type) const noexcept {
	auto lock = std::unique_lock(self->objects_m);
	const auto it = self->objects.find(NexusObjectKeyView{name, object_type});
	return it == self->objects.end() ? nullptr : it->second;
}

void Nexus::aux_object_view_remove(Nexus::type_uid object_type) const noexcept {
	auto lock = std::unique_lock(self->objects_m);
	self->objects.erase(NexusObjectKey{"", object_type});
}

void Nexus::aux_object_view_remove(const std::string_view name, Nexus::type_uid object_type) const noexcept {
	auto lock = std::unique_lock(self->objects_m);
	const auto it = self->objects.find(NexusObjectKeyView{name, object_type});
	if (it != self->objects.end())
		self->objects.erase(it);
}

// -------------------------------------------------------------------------------------------------

std::vector<ImplNexus::Target>& aux_create_channel(ImplNexus& self, ImplNexus::channel_ptr channel_owner, ImplNexus::slot_ptr slot_owner, ImplNexus::type_uid event_type) {
	const auto channel_key = NexusChannelKey{channel_owner, event_type};

	auto& memberships_of_channel = self.memberships[channel_owner];
	if (!libv::linear_contains(memberships_of_channel, channel_key))
		memberships_of_channel.emplace_back(channel_key);

	auto& memberships_of_slot = self.memberships[slot_owner];
	if (!libv::linear_contains(memberships_of_slot, channel_key))
		memberships_of_slot.emplace_back(channel_key);

	return self.channels[channel_key];
}

void Nexus::aux_connect(channel_ptr channel_owner, slot_ptr slot_owner, type_uid event_type, libv::unique_function<void(channel_ptr, const void*)>&& func) {
	auto lock = std::unique_lock(self->channel_m);
	auto& channel = aux_create_channel(*self, channel_owner, slot_owner, event_type);
	channel.emplace_back(slot_owner, std::move(func));
}

void Nexus::aux_connect_and_call(channel_ptr channel_owner, slot_ptr slot_owner, type_uid event_type, libv::unique_function<void(channel_ptr, const void*)>&& func, const void* event_ptr) {
	auto lock = std::unique_lock(self->channel_m);
	auto& channel = aux_create_channel(*self, channel_owner, slot_owner, event_type);
	auto& callback = channel.emplace_back(slot_owner, std::move(func)).callback;
	// WARNING: The callback mustn't modify the list of the subscribed entries of the same channel
	callback(channel_owner, event_ptr);
}

void Nexus::aux_connect_front(channel_ptr channel_owner, slot_ptr slot_owner, type_uid event_type, libv::unique_function<void(channel_ptr, const void*)>&& func) {
	auto lock = std::unique_lock(self->channel_m);
	auto& channel = aux_create_channel(*self, channel_owner, slot_owner, event_type);
	channel.emplace(channel.begin(), slot_owner, std::move(func));
}

void Nexus::aux_connect_front_and_call(channel_ptr channel_owner, slot_ptr slot_owner, type_uid event_type, libv::unique_function<void(channel_ptr, const void*)>&& func, const void* event_ptr) {
	auto lock = std::unique_lock(self->channel_m);
	auto& channel = aux_create_channel(*self, channel_owner, slot_owner, event_type);
	auto& callback = channel.emplace(channel.begin(), slot_owner, std::move(func))->callback;
	// WARNING: The callback mustn't modify the list of the subscribed entries of the same channel
	callback(channel_owner, event_ptr);
}

void Nexus::aux_broadcast(channel_ptr channel_owner, type_uid event_type, const void* event_ptr) const {
	const auto channel_key = NexusChannelKey{channel_owner, event_type};

	auto lock = std::unique_lock(self->channel_m);

	const auto it = self->channels.find(channel_key);
	if (it != self->channels.end())
		for (auto& target : it->second)
			// WARNING: The callback mustn't modify the list of the subscribed entries of the same channel
			target.callback(channel_owner, event_ptr);
}

void Nexus::aux_disconnect_all(channel_or_slot_ptr owner) {
	auto lock = std::unique_lock(self->channel_m);

	const auto ms_it = self->memberships.find(owner);
	if (ms_it == self->memberships.end())
		return; // Could happen when conditional subscription and unconditional unsubscription is used, Sounds reasonable, Therefore it is allowed

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
			libv::erase_all_stable(ch_it->second, owner, &ImplNexus::Target::slot_owner);

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

void Nexus::aux_disconnect_channel(channel_ptr channel_owner, type_uid event_type) {
	const auto channel_key = NexusChannelKey{channel_owner, event_type};

	auto lock = std::unique_lock(self->channel_m);

	const auto ms_it = self->memberships.find(channel_owner);
	if (ms_it == self->memberships.end())
		return; // Could happen when conditional subscription and unconditional unsubscription is used, Sounds reasonable, Therefore it is allowed

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

void Nexus::aux_disconnect_channel_all(channel_ptr channel_owner) {
	auto lock = std::unique_lock(self->channel_m);

	const auto ms_it = self->memberships.find(channel_owner);
	if (ms_it == self->memberships.end())
		return; // Could happen when conditional subscription and unconditional unsubscription is used, Sounds reasonable, Therefore it is allowed

	libv::erase_if_unstable(ms_it->second, [&](const NexusChannelKey& channel_key) {
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

void Nexus::aux_disconnect_slot(slot_ptr slot_owner, type_uid event_type) {
	auto lock = std::unique_lock(self->channel_m);

	const auto ms_it = self->memberships.find(slot_owner);
	if (ms_it == self->memberships.end())
		return; // Could happen when conditional subscription and unconditional unsubscription is used, Sounds reasonable, Therefore it is allowed

	libv::erase_if_unstable(ms_it->second, [&](const NexusChannelKey& channel_key) {
		if (channel_key.type != event_type)
			return false;

		const auto ch_it = self->channels.find(channel_key);
		assert(ch_it != self->channels.end() && "Internal consistency violation");

		libv::erase_all_stable(ch_it->second, slot_owner, &ImplNexus::Target::slot_owner);

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

void Nexus::aux_disconnect_slot_all(slot_ptr slot_owner) {
	auto lock = std::unique_lock(self->channel_m);

	const auto ms_it = self->memberships.find(slot_owner);
	if (ms_it == self->memberships.end())
		return; // Could happen when conditional subscription and unconditional unsubscription is used, Sounds reasonable, Therefore it is allowed

	libv::erase_if_unstable(ms_it->second, [&](const NexusChannelKey& channel_key) {
		const auto ch_it = self->channels.find(channel_key);
		assert(ch_it != self->channels.end() && "Internal consistency violation");

		libv::erase_all_stable(ch_it->second, slot_owner, &ImplNexus::Target::slot_owner);

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

// -------------------------------------------------------------------------------------------------

std::size_t Nexus::num_channel() const noexcept {
	auto lock = std::unique_lock(self->channel_m);
	return self->channels.size();
}

std::size_t Nexus::num_tracked() const noexcept {
	auto lock = std::unique_lock(self->channel_m);
	return self->memberships.size();
}

std::size_t Nexus::num_object() const noexcept {
	auto lock = std::unique_lock(self->objects_m);
	return self->objects.size();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
