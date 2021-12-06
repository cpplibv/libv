// Project: libv.utility, File: src/libv/utility/read_file_stop.hpp

// std
#include <filesystem>
#include <stop_token>
#include <system_error>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct [[nodiscard]] ReadFileResult {
	std::vector<std::byte> data;
	std::error_code ec;
};

// By default reading in 1 MB chunks between checks for cancellation
static constexpr std::size_t default_read_file_stop_chunk_size = 1 * 1024 * 1024;

// -------------------------------------------------------------------------------------------------

struct fn_read_file_stop {
	[[nodiscard]] ReadFileResult operator()(std::stop_token st, const std::filesystem::path& filepath) const noexcept;
	[[nodiscard]] ReadFileResult operator()(std::stop_token st, const std::filesystem::path& filepath, std::size_t chunk_size) const noexcept;
};

static constexpr fn_read_file_stop read_file_stop{};

// -------------------------------------------------------------------------------------------------

} // namespace libv
