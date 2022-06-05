// Project: libv.utility, File: src/libv/utility/nexus.hpp

#pragma once

// fwd
#include <libv/utility/nexus_fwd.hpp>
// libv
#include <libv/utility/type_key.hpp>
#include <libv/meta/force_inline.hpp>
// std
#include <functional>
#include <memory>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Performance analysis:
// https://godbolt.org/z/Kfexxh4f4
// https://quick-bench.com/q/-8xyfuQaWaG5-mYHGuhXoSL0I3w
// https://quick-bench.com/q/xrORehjQdzzC2ZMGDcKcog_KQUM

// -------------------------------------------------------------------------------------------------

class ImplNexus;

/// Handler type
class Nexus {
public:
	using track_ptr = const void*;
	using key_type = libv::type_uid;

private:
	std::shared_ptr<ImplNexus> self;

public:
	Nexus();
	inline Nexus(const Nexus&) noexcept = default;
	inline Nexus& operator=(const Nexus&) & noexcept = default;
	inline Nexus(Nexus&&) noexcept = default;
	inline Nexus& operator=(Nexus&&) & noexcept = default;
	~Nexus();

private:
	void aux_connect(track_ptr owner, key_type event_type, std::function<void(const void*)> func);
	void aux_broadcast(key_type event_type, const void* event_ptr) const;
	void aux_disconnect(track_ptr owner, key_type event_type);
	void aux_disconnect_all(track_ptr owner);

public:
	template <typename Event, typename Func>
	inline void connect(Func&& func) {
		connect<Event>(nullptr, std::forward<Func>(func));
	}

	template <typename Event, typename Func>
	inline void connect(track_ptr owner, Func&& func) {
		aux_connect(owner, libv::type_key<Event>(), [f = std::forward<Func>(func)](const void* event_ptr) mutable {
			if constexpr(std::is_invocable_v<Func, const Event&>)
				f(*static_cast<const Event*>(event_ptr));
			else
				f();
		});
	}

	template <typename Event>
	inline void broadcast(const Event& event) const {
		aux_broadcast(libv::type_key<Event>(), &event);
	}

	template <typename Event, typename... Args>
	inline void broadcast(Args&&... args) const {
		Event event{std::forward<Args>(args)...};
		aux_broadcast(libv::type_key<Event>(), &event);
	}

	template <typename Event>
	inline void disconnect(track_ptr owner) {
		aux_disconnect(owner, libv::type_key<Event>());
	}

	inline void disconnect_all(track_ptr owner) {
		aux_disconnect_all(owner);
	}
};

// =================================================================================================

class ImplNexus2;

