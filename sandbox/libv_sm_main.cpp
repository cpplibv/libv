// Project: libv, File: sandbox/libv_sm_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/log/log.hpp>
//#include <libv/utility/state.hpp>
#include <libv/state/idea_state.hpp>
// std
#include <iostream>


//// -------------------------------------------------------------------------------------------------
//
////	auto operator()() const {
////		return table(
////				wait_info(
////					on_entry<_>             / send_report_version,
////					on<VersionNotSupported> / version_not_supported = terminate,
////					on<VersionUpToDate>     / version_up_to_date    = terminate,
////					on<UpdateInfo>          / receive_version_info  = wait_chunk,
////					on<net_io_error>                                = terminate
////				)
////				wait_chunk(
////					on_entry<_>             / request_next_chunk,
////					on<PatchNotSupported>   / patch_not_supported   = terminate,
////					on<PatchChunk>          / save_and_request_next_chunk,
////					on<net_io_error>                                = terminate
////				)
////		);
////	}
//
//struct Client_to_MasterProxy {
//
//	struct EventCommunicationError : libv::sm::event<EventCommunicationError> {
//		int ec;
//	};
//	struct EventReceiveAnswer : libv::sm::event<EventReceiveAnswer> {};
//	struct EventUserSubscribe : libv::sm::event<EventUserSubscribe> {};
//	struct EventReceiveSOW : libv::sm::event<EventReceiveSOW> {};
//	struct EventReceiveUpdate : libv::sm::event<EventReceiveUpdate> {};
//	struct EventUserUnsubscribe : libv::sm::event<EventUserUnsubscribe> {};
//
//	struct MainState;
//	struct AwaitingAnswer;
//	struct StandBy;
//	struct UpdateContinuous;
//
//	struct SM : libv::sm::machine<SM, MainState> {
////		double connection;
////		libv::observer_ptr<Client> client;
////		double state;
//	};
//
//	struct MainState : libv::sm::state<MainState, SM, AwaitingAnswer> {
//		using reactions = react_list<EventCommunicationError>;
//
//		result react(const EventCommunicationError&);
//	};
//
//	struct AwaitingAnswer : libv::sm::state<AwaitingAnswer, MainState> {
//		using reactions = react_list<EventReceiveAnswer>;
//
//		result react(const EventReceiveAnswer&);
//		AwaitingAnswer();
//	};
//
//	struct StandBy : libv::sm::state<StandBy, MainState> {
//		using reactions = react_list<EventUserSubscribe>;
//
//		result react(const EventUserSubscribe&);
//	};
//
//	struct UpdateContinuous : libv::sm::state<UpdateContinuous, MainState> {
//		using reactions = react_list<EventReceiveSOW, EventReceiveUpdate, EventUserUnsubscribe>;
//
////		result enter();
////		result leave();
//		result react(const EventReceiveSOW&);
//		result react(const EventReceiveUpdate&);
//		result react(const EventUserUnsubscribe&);
//		UpdateContinuous();
//		~UpdateContinuous();
//	};
//};
//
//int main() {
//	std::cout << libv::logger_stream;
//
//	std::cout << "State";
//
//	Client_to_MasterProxy::SM sm;
//
//	sm.initiate();
//	sm.process_event(Client_to_MasterProxy::EventReceiveAnswer{});
//	sm.process_event(Client_to_MasterProxy::EventReceiveUpdate{});
//	sm.terminated();
//
////	Sender s{};
////	my_logger ml{};
//////	boost::sml::sm<tcp_release, boost::sml::logger<my_logger>> sm{s, ml}; // pass dependencies via constructor
////	boost::sml::sm<tcp_release2, boost::sml::logger<my_logger>> sm{s, ml}; // pass dependencies via constructor
////	libv::sml::to_plantuml(sm, std::cout);
////
////	assert(sm.is(tcp_release::established));
////
////	sm.process_event(EventRelease{}); // complexity O(1)
////	assert(sm.is(tcp_release::fin_wait_1));
////
////	sm.process_event(EventAck{true}); // prints 'send: 0'
////	assert(sm.is(tcp_release::fin_wait_2));
////
////	sm.process_event(EventFin{42, true}); // prints 'send: 42'
////	assert(sm.is(tcp_release::timed_wait));
////
////	sm.process_event(EventTimeout{});
////	assert(sm.is(tcp_release::terminate)); // terminated
//}
//
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================

