// Project: libv.net, File: src/libv/net/error.cpp

// hpp
#include <libv/net/error.hpp>
// ext
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <boost/asio/error.hpp>
// std
#include <system_error>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

std::string to_string(const std::error_code ec) {
	if (false);

	else if (ec == boost::asio::error::make_error_code(boost::asio::error::service_not_found))
		return fmt::format("{} {}", fmt::streamed(ec), "The service is not supported for the given socket type");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::socket_type_not_supported))
		return fmt::format("{} {}", fmt::streamed(ec), "The socket type is not supported");

	else if (ec == boost::asio::error::make_error_code(boost::asio::error::bad_descriptor))
		return fmt::format("{} {}", fmt::streamed(ec), "Bad file descriptor");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::connection_aborted))
		return fmt::format("{} {}", fmt::streamed(ec), "Connection aborted: An established connection was aborted by the software in your host machine");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::connection_refused))
		return fmt::format("{} {}", fmt::streamed(ec), "Connection refused: No connection could be made because the target machine actively refused it");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::connection_reset))
		return fmt::format("{} {}", fmt::streamed(ec), "Connection reset by peer: An existing connection was forcibly closed by the remote host");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::not_connected))
		return fmt::format("{} {}", fmt::streamed(ec), "Transport endpoint is not connected");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::timed_out))
		return fmt::format("{} {}", fmt::streamed(ec), "Connection timed out");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::operation_aborted))
		// Win text: The I/O operation has been aborted because of either a thread exit or an application request
		return fmt::format("{} {}", fmt::streamed(ec), "Operation aborted");
	// else if (...)
		// < Additional system mapped errors should go here on-demand

	else if (ec == boost::asio::error::make_error_code(boost::asio::error::already_open))
		return fmt::format("{} {}", fmt::streamed(ec), "Already open");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::eof))
		return fmt::format("{} {}", fmt::streamed(ec), "End of file");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::not_found))
		return fmt::format("{} {}", fmt::streamed(ec), "Element not found");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::fd_set_failure))
		return fmt::format("{} {}", fmt::streamed(ec), "The descriptor does not fit into the select call's fd_set");

	else if (ec == boost::asio::error::make_error_code(boost::asio::error::host_not_found))
		return fmt::format("{} {}", fmt::streamed(ec), "Host not found (authoritative)");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::host_not_found_try_again))
		return fmt::format("{} {}", fmt::streamed(ec), "Host not found (non-authoritative)");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::no_data))
		return fmt::format("{} {}", fmt::streamed(ec), "The query is valid but does not have associated address data");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::no_recovery))
		return fmt::format("{} {}", fmt::streamed(ec), "A non-recoverable error occurred");

	else
		return fmt::format("{} {}", fmt::streamed(ec), ec.message());
}

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
