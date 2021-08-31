// Project: libv.utility, File: src/libv/utility/nexus.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/utility/nexus_fwd.hpp>
// libv
#include <libv/utility/type_key.hpp>
// std
#include <functional>
#include <memory>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Performance analysis:
// https://quick-bench.com/q/-8xyfuQaWaG5-mYHGuhXoSL0I3w
// https://godbolt.org/z/Kfexxh4f4

class ImplNexus;

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

// -------------------------------------------------------------------------------------------------

} // namespace libv
