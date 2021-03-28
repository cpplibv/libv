// Project: libv.ctrl, File: src/libv/ctrl/sequence.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <initializer_list>
#include <iosfwd>
#include <string>
// pro
#include <libv/ctrl/combination.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

enum class validation_result : uint32_t {
	/// Valid sequence
	success = 0,

	//	__ = 1 << 0,
	//	__ = 1 << 1,

	///
	redundant_input_in_combination = 1 << 2,

	///
	multiple_action_in_combination = 1 << 3,

	///
	analog_with_dia = 1 << 4,

	///
	conflicting_analog_dimension = 1 << 5,

	///
	empty = 1 << 6,

	///
	contains_invalid_input = 1 << 7,

	///
	contains_empty_combination = 1 << 8,
};

constexpr inline validation_result& operator|=(validation_result& lhs, validation_result rhs) noexcept {
	lhs = validation_result{libv::to_value(lhs) | libv::to_value(rhs)};
	return lhs;
}

constexpr inline validation_result operator|(validation_result lhs, validation_result rhs) noexcept {
	return validation_result{libv::to_value(lhs) | libv::to_value(rhs)};
}

// -------------------------------------------------------------------------------------------------

struct Sequence {
public:
	boost::container::small_vector<Combination, 2> combinations;

public:
	inline Sequence() = default;

	explicit inline Sequence(Input input) {
		combinations.emplace_back(std::move(input));
	}

	explicit inline Sequence(Combination combination) {
		combinations.emplace_back(std::move(combination));
	}

	inline Sequence(std::initializer_list<Input> inputs) : Sequence(Combination{inputs}) {}
	inline Sequence(std::initializer_list<Combination> combinations) : combinations(combinations) {}

public:
	inline void add_combination(Combination combination) {
		combinations.emplace_back(std::move(combination));
	}

public:
	void normalize();
	validation_result validate() const noexcept;

public:
	[[nodiscard]] friend bool operator<(const Sequence& lhs, const Sequence& rhs) noexcept;
	[[nodiscard]] friend bool operator==(const Sequence& lhs, const Sequence& rhs) noexcept;
	[[nodiscard]] friend inline bool operator!=(const Sequence& lhs, const Sequence& rhs) noexcept {
		return !(lhs == rhs);
	}

public:
	std::ostream& to_stream_symbol(std::ostream& os) const;
	std::ostream& to_stream_name(std::ostream& os) const;

	[[nodiscard]] std::string to_string_symbol() const;
	[[nodiscard]] std::string to_string_name() const;

	/// Shortcut to member function to_stream_name
	[[nodiscard]] std::string to_string() const;

	/// Shortcut to member function to_stream_name
	friend inline std::ostream& operator<<(std::ostream& os, const Sequence& var) {
		return var.to_stream_name(os);
	}
};

// -------------------------------------------------------------------------------------------------

std::string to_string(const Sequence& var);

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
