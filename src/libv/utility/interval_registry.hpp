// Project: libv.utility, File: src/libv/utility/interval_registry.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/container/flat_set.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

class IntervalRegistry {
public:
	struct Interval {
		size_t offset;
		size_t size;

		[[nodiscard]] friend constexpr inline bool operator<(const Interval& lhs, const Interval& rhs) noexcept {
			return lhs.offset < rhs.offset;
		}

		[[nodiscard]] friend constexpr inline bool operator==(const Interval& lhs, const Interval& rhs) noexcept {
			return lhs.offset == rhs.offset && lhs.size == rhs.size;
		}
	};

private:
	boost::container::flat_set<Interval> marked;

public:
	inline IntervalRegistry() = default;
	inline ~IntervalRegistry() = default;

public:
	inline Interval next_marked() noexcept {
		assert(!marked.empty());
		return *marked.begin();
	}

public:
	inline void clear() noexcept {
		return marked.clear();
	}

	[[nodiscard]] inline bool empty() const noexcept {
		return marked.empty();
	}

	[[nodiscard]] inline bool has_marked() const noexcept {
		return !empty();
	}

	[[nodiscard]] inline size_t interval_count() const noexcept {
		return marked.size();
	}

public:
	void mark(size_t offset, size_t size) noexcept;
	void unmark(size_t offset, size_t size) noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
