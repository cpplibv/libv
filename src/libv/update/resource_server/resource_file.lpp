// Project: libv.update, File: src/libv/update/resource_server/resource_file.lpp, Author: Császár Mátyás [Vader]

#pragma once

// hpp
#include <libv/update/resource_server/resource_server.hpp>
// libv
#include <libv/utility/read_file.hpp>
// std
#include <filesystem>
#include <span>
#include <vector>
// pro
//#include <libv/update/log.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct ResourceFile {
	std::filesystem::path filepath;
	resource_cache_policy cache_policy;

//	libv::mt::async_value<libv::ReadFileResult> data;
	std::vector<std::byte> data_;

public:
	ResourceFile(std::filesystem::path filepath_, resource_cache_policy cachePolicy) :
		filepath(std::move(filepath_)),
		cache_policy(cachePolicy) {

		// TODO P2: libv.update: Lazy load file for resource server based on cache_policy
		data_ = libv::read_file_bin_or_throw(filepath);
	}

	[[nodiscard]] constexpr inline std::span<const std::byte> data() const noexcept {
		return data_;
	}

	[[nodiscard]] inline uint64_t size() const noexcept {
		return static_cast<uint64_t>(data_.size());
	}

	[[nodiscard]] inline std::vector<std::byte> copy_chunk(uint64_t offset, uint64_t amount) const {
		assert(offset + amount <= data_.size());
		std::vector<std::byte> result;

		result.resize(amount);
		std::memcpy(result.data(), data_.data() + offset, amount);

		return result;
	}

	[[nodiscard]] inline std::span<const std::byte> view_chunk(uint64_t offset, uint64_t amount) const {
		assert(offset + amount <= data_.size());
		return std::span<const std::byte>(data_.data() + offset, amount);
	}

//	bool fetch(uint64_t offset, uint64_t amount) {
//		return is_ready;
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
