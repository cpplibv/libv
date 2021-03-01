// Project: libv.net, File: src/libv/net/detail/current_io_context.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace net {
namespace detail {

// -------------------------------------------------------------------------------------------------

/// Global variable for conveniently inject the io_context to base classes of Connection and Acceptor
thread_local inline class IOContext* current_io_context = nullptr;

// -------------------------------------------------------------------------------------------------

} // namespace detail
} // namespace net
} // namespace libv
