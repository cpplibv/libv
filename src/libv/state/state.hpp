// Project: libv.state, File: src/libv/state/state.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>


namespace libv {
namespace sm {

// -------------------------------------------------------------------------------------------------

template <typename T>
using event = boost::statechart::event<T>;

template <typename CRTP, typename InitialState>
using machine = boost::statechart::state_machine<CRTP, InitialState>;

using result = boost::statechart::result;

template <typename CRTP, typename Context, typename EntryState = boost::mpl::list<>>
struct state : boost::statechart::simple_state<CRTP, Context, EntryState> {
	template <typename... T>
	using react_list = boost::mpl::list<boost::statechart::custom_reaction<T>...>;

	using result = libv::sm::result;
};

// -------------------------------------------------------------------------------------------------

} // namespace sm
} // namespace libv