inline libv::LoggerModule log_snb{libv::logger_stream, "snb"};


struct EventCommunicationError {};
struct EventReceiveAnswer {};
struct EventReceiveSOW {};
struct EventReceiveUpdate {};
struct EventUserUnsubscribe {};
struct EventUserSubscribe {};


struct MainState : state {
	struct AwaitingAnswer;
	struct StandBy;
	struct UpdateContinuous;

	using initial = StandBy;
	using states = list<AwaitingAnswer, StandBy, UpdateContinuous>;
	using reacts = list<EventCommunicationError>;

	MainState() { log_snb.trace("MainState Ctor"); }
	void enter() { log_snb.trace("MainState enter"); }
	result react(const EventCommunicationError&) { log_snb.trace("MainState EventCommunicationError"); return terminate(); }
	void leave() { log_snb.trace("MainState leave"); }
	~MainState() { log_snb.trace("MainState Dtor"); }

	struct AwaitingAnswer : state  {
		using reacts = list<EventReceiveAnswer>;

		AwaitingAnswer() { log_snb.trace("AwaitingAnswer Ctor"); }
		void enter() { log_snb.trace("AwaitingAnswer enter"); }
		result react(const EventReceiveAnswer&) { log_snb.trace("AwaitingAnswer EventReceiveAnswer"); return discard_event(); }
		void leave() { log_snb.trace("AwaitingAnswer leave"); }
		~AwaitingAnswer() { log_snb.trace("AwaitingAnswer Dtor"); }
	};

	struct StandBy : state {
		struct Idle;

		using initial = Idle;
		using reacts = list<EventUserSubscribe>;
		using states = list<Idle>;

		StandBy() { log_snb.trace("StandBy Ctor"); }
		void enter() { log_snb.trace("StandBy enter"); }
		result react(const EventUserSubscribe&);
		void leave() { log_snb.trace("StandBy leave"); }
		~StandBy() { log_snb.trace("StandBy Dtor"); }

		struct Idle : state {
			Idle() { log_snb.trace("Idle Ctor"); }
			void enter() { log_snb.trace("Idle enter"); }
			void leave() { log_snb.trace("Idle leave"); }
			~Idle() { log_snb.trace("Idle Dtor"); }
		};
	};

	struct UpdateContinuous : state {
		using reacts = list<EventReceiveSOW, EventReceiveUpdate, EventUserUnsubscribe>;

		UpdateContinuous() { log_snb.trace("UpdateContinuous Ctor"); }
		void enter() { log_snb.trace("UpdateContinuous enter"); }
		result react(const EventReceiveSOW&) { log_snb.trace("UpdateContinuous EventReceiveSOW"); return discard_event(); }
		result react(const EventReceiveUpdate&) { log_snb.trace("UpdateContinuous EventReceiveUpdate"); return discard_event(); }
		result react(const EventUserUnsubscribe&) { log_snb.trace("UpdateContinuous EventUserUnsubscribe"); return discard_event(); }
		void leave() { log_snb.trace("UpdateContinuous leave"); }
		~UpdateContinuous() { log_snb.trace("UpdateContinuous Dtor"); }
	};
};

result MainState::StandBy::react(const EventUserSubscribe&) {
	log_snb.trace("StandBy EventUserSubscribe");

	//context().can_access_main_state_member_variable;
	//root_context().can_access_root_state_member_variable;
	//	return terminate();
	return discard_event();
	//return forward_event();
	//return defer_event();
	//return transit<UpdateContinuous>();
}


int main() {
	std::cout << libv::logger_stream;

	log_snb.trace(" --- main Start");
	{
		machine<MainState> state_machine;

		state_machine.process_event(EventReceiveAnswer{});
		assert(state_machine.is<MainState::UpdateContinuous>());
		state_machine.process_event(EventUserSubscribe{});
		assert(state_machine.is<MainState::UpdateContinuous>());
		state_machine.process_event(EventUserUnsubscribe{});
		assert(state_machine.terminated());

		log_snb.trace(" --- main Stopping");
	}
	log_snb.trace(" --- main Stopped");
}
