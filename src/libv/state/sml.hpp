// Project: libv.state, File: src/libv/state/sml.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/sml.hpp>
// std
#include <type_traits>
#include <string_view>


// -------------------------------------------------------------------------------------------------

namespace libv {
namespace sml {
namespace detail {

template <typename T, typename OStream>
void dump_transition(OStream& os) noexcept {
	auto src_state = std::string_view{boost::sml::aux::string<typename T::src_state>{}.c_str()};
	auto dst_state = std::string_view{boost::sml::aux::string<typename T::dst_state>{}.c_str()};
	if (dst_state == "X" || dst_state == "terminate")
		dst_state = "[*]";

	if (T::initial)
		os << "[*] --> " << src_state << '\n';

	os << src_state << " --> " << dst_state;

	const auto has_event = !std::is_same_v<typename T::event, boost::sml::anonymous>;
	const auto has_guard = !std::is_same_v<typename T::guard, boost::sml::front::always>;
	const auto has_action = !std::is_same_v<typename T::action, boost::sml::front::none>;

	// remove: "boost::sml::v[0-9_]*::front::internal : boost::sml::v[0-9_]*::back::on_"
	// remove: "boost::sml::v[0-9_]*::back::_, boost::sml::v[0-9_]*::back::"
	// replace: ' ' --> '_' in identifiers

	if (has_event || has_guard || has_action)
		os << " :";

	if (has_event)
		os << ' ' << boost::sml::aux::get_type_name<typename T::event>();

	if (has_guard)
		os << " [" << boost::sml::aux::get_type_name<typename T::guard::type>() << ']';

	if (has_action)
		os << " / " << boost::sml::aux::get_type_name<typename T::action::type>();

	os << '\n';
}

template <template <typename...> typename T, typename... Ts, typename OStream>
void dump_transitions(const T<Ts...>&, OStream& os) noexcept {
	(dump_transition<Ts>(os), ...);
}

} // namespace detail

// --- to_plantuml ---------------------------------------------------------------------------------

template <typename SM, typename OStream>
void to_plantuml(const SM&, OStream& os) noexcept {
	os << "@startuml\n\n";
	detail::dump_transitions(typename SM::transitions{}, os);
	os << "\n@enduml\n";
}

// --- process_as_event ----------------------------------------------------------------------------

template <typename SM, typename Event>
auto process_as_event(SM& sm) noexcept {
	return [&sm](auto&&... args) {
		sm.process_event(Event{std::forward<decltype(args)>(args)...});
	};
}

// --- state_machine -------------------------------------------------------------------------------

struct state_machine {
public:
	static constexpr auto terminate = boost::sml::X;

protected:
	using _ = boost::sml::_;

	template <typename T> static constexpr auto exception = boost::sml::exception<T>;
	template <typename T> static constexpr auto on_ = boost::sml::event<T>;
	template <typename T> static constexpr auto on = on_<T>;
	template <typename T = _> static constexpr auto on_entry = boost::sml::on_entry<T>;
	template <typename T = _> static constexpr auto on_exit = boost::sml::on_exit<T>;
	template <typename T> static constexpr auto state = boost::sml::state<T>;
	template <typename T> static constexpr auto unexpected_event = boost::sml::unexpected_event<T>;

	template <typename... Args>
	auto table(Args&&... args) const {
		return boost::sml::make_transition_table(std::forward<Args>(args)...);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace sml
} // namespace libv
