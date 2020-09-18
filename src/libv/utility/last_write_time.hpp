// File: approxing.hpp Author: Vader Created on 2017. augusztus 16., 23:59

#pragma once

// std
#include <chrono>
#include <filesystem>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] inline std::chrono::system_clock::time_point last_write_time(const std::filesystem::path& path) {
	// C++20: There might be a better way in the future for this like:
	//  std::chrono::clock_cast<std::chrono::system_clock>(lwt)

	const auto lwt = std::filesystem::last_write_time(path);
	const auto lwt_sys = decltype(lwt)::clock::to_sys(lwt);

	return lwt_sys;
}

[[nodiscard]] inline std::chrono::system_clock::time_point last_write_time(const std::filesystem::path& path, std::error_code& ec) noexcept {
	// C++20: There might be a better way in the future for this like:
	//  std::chrono::clock_cast<std::chrono::system_clock>(lwt)

	const auto lwt = std::filesystem::last_write_time(path, ec);
	const auto lwt_sys = decltype(lwt)::clock::to_sys(lwt);

	return lwt_sys;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
