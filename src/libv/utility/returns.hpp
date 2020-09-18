// File: returns.hpp Author: Vader Created on 2017. szeptember 8., 22:04

#pragma once


/// Utility macro for wrapping function body into a sfinae-decltype-noexcept-return context
/// @Usage auto foo(int i) LIBV_RETURNS(i * 2)
#define LIBV_RETURNS(...) \
	noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }
