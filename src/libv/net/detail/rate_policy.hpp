// Project: libv.net, File: src/libv/net/detail/rate_policy.hpp

#pragma once

// ext
#include <boost/beast/core/rate_policy.hpp>
// std
#include <atomic>
#include <limits>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

/// A rate policy with simple, configurable limits on reads and writes in bytes per second.
class rate_policy {
	friend class boost::beast::rate_policy_access;

	static constexpr size_t all = std::numeric_limits<size_t>::max();

private:
	size_t read_remain = all;
	size_t write_remain = all;
	size_t read_limit_ = all;
	size_t write_limit_ = all;

private:
	std::atomic_size_t read_total = 0;
	std::atomic_size_t write_total = 0;

private:
	[[nodiscard]] inline size_t available_read_bytes() const noexcept {
		return read_remain;
	}

	[[nodiscard]] inline size_t available_write_bytes() const noexcept {
		return write_remain;
	}

	inline void transfer_read_bytes(size_t n) noexcept {
		read_total += n;
		if (read_remain != all)
			read_remain = (n < read_remain) ? read_remain - n : 0;
	}

	inline void transfer_write_bytes(size_t n) noexcept {
		write_total += n;
		if (write_remain != all)
			write_remain = (n < write_remain) ? write_remain - n : 0;
	}

	inline void on_timer() noexcept {
		read_remain = read_limit_;
		write_remain = write_limit_;
	}

public:
	/// Set the limit of bytes per second to read, 0 means unlimited
	void read_limit(size_t bytes_per_second) noexcept {
		if (bytes_per_second == 0)
			bytes_per_second = all;

		read_limit_ = bytes_per_second;
		if (read_remain > bytes_per_second)
			read_remain = bytes_per_second;
	}

	/// Set the limit of bytes per second to write, 0 means unlimited
	void write_limit(size_t bytes_per_second) noexcept {
		if (bytes_per_second == 0)
			bytes_per_second = all;

		write_limit_ = bytes_per_second;
		if (write_remain > bytes_per_second)
			write_remain = bytes_per_second;
	}

public:
	[[nodiscard]] inline size_t num_byte_read() const noexcept {
		return read_total;
	}

	[[nodiscard]] inline size_t num_byte_wrote() const noexcept {
		return write_total;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
