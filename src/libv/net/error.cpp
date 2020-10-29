// Project: libv.net, File: src/libv/net/error.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/net/error.hpp>
// ext
#include <fmt/format.h>
#include <boost/asio/error.hpp>
// std
#include <system_error>


namespace libv {
namespace net {

std::string to_string(const std::error_code ec) {
	if (false);

	else if (ec == boost::asio::error::make_error_code(boost::asio::error::service_not_found))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "The service is not supported for the given socket type");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::socket_type_not_supported))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "The socket type is not supported");

	else if (ec == boost::asio::error::make_error_code(boost::asio::error::bad_descriptor))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Bad file descriptor");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::connection_refused))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Connection refused");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::connection_reset))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Connection reset by peer");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::not_connected))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Transport endpoint is not connected");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::timed_out))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Connection timed out");
		// ...

	else if (ec == boost::asio::error::make_error_code(boost::asio::error::already_open))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Already open");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::eof))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "End of file");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::not_found))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Element not found");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::fd_set_failure))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "The descriptor does not fit into the select call's fd_set");

	else if (ec == boost::asio::error::make_error_code(boost::asio::error::host_not_found))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Host not found (authoritative)");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::host_not_found_try_again))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "Host not found (non-authoritative)");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::no_data))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "The query is valid but does not have associated address data");
	else if (ec == boost::asio::error::make_error_code(boost::asio::error::no_recovery))
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), "A non-recoverable error occurred");

	else
		return fmt::format("{}:{} - {}", ec.category().name(), ec.value(), ec.message());
}

} // namespace net
} // namespace libv
