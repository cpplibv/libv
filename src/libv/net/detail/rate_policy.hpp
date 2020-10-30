// Project: libv.net, File: src/libv/net/detail/rate_policy.hpp, Author: Császár Mátyás [Vader]

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

	static size_t constexpr all = std::numeric_limits<size_t>::max();

private:
	size_t rd_remain = all;
	size_t wr_remain = all;
	size_t rd_limit = all;
	size_t wr_limit = all;

private:
	std::atomic_size_t rd_total = 0;
	std::atomic_size_t wr_total = 0;

private:
	[[nodiscard]] size_t available_read_bytes() const noexcept {
		return rd_remain;
	}

	[[nodiscard]] size_t available_write_bytes() const noexcept {
		return wr_remain;
	}

	void transfer_read_bytes(size_t n) noexcept {
		rd_total += n;
		if (rd_remain != all)
			rd_remain = (n < rd_remain) ? rd_remain - n : 0;
	}

	void transfer_write_bytes(size_t n) noexcept {
		wr_total += n;
		if (wr_remain != all)
			wr_remain = (n < wr_remain) ? wr_remain - n : 0;
	}

	void on_timer() noexcept {
		rd_remain = rd_limit;
		wr_remain = wr_limit;
	}

public:
	/// Set the limit of bytes per second to read, 0 means unlimited
	void read_limit(size_t bytes_per_second) noexcept {
		if (bytes_per_second == 0)
			bytes_per_second = all;

		rd_limit = bytes_per_second;
		if (rd_remain > bytes_per_second)
			rd_remain = bytes_per_second;
	}

	/// Set the limit of bytes per second to write, 0 means unlimited
	void write_limit(size_t bytes_per_second) noexcept {
		if (bytes_per_second == 0)
			bytes_per_second = all;

		wr_limit = bytes_per_second;
		if (wr_remain > bytes_per_second)
			wr_remain = bytes_per_second;
	}

public:
	[[nodiscard]] inline size_t total_read_bytes() const noexcept {
		return rd_total;
	}

	[[nodiscard]] inline size_t total_write_bytes() const noexcept {
		return wr_total;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
