// Project: libv.utility, File: src/libv/utility/nexus.hpp

#pragma once

// fwd
#include <libv/utility/nexus_fwd.hpp>
// libv
#include <libv/meta/always.hpp>
#include <libv/meta/force_inline.hpp>
#include <libv/utility/type_key.hpp>
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
	using channel_ptr = const void*;
	using slot_ptr = const void*;
	using channel_or_slot_ptr = const void*;
	using key_type = libv::type_uid;

private:
	std::shared_ptr<ImplNexus> self;

public:
	Nexus();
	inline Nexus(const Nexus&) noexcept = default;
	inline Nexus& operator=(const Nexus&) & noexcept = default;
	inline Nexus(Nexus&&) noexcept = default;
	inline Nexus& operator=(Nexus&&) & noexcept = default;
	inline ~Nexus() = default;

private:
	void aux_connect(channel_ptr channel_owner, slot_ptr slot_owner, key_type event_type, std::function<void(channel_ptr, const void*)>&& func);
	void aux_connect_and_call(channel_ptr channel_owner, slot_ptr slot_owner, key_type event_type, std::function<void(channel_ptr, const void*)>&& func, const void* event_ptr);
	void aux_connect_front(channel_ptr channel_owner, slot_ptr slot_owner, key_type event_type, std::function<void(channel_ptr, const void*)>&& func);
	void aux_connect_front_and_call(channel_ptr channel_owner, slot_ptr slot_owner, key_type event_type, std::function<void(channel_ptr, const void*)>&& func, const void* event_ptr);
	void aux_broadcast(channel_ptr channel_owner, key_type event_type, const void* event_ptr) const;
	void aux_disconnect_all(channel_or_slot_ptr owner);
	void aux_disconnect_channel(channel_ptr channel_owner, key_type event_type);
	void aux_disconnect_channel_all(channel_ptr channel_owner);
	void aux_disconnect_slot(slot_ptr slot_owner, key_type event_type);
	void aux_disconnect_slot_all(slot_ptr slot_owner);

	template <typename Event, typename Func>
	[[nodiscard]] LIBV_FORCE_INLINE std::function<void(channel_ptr, const void*)> aux_make_callback_global(Func&& func) {
		return [f = std::forward<Func>(func)](channel_ptr, const void* event_ptr) mutable {
			if constexpr(std::is_invocable_v<Func, const Event&>)
				f(*static_cast<const Event*>(event_ptr));
			else
				f(); // Callback is not interested in the event
		};
	}

	template <typename Event, typename Channel, typename Func>
	[[nodiscard]] LIBV_FORCE_INLINE std::function<void(channel_ptr, const void*)> aux_make_callback_channel(Func&& func) {
		static constexpr bool is_void_channel = std::is_void_v<Channel>;

		if constexpr (is_void_channel) {
			return aux_make_callback_global<Event>(std::forward<Func>(func));

		} else {
			if constexpr (std::is_invocable_v<Func, Channel*, const Event&>)
				return [f = std::forward<Func>(func)](channel_ptr channel_owner, const void* event_ptr) mutable {
					f(const_cast<Channel*>(static_cast<const Channel*>(channel_owner)), *static_cast<const Event*>(event_ptr));
				};

			else if constexpr (std::is_invocable_v<Func, const Event&>)
				// Callback is not interested in the channel
				return [f = std::forward<Func>(func)](channel_ptr, const void* event_ptr) mutable {
					f(*static_cast<const Event*>(event_ptr));
				};

			else if constexpr (std::is_invocable_v<Func, Channel*>)
				// Callback is not interested in the event
				return [f = std::forward<Func>(func)](channel_ptr channel_owner, const void*) mutable {
					f(const_cast<Channel*>(static_cast<const Channel*>(channel_owner)));
				};

			else if constexpr (std::is_invocable_v<Func>)
				// Callback is not interested in the channel or event
				return [f = std::forward<Func>(func)](channel_ptr, const void*) mutable {
					f();
				};

			else
				static_assert(libv::meta::always_false_v<Func>,
						"Callback function has invalid signature, Expected either:"
						"\n\tvoid(Channel*, const Event&)"
						"\n\tvoid(const Event&)"
						"\n\tvoid(Channel*)"
						"\n\tvoid()");
		}
	}

public:
	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global(Func&& func) {
		aux_connect(nullptr, nullptr, libv::type_key<Event>(), aux_make_callback_global<Event>(std::forward<Func>(func)));
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global(slot_ptr slot_owner, Func&& func) {
		aux_connect(nullptr, slot_owner, libv::type_key<Event>(), aux_make_callback_global<Event>(std::forward<Func>(func)));
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global_front(Func&& func) {
		aux_connect_front(nullptr, nullptr, libv::type_key<Event>(), aux_make_callback_global<Event>(std::forward<Func>(func)));
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global_front(slot_ptr slot_owner, Func&& func) {
		aux_connect_front(nullptr, slot_owner, libv::type_key<Event>(), aux_make_callback_global<Event>(std::forward<Func>(func)));
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global_and_call(Func&& func, const Event& event) {
		aux_connect_and_call(nullptr, nullptr, libv::type_key<Event>(), aux_make_callback_global<Event>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global_and_call(slot_ptr slot_owner, Func&& func, const Event& event) {
		aux_connect_and_call(nullptr, slot_owner, libv::type_key<Event>(), aux_make_callback_global<Event>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global_front_and_call(Func&& func, const Event& event) {
		aux_connect_front_and_call(nullptr, nullptr, libv::type_key<Event>(), aux_make_callback_global<Event>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Func>
	LIBV_FORCE_INLINE void connect_global_front_and_call(slot_ptr slot_owner, Func&& func, const Event& event) {
		aux_connect_front_and_call(nullptr, slot_owner, libv::type_key<Event>(), aux_make_callback_global<Event>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Channel, typename Func>
	LIBV_FORCE_INLINE void connect_channel(Channel* channel_owner, Func&& func) {
		aux_connect(channel_owner, nullptr, libv::type_key<Event>(), aux_make_callback_channel<Event, Channel>(std::forward<Func>(func)));
	}

	template <typename Event, typename Channel, typename Func>
	LIBV_FORCE_INLINE void connect_channel(Channel* channel_owner, slot_ptr slot_owner, Func&& func) {
		aux_connect(channel_owner, slot_owner, libv::type_key<Event>(), aux_make_callback_channel<Event, Channel>(std::forward<Func>(func)));
	}

	template <typename Event, typename Channel, typename Func>
	LIBV_FORCE_INLINE void connect_channel_front(Channel* channel_owner, Func&& func) {
		aux_connect_front(channel_owner, nullptr, libv::type_key<Event>(), aux_make_callback_channel<Event, Channel>(std::forward<Func>(func)));
	}

	template <typename Event, typename Channel, typename Func>
	LIBV_FORCE_INLINE void connect_channel_front(Channel* channel_owner, slot_ptr slot_owner, Func&& func) {
		aux_connect_front(channel_owner, slot_owner, libv::type_key<Event>(), aux_make_callback_channel<Event, Channel>(std::forward<Func>(func)));
	}

	template <typename Event, typename Channel, typename Func>
	LIBV_FORCE_INLINE void connect_channel_and_call(Channel* channel_owner, Func&& func, const Event& event) {
		aux_connect_and_call(channel_owner, nullptr, libv::type_key<Event>(), aux_make_callback_channel<Event, Channel>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Channel, typename Func>
	LIBV_FORCE_INLINE void connect_channel_and_call(Channel* channel_owner, slot_ptr slot_owner, Func&& func, const Event& event) {
		aux_connect_and_call(channel_owner, slot_owner, libv::type_key<Event>(), aux_make_callback_channel<Event, Channel>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Channel, typename Func>
	LIBV_FORCE_INLINE void connect_channel_front_and_call(Channel* channel_owner, Func&& func, const Event& event) {
		aux_connect_front_and_call(channel_owner, nullptr, libv::type_key<Event>(), aux_make_callback_channel<Event, Channel>(std::forward<Func>(func)), &event);
	}

	template <typename Event, typename Channel, typename Func>
	LIBV_FORCE_INLINE void connect_channel_front_and_call(Channel* channel_owner, slot_ptr slot_owner, Func&& func, const Event& event) {
		aux_connect_front_and_call(channel_owner, slot_owner, libv::type_key<Event>(), aux_make_callback_channel<Event, Channel>(std::forward<Func>(func)), &event);
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
	LIBV_FORCE_INLINE void broadcast_channel(channel_ptr channel_owner, const Event& event) const {
		aux_broadcast(channel_owner, libv::type_key<Event>(), &event);
	}

	template <typename Event, typename... Args>
	LIBV_FORCE_INLINE void broadcast_channel(channel_ptr channel_owner, Args&&... args) const {
		Event event{std::forward<Args>(args)...};
		aux_broadcast(channel_owner, libv::type_key<Event>(), &event);
	}

	/// Disconnect every callback (of any type) associated with \c owner (both as a channel or a slot)
	LIBV_FORCE_INLINE void disconnect_all(channel_or_slot_ptr owner) {
		aux_disconnect_all(owner);
	}

	/// Disconnect every callback of type \c Event where \c channel_owner is the channel
	template <typename Event>
	LIBV_FORCE_INLINE void disconnect_channel(channel_ptr channel_owner) {
		aux_disconnect_channel(channel_owner, libv::type_key<Event>());
	}

	/// Disconnect every callback (of any type) where \c channel_owner is the channel
	LIBV_FORCE_INLINE void disconnect_channel_all(channel_ptr channel_owner) {
		aux_disconnect_channel_all(channel_owner);
	}

	/// Disconnect every callback of type \c Event where \c slot_owner is the slot
	template <typename Event>
	LIBV_FORCE_INLINE void disconnect_slot(slot_ptr slot_owner) {
		aux_disconnect_slot(slot_owner, libv::type_key<Event>());
	}

	/// Disconnect every callback (of any type) where \c slot_owner is the slot
	LIBV_FORCE_INLINE void disconnect_slot_all(slot_ptr slot_owner) {
		aux_disconnect_slot_all(slot_owner);
	}

public:
	[[nodiscard]] std::size_t num_channel() const noexcept;
	[[nodiscard]] std::size_t num_tracked() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
