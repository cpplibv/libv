// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

// ext
#include <boost/sml.hpp>
// libv
#include <libv/log/log.hpp>
// std
#include <cassert>
#include <iostream>


// -------------------------------------------------------------------------------------------------

template <typename T, typename OStream>
void dump_transition(OStream& os) noexcept {
	auto src_state = std::string{boost::sml::aux::string<typename T::src_state>{}.c_str()};
	auto dst_state = std::string{boost::sml::aux::string<typename T::dst_state>{}.c_str()};
	if (dst_state == "X")
		dst_state = "[*]";

	if (T::initial)
		os << "[*] --> " << src_state << '\n';

	os << src_state << " --> " << dst_state;

	const auto has_event = !boost::sml::aux::is_same<typename T::event, boost::sml::anonymous>::value;
	const auto has_guard = !boost::sml::aux::is_same<typename T::guard, boost::sml::front::always>::value;
	const auto has_action = !boost::sml::aux::is_same<typename T::action, boost::sml::front::none>::value;

	if (has_event || has_guard || has_action)
		os << " :";

	if (has_event)
		os << " " << boost::sml::aux::get_type_name<typename T::event > ();

	if (has_guard)
		os << " [" << boost::sml::aux::get_type_name<typename T::guard::type > () << "]";

	if (has_action)
		os << " / " << boost::sml::aux::get_type_name<typename T::action::type > ();

	os << '\n';
}

template <template <typename...> typename T, typename... Ts, typename OStream>
void dump_transitions(const T<Ts...>&, OStream& os) noexcept {
	int _[]{0, (dump_transition<Ts>(os), 0)...};
	(void) _;
}

template <typename SM, typename OStream>
void dump(const SM&, OStream& os) noexcept {
	os << "@startuml\n\n";
	dump_transitions(typename SM::transitions{}, os);
	os << "\n@enduml\n";
}

// -------------------------------------------------------------------------------------------------

// Dependencies
struct Sender {
	template<class TMsg>
	constexpr void send(const TMsg& msg) {
		libv::log.info("send: {}", msg.id);
	}
};

// Events
struct EventAck { bool valid{}; };
struct EventFin { int id{}; bool valid{}; };
struct EventRelease {};
struct EventTimeout {};

// Guards
constexpr auto is_valid = [](const auto& event) { return event.valid; };

// Actions
constexpr auto send_fin = [](Sender& s) { s.send(EventFin{0}); };
constexpr auto send_ack = [](const auto& event, Sender& s) { s.send(event); };

// State Machine
struct tcp_release {
	/// Transition DSL: src_state + event [ guard ] / action = dst_state
	auto operator()() const {
		using namespace boost::sml;
		return make_transition_table(
		   *"established"_s + event<EventRelease>          / send_fin  = "fin_wait_1"_s,
			"fin_wait_1"_s  + event<EventAck> [ is_valid ]             = "fin_wait_2"_s,
			"fin_wait_2"_s  + event<EventFin> [ is_valid ] / send_ack  = "timed_wait"_s,
			"fin_wait_3"_s  + event<EventFin> [ is_valid ] / send_ack  = "fin_wait_1"_s,
			"timed_wait"_s  + event<EventTimeout>                      = X
		);
	}
};

int main() {
	std::cout << libv::logger;
	using namespace boost::sml;

	Sender s{};
	boost::sml::sm<tcp_release> sm{s}; // pass dependencies via ctor
	dump(sm, std::cout);

	assert(sm.is("established"_s));

	sm.process_event(EventRelease{}); // complexity O(1)
	assert(sm.is("fin_wait_1"_s));

	sm.process_event(EventAck{true}); // prints 'send: 0'
	assert(sm.is("fin_wait_2"_s));

	sm.process_event(EventFin{42, true}); // prints 'send: 42'
	assert(sm.is("timed_wait"_s));

	sm.process_event(EventTimeout{});
	assert(sm.is(boost::sml::X)); // terminated
}
