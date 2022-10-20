// Created by Vader on 2022.09.19..

#pragma once

// ext
#include <fmt/format.h>
// libv
//#include <libv/meta/force_inline.hpp>
// std
#include <cstdint>
#include <string>
#include <ostream>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct version_number {
	uint16_t major = 0; /// Breaking changes
	uint16_t minor = 0; /// Non-breaking additions
	uint16_t patch = 0; /// Bug fix only no changes
	uint16_t build = 0; ///

public:
	constexpr inline version_number() noexcept = default;
	constexpr inline version_number(uint16_t major, uint16_t minor) noexcept :
			major(major), minor(minor) {}
	constexpr inline version_number(uint16_t major, uint16_t minor, uint16_t patch) noexcept :
			major(major), minor(minor), patch(patch) {}
	constexpr inline version_number(uint16_t major, uint16_t minor, uint16_t patch, uint16_t build) noexcept :
			major(major), minor(minor), patch(patch), build(build) {}

public:
	template <typename Archive> void serialize(Archive& ar) {
		ar.nvp("major", major);
		ar.nvp("minor", minor);
		ar.nvp("patch", patch);
		ar.nvp("build", build);
	}

public:
	[[nodiscard]] constexpr inline auto operator<=>(const version_number&) const = default;

	friend std::ostream& operator<<(std::ostream& os, const version_number& var) {
		return os << var.major << '.' << var.minor << '.' << var.patch << '.' << var.build;
	}
};

} // namespace libv --------------------------------------------------------------------------------

template <>
struct fmt::formatter<libv::version_number> : fmt::formatter<uint16_t> {

	template <typename FormatContext>
	auto format(const libv::version_number& var, FormatContext& ctx) const {
		auto out = ctx.out();

		out = formatter<uint16_t>::format(var.major, ctx);
		*out++ = '.';
		ctx.advance_to(out);
		out = formatter<uint16_t>::format(var.minor, ctx);
		*out++ = '.';
		ctx.advance_to(out);
		out = formatter<uint16_t>::format(var.patch, ctx);
		*out++ = '.';
		ctx.advance_to(out);
		out = formatter<uint16_t>::format(var.build, ctx);

		ctx.advance_to(out);
		return out;
	}
};

namespace libv { // --------------------------------------------------------------------------------

struct build_info {
	libv::version_number version_number;
//	std::string version_name;
//	... build_number;
//	... build_hash;
//	... build_date / build_time;
//	... git_commit_number;
//	... git_branch;
//	... variant;
//  std::string compiler;
//  std::string build_type;
//  std::chrono::system_clock::time_point build_time;
//	std::optional<libv::hash::SHA1> git_hash;
//
//	? uint8_t stage;
//		Alpha
//		Beta
//		Release candidate
//		Release
//
//		Stable
//
//	WISH_GIT_BRANCH
//	WISH_GIT_COMMIT_HASH
//	WISH_DATE_SHORT WISH_TIME_SHORT
};

//extern BuildInfo build;
//extern libv::build_info build;

// -------------------------------------------------------------------------------------------------

} // namespace libv
