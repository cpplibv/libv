// Project: libv.update, File: src/libv/update/resource_client/resource_file.lpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/interval_registry.hpp>
// std
#include <filesystem>
#include <fstream>
#include <mutex>
#include <span>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class ResourceFile {
private:
	mutable std::mutex mutex;

	bool created = false;

	std::filesystem::path path;
	std::fstream file;

	std::size_t resource_size = 0;
	std::size_t metadata_cursor = 0;
	IntervalRegistry remaining_intervals;

public:
	ResourceFile() = default;
	~ResourceFile();

private:
	void _recover_meta_blocks();

public:
	[[nodiscard]] inline bool done() const noexcept {
		const auto lock = std::scoped_lock(mutex);
		return remaining_intervals.empty();
	}

//	[[nodiscard]] inline auto task() const noexcept {
//		return remaining_intervals.next_marked();
//	}

public:
	std::span<const IntervalRegistry::Interval> create(std::filesystem::path path_, std::size_t resource_size_);
	void write(std::size_t offset, std::span<const std::byte> data);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
