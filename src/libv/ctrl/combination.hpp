// Project: libv.ctrl, File: src/libv/ctrl/combination.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// ext
#include <boost/container/small_vector.hpp>
// std
#include <initializer_list>
#include <iosfwd>
#include <string>
// pro
#include <libv/ctrl/input.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

struct Combination {
	boost::container::small_vector<Input, 3> inputs;

public:
	inline Combination() = default;
	inline Combination(std::initializer_list<Input> inputs) : inputs(inputs) {}

	explicit inline Combination(Input input) {
		inputs.emplace_back(std::move(input));
	}

	inline void add_input(Input input) {
		inputs.emplace_back(std::move(input));
	}

public:
	void normalize();

public:
	[[nodiscard]] friend bool operator<(const Combination& lhs, const Combination& rhs) noexcept;
	[[nodiscard]] friend bool operator==(const Combination& lhs, const Combination& rhs) noexcept;
	[[nodiscard]] friend inline bool operator!=(const Combination& lhs, const Combination& rhs) noexcept {
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
	friend inline std::ostream& operator<<(std::ostream& os, const Combination& var) {
		return var.to_stream_name(os);
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
