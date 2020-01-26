// File:   function_ref.hpp Author: Vader Created on 30 August 2019, 00:10

#pragma once

// std
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P4: unique_function is a forward compatible placeholder, acquire a decent implementation

template <typename Signature>
struct unique_function : std::function<Signature> {
	unique_function() = default;
	unique_function(const unique_function&) = delete;
	unique_function(unique_function&&) = default;
	unique_function& operator=(const unique_function&) & = delete;
	unique_function& operator=(unique_function&&) & = default;

	using std::function<Signature>::function;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