/// Handler type
class Nexus2 {
public:
	using track_ptr = const void*;
	using key_type = libv::type_uid;

private:
	std::shared_ptr<ImplNexus2> self;

public:
	Nexus2();
	inline Nexus2(const Nexus2&) noexcept = default;
	inline Nexus2& operator=(const Nexus2&) & noexcept = default;
	inline Nexus2(Nexus2&&) noexcept = default;
	inline Nexus2& operator=(Nexus2&&) & noexcept = default;
	~Nexus2();

private:
	void aux_connect(track_ptr channel_owner, track_ptr slot_owner, key_type event_type, std::function<void(const void*)> func);
	void aux_connect_and_call(track_ptr channel_owner, track_ptr slot_owner, key_type event_type, std::function<void(const void*)> func, const void* event_ptr);
	void aux_broadcast(track_ptr channel_owner, key_type event_type, const void* event_ptr) const;
	void aux_disconnect_all(track_ptr owner);
	void aux_disconnect_channel(track_ptr channel_owner, key_type event_type);
	void aux_disconnect_channel_all(track_ptr channel_owner);
	void aux_disconnect_slot(track_ptr slot_owner, key_type event_type);
	void aux_disconnect_slot_all(track_ptr slot_owner);

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE std::function<void(const void*)> aux_make_callback(Func&& func) {
		return [f = std::forward<Func>(func)](const void* event_ptr) mutable {
			if constexpr(std::is_invocable_v<Func, const Event&>)
				f(*static_cast<const Event*>(event_ptr));
			else
				f(); // Enclosing lambda still required to discard the event_ptr
		};
	}

public:
	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global(Func&& func) {
		aux_connect(nullptr, nullptr, libv::type_key<Event>(), aux_make_callback<Event>(std::forward<Func>(func)));
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global(track_ptr slot_owner, Func&& func) {
		aux_connect(nullptr, slot_owner, libv::type_key<Event>(), aux_make_callback<Event>(std::forward<Func>(func)));
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global_and_call(Func&& func, const Event& event) {
		aux_connect_and_call(nullptr, nullptr, libv::type_key<Event>(), aux_make_callback<Event>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global_and_call(track_ptr slot_owner, Func&& func, const Event& event) {
		aux_connect_and_call(nullptr, slot_owner, libv::type_key<Event>(), aux_make_callback<Event>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_channel(track_ptr channel_owner, Func&& func) {
		aux_connect(channel_owner, nullptr, libv::type_key<Event>(), aux_make_callback<Event>(std::forward<Func>(func)));
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_channel(track_ptr channel_owner, track_ptr slot_owner, Func&& func) {
		aux_connect(channel_owner, slot_owner, libv::type_key<Event>(), aux_make_callback<Event>(std::forward<Func>(func)));
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_channel_and_call(track_ptr channel_owner, Func&& func, const Event& event) {
		aux_connect_and_call(channel_owner, nullptr, libv::type_key<Event>(), aux_make_callback<Event>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_channel_and_call(track_ptr channel_owner, track_ptr slot_owner, Func&& func, const Event& event) {
		aux_connect_and_call(channel_owner, slot_owner, libv::type_key<Event>(), aux_make_callback<Event>(std::forward<Func>(func)), &event);
	}

	template <typename Event>
	LIBV_FORCE_INLINE void broadcast_global(const Event& event) const {
		aux_broadcast(nullptr, libv::type_key<Event>(), &event);
	}

	template <typename Event, typename... Args>
	LIBV_FORCE_INLINE void broadcast_global(Args&&... args) const {
		Event event{std::forward<Args>(args)...};
		aux_broadcast(nullptr, libv::type_key<Event>(), &event);
	}

	template <typename Event>
	LIBV_FORCE_INLINE void broadcast_channel(track_ptr channel_owner, const Event& event) const {
		aux_broadcast(channel_owner, libv::type_key<Event>(), &event);
	}

	template <typename Event, typename... Args>
	LIBV_FORCE_INLINE void broadcast_channel(track_ptr channel_owner, Args&&... args) const {
		Event event{std::forward<Args>(args)...};
		aux_broadcast(channel_owner, libv::type_key<Event>(), &event);
	}

	LIBV_FORCE_INLINE void disconnect_all(track_ptr owner) {
		aux_disconnect_all(owner);
	}

	template <typename Event>
	LIBV_FORCE_INLINE void disconnect_channel(track_ptr channel_owner) {
		aux_disconnect_channel(channel_owner, libv::type_key<Event>());
	}

	LIBV_FORCE_INLINE void disconnect_channel_all(track_ptr channel_owner) {
		aux_disconnect_channel_all(channel_owner);
	}

	template <typename Event>
	LIBV_FORCE_INLINE void disconnect_slot(track_ptr slot_owner) {
		aux_disconnect_slot(slot_owner, libv::type_key<Event>());
	}

	LIBV_FORCE_INLINE void disconnect_slot_all(track_ptr slot_owner) {
		aux_disconnect_slot_all(slot_owner);
	}

public:
	[[nodiscard]] std::size_t num_channel() const noexcept;
	[[nodiscard]] std::size_t num_tracked() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
