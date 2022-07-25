// Created by Vader on 2022.07.26..

#pragma once


namespace fmt {
inline namespace v8 {

// NOTE: inline namespace version has to be manually updated if it changes in fmt
// (test_fmt_fwd.cpp does have an automated test for this)

// -------------------------------------------------------------------------------------------------

/// Default value for Char:   char
/// Default value for Enable: void
template <typename T, typename Char, typename Enable>
struct formatter;

// -------------------------------------------------------------------------------------------------

} // inline namespace v8
} // namespace fmt
