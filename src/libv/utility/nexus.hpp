// Project: libv.utility, File: src/libv/utility/nexus.hpp

#pragma once

// fwd
#include <libv/utility/nexus_fwd.hpp>
// libv
#include <libv/meta/force_inline.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
#include <libv/utility/type_key.hpp>
#include <libv/utility/unique_function.hpp>
// std
#include <string>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Performance analysis:
// https://godbolt.org/z/Kfexxh4f4
// https://quick-bench.com/q/-8xyfuQaWaG5-mYHGuhXoSL0I3w
// https://quick-bench.com/q/xrORehjQdzzC2ZMGDcKcog_KQUM

// -------------------------------------------------------------------------------------------------

struct ImplNexus;
void increase_ref_count(ImplNexus* ptr);
void decrease_ref_count(ImplNexus* ptr);

/// Handler type
class Nexus {
public:
	using channel_ptr = const void*;
	using slot_ptr = const void*;
	using channel_or_slot_ptr = const void*;
	using type_uid = libv::type_uid;

	using object_ptr = void*;

private:
	libv::intrusive2_ptr<ImplNexus> self;

public:
	Nexus();
	Nexus(const Nexus&) noexcept = default;
	Nexus& operator=(const Nexus&) & noexcept = default;
	Nexus(Nexus&&) noexcept = default;
	Nexus& operator=(Nexus&&) & noexcept = default;
	~Nexus() = default;

private:
	void aux_connect(channel_ptr channel_owner, slot_ptr slot_owner, type_uid event_type, libv::unique_function<void(channel_ptr, const void*)>&& func);
	void aux_connect_and_call(channel_ptr channel_owner, slot_ptr slot_owner, type_uid event_type, libv::unique_function<void(channel_ptr, const void*)>&& func, const void* event_ptr);
	void aux_connect_front(channel_ptr channel_owner, slot_ptr slot_owner, type_uid event_type, libv::unique_function<void(channel_ptr, const void*)>&& func);
	void aux_connect_front_and_call(channel_ptr channel_owner, slot_ptr slot_owner, type_uid event_type, libv::unique_function<void(channel_ptr, const void*)>&& func, const void* event_ptr);
	void aux_broadcast(channel_ptr channel_owner, type_uid event_type, const void* event_ptr) const;
	void aux_disconnect_all(channel_or_slot_ptr owner);
	void aux_disconnect_channel(channel_ptr channel_owner, type_uid event_type);
	void aux_disconnect_channel_all(channel_ptr channel_owner);
	void aux_disconnect_slot(slot_ptr slot_owner, type_uid event_type);
	void aux_disconnect_slot_all(slot_ptr slot_owner);

private:
	// NOTE: It is possible to extend or implement Nexus with an owning object version not just views: Simple 8 byte deleter function with the object pointer (like shared_ptr)
	void aux_object_view_set(type_uid object_type, object_ptr ptr);
	void aux_object_view_set(std::string name, type_uid object_type, object_ptr ptr);
	[[nodiscard]] object_ptr aux_object_view_get(type_uid object_type) const noexcept;
	[[nodiscard]] object_ptr aux_object_view_get(std::string_view name, type_uid object_type) const noexcept;
	void aux_object_view_remove(type_uid object_type) const noexcept;
	void aux_object_view_remove(std::string_view name, type_uid object_type) const noexcept;
//	void aux_object_view_push(type_uid object_type, object_ptr ptr);
//	void aux_object_view_pop(type_uid object_type) noexcept;
//	void aux_object_view_push(std::string name, type_uid object_type, object_ptr ptr);
//	void aux_object_view_pop(std::string name, type_uid object_type) noexcept;

private:
	template <typename Event, typename Func>
	[[nodiscard]] LIBV_FORCE_INLINE libv::unique_function<void(channel_ptr, const void*)> aux_make_callback_global(Func&& func) {
		return [f = std::forward<Func>(func)](channel_ptr, const void* event_ptr) mutable {
			if constexpr(std::is_invocable_v<Func, const Event&>)
				f(*static_cast<const Event*>(event_ptr));
			else
				f(); // Callback is not interested in the event
		};
	}

	template <typename Event, typename Channel, typename Func>
	[[nodiscard]] LIBV_FORCE_INLINE libv::unique_function<void(channel_ptr, const void*)> aux_make_callback_channel(Func&& func) {
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
				static_assert(false,
						"Callback function has invalid signature, Expected either:"
						"\n\tvoid(Channel*, const Event&)"
						"\n\tvoid(const Event&)"
						"\n\tvoid(Channel*)"
						"\n\tvoid()");
		}
	}

public:
	template <typename Object>
	LIBV_FORCE_INLINE void object_view_set(Object* ptr) {
		aux_object_view_set(libv::type_key<Object>(), ptr);
	}

	template <typename Object>
	LIBV_FORCE_INLINE void object_view_set(const std::string& name, Object* ptr) {
		aux_object_view_set(name, libv::type_key<Object>(), ptr);
	}

	template <typename Object>
	[[nodiscard]] LIBV_FORCE_INLINE Object* object_view_get() const noexcept {
		return static_cast<Object*>(aux_object_view_get(libv::type_key<Object>()));
	}

	template <typename Object>
	[[nodiscard]] LIBV_FORCE_INLINE Object* object_view_get(const std::string_view name) const noexcept {
		return static_cast<Object*>(aux_object_view_get(name, libv::type_key<Object>()));
	}

	template <typename Object>
	LIBV_FORCE_INLINE void object_view_remove() noexcept {
		aux_object_view_remove(libv::type_key<Object>());
	}

	template <typename Object>
	LIBV_FORCE_INLINE void object_view_remove(const std::string_view name) noexcept {
		aux_object_view_remove(name, libv::type_key<Object>());
	}

//	template <typename Object>
//	LIBV_FORCE_INLINE void push_object_view(Object* ptr) {
//		aux_push(libv::type_key<Object>(), ptr);
//	}
//
//	template <typename Object>
//	LIBV_FORCE_INLINE void pop_object_view(Object* ptr) noexcept {
//		aux_pop(libv::type_key<Object>());
//	}

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
	/// Number of channels
	[[nodiscard]] std::size_t num_channel() const noexcept;
	/// Number of tracked objects as either signal or slot
	[[nodiscard]] std::size_t num_tracked() const noexcept;
	/// Number of objects as beans
	[[nodiscard]] std::size_t num_object() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
