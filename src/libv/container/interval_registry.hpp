// Project: libv.container, File: src/libv/container/interval_registry.hpp

#pragma once

// std
#include <span>
// ext
#include <boost/container/flat_set.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

class IntervalRegistry {
public:
	struct Interval {
		std::size_t offset;
		std::size_t size;

		[[nodiscard]] friend constexpr inline bool operator<(const Interval& lhs, const Interval& rhs) noexcept {
			return lhs.offset < rhs.offset;
		}

		[[nodiscard]] inline bool operator==(const Interval& other) const noexcept = default;
	};

private:
	boost::container::flat_set<Interval> marked;

public:
	inline IntervalRegistry() = default;
	inline ~IntervalRegistry() = default;

public:
	[[nodiscard]] inline Interval next_marked() const noexcept {
		assert(!marked.empty());
		return *marked.begin();
	}

	[[nodiscard]] inline std::span<const Interval> marked_intervals() const noexcept {
		return std::span<const Interval>(&*marked.cbegin(), marked.size());
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

	[[nodiscard]] inline std::size_t interval_count() const noexcept {
		return marked.size();
	}

public:
	void mark(std::size_t offset, std::size_t size) noexcept;
	void mark(const IntervalRegistry& intervals) noexcept;
	void unmark(std::size_t offset, std::size_t size) noexcept;
	void unmark(const IntervalRegistry& intervals) noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
