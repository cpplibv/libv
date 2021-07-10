// Project: libv.ui, File: src/libv/ui/context/context_event.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <typeindex>


namespace libv {

// -------------------------------------------------------------------------------------------------

class ImplEventHub;

class EventHub {
	using channel_key_type = std::string;
	using channel_key_view_type = std::string_view;

private:
	std::shared_ptr<ImplEventHub> self;

public:
	EventHub();
	inline EventHub(const EventHub&) noexcept = default;
	inline EventHub& operator=(const EventHub&) & noexcept = default;
	inline EventHub(EventHub&&) noexcept = default;
	inline EventHub& operator=(EventHub&&) & noexcept = default;
	~EventHub();

private:
	void aux_connect(channel_key_type channel, std::type_index event_type, std::function<void(const void*)> func);
	void aux_broadcast(channel_key_view_type channel, std::type_index event_type, const void* event_ptr);
	void aux_disconnect(channel_key_view_type channel, std::type_index event_type);

public:
	template <typename Event, typename Func>
	inline void connect(channel_key_type channel, Func&& func) {
		aux_connect(std::move(channel), std::type_index{typeid(Event)}, [f = std::forward<Func>(func)](void* event_ptr) {
			f(*static_cast<Event*>(event_ptr));
		});
	}

	template <typename Event, typename Func>
	inline void connect(Func&& func) {
		aux_connect("", std::type_index{typeid(Event)}, [f = std::forward<Func>(func)](const void* event_ptr) {
			f(*static_cast<Event*>(event_ptr));
		});
	}

	template <typename Event>
	inline void broadcast(channel_key_view_type channel, const Event& event) {
		aux_broadcast(channel, std::type_index(typeid(Event)), &event);
	}

	template <typename Event>
	inline void broadcast(const Event& event) {
		aux_broadcast("", std::type_index(typeid(Event)), &event);
	}

	template <typename Event, typename... Args>
	inline void broadcast(channel_key_view_type channel, Args&& args) {
		Event event{std::forward<Args>(args)...};
		aux_broadcast(channel, std::type_index(typeid(Event)), &event);
	}

	template <typename Event, typename... Args>
	inline void broadcast(Args&& args) {
		Event event{std::forward<Args>(args)...};
		aux_broadcast("", std::type_index(typeid(Event)), &event);
	}

	template <typename Event>
	void disconnect(channel_key_view_type channel) {
		aux_disconnect(channel, std::type_index(typeid(Event)));
	}

	template <typename Event>
	void disconnect() {
		aux_disconnect("", std::type_index(typeid(Event)));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
