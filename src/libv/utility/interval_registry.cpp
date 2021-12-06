// Project: libv.utility, File: src/libv/utility/interval_registry.cpp

// hpp
#include <libv/utility/interval_registry.hpp>
// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

void IntervalRegistry::mark(std::size_t offset, std::size_t size) noexcept {
	if (size == 0)
		return;

	// Dual key lookup is possible as container elements are strictly exclusive intervals
	const auto it_lo = std::ranges::lower_bound(marked, offset, std::less<>(), [](const auto& i) { return i.offset + i.size; });
	const auto it_hi = std::ranges::upper_bound(marked, offset + size, std::less<>(), &Interval::offset);

	if (it_lo == it_hi) {
		marked.emplace(offset, size);

	} else {
		const auto it_last = std::prev(it_hi);

		const auto new_offset = std::min(it_lo->offset, offset);
		const auto new_size = std::max(it_last->offset + it_last->size, offset + size) - new_offset;

		it_lo->offset = new_offset;
		it_lo->size = new_size;

		marked.erase(std::next(it_lo), it_hi);
	}
}

void IntervalRegistry::mark(const IntervalRegistry& intervals) noexcept {
	// TODO P5: Optimization: A more efficient merge could be implemented (based on merge-sort / join)
	for (const auto& interval : intervals.marked)
		mark(interval.offset, interval.size);
}

void IntervalRegistry::unmark(std::size_t offset, std::size_t size) noexcept {
	if (size == 0)
		return;

	// Dual key lookup is possible as container elements are strictly exclusive intervals
	const auto it_lo = std::ranges::lower_bound(marked, offset, std::less<>(), [](const auto& i) { return i.offset + i.size; });
	const auto it_hi = std::ranges::upper_bound(marked, offset + size, std::less<>(), &Interval::offset);

	auto lo = std::distance(marked.begin(), it_lo);
	auto hi = std::distance(marked.begin(), it_hi);

	for (auto i = lo; i < hi; ++i) {
		const auto it = marked.nth(i);

		const auto edge_on_left = it->offset <= offset && offset <= it->offset + it->size;
		const auto edge_on_rght = it->offset <= offset + size && offset + size <= it->offset + it->size;

		const auto new_left_side = Interval{it->offset, offset - it->offset};
		const auto new_rght_side = Interval{offset + size, it->offset + it->size - offset - size};

		const auto do_cut_left = edge_on_left && new_left_side.size != 0;
		const auto do_cut_rght = edge_on_rght && new_rght_side.size != 0;

		if (do_cut_left && do_cut_rght) {
			*it = new_left_side;
			marked.emplace_hint(it, new_rght_side);

		} else if (do_cut_left) {
			*it = new_left_side;

		} else if (do_cut_rght) {
			*it = new_rght_side;

		} else {
			marked.erase(it);
			--hi;
			--i;
		}
	}
}

void IntervalRegistry::unmark(const IntervalRegistry& intervals) noexcept {
	// TODO P5: Optimization: A more efficient merge could be implemented (based on merge-sort / join)
	for (const auto& interval : intervals.marked)
		unmark(interval.offset, interval.size);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
