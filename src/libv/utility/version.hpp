// Project: libv.utility, File: src/libv/utility/version.hpp

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

////template <std::size_t N, typename T>
////struct fmt::formatter<libv::vec_t<N, T>, char, void> : public fmt::formatter<T, char, void> {
////	const char* sep_begin = " ";
////	const char* sep_end = sep_begin + 1;
////
////	template <typename ParseContext>
////	constexpr auto parse(ParseContext& ctx) {
////		const auto specified = ctx.begin() != ctx.end();
////		const auto begin = ctx.begin();
////		auto end = begin;
////		for (; end != ctx.end() && *end != '}'; ++end);
////
////		if (!specified)
////			return formatter<T, char, void>::parse(ctx);
////
////		auto it_colon = begin;
////		for (; it_colon != end && *it_colon != ':'; ++it_colon);
////		if (it_colon == end) {
////			sep_begin = &*begin;
////			sep_end = &*end;
////			ctx.advance_to(end);
////			return formatter<T, char, void>::parse(ctx);
////		}
////
////		sep_begin = &*begin;
////		sep_end = &*it_colon;
////		ctx.advance_to(it_colon + 1);
////		return formatter<T, char, void>::parse(ctx);
////	}
////
////	template <typename FormatContext>
////	auto format(const libv::vec_t<N, T>& vec, FormatContext& ctx) const {
////		auto out = ctx.out();
////
////		for (std::size_t i = 0; i < N; ++i) {
////			if (i != 0) {
////				for (auto s = sep_begin; s != sep_end; ++s)
////					*out++ = *s;
////				ctx.advance_to(out);
////			}
////			out = formatter<T, char, void>::format(vec[i], ctx);
////			ctx.advance_to(out);
////		}
////
////		return out;
////	}
////};
//
//
//template <>
//struct fmt::formatter<libv::version_number> {
//	bool hex = false;
//
//	constexpr auto parse(fmt::format_parse_context& ctx) {
//		auto it = ctx.begin();
//		if (it == ctx.end() || *it != 'x')
//			return it;
//		++it;
//		hex = true;
//		return it;
//	}
//
//	auto format(libv::version_number var, fmt::format_context& ctx) {
//		if (hex)
//			return fmt::format_to(ctx.out(), "{:x}", var.major);
//		else
//			return fmt::format_to(ctx.out(), "{}", var.major);
//	}
//};
//
//template <>
//struct fmt::formatter<libv::version_number> {
//
//	fmt::formatter<uint16_t> fmt_major;
//	fmt::formatter<uint16_t> fmt_minor;
//	fmt::formatter<uint16_t> fmt_patch;
//	fmt::formatter<uint16_t> fmt_build;
//
//	template <typename ParseContext>
//	constexpr auto parse(ParseContext& ctx) {
//		auto it = ctx.begin();
//		const auto end = ctx.end();
//
//		fmt_major.parse(fmt::format_parse_context(fmt::string_view{it, end}));
//
//	}

template <>
struct fmt::formatter<libv::version_number> : fmt::formatter<uint16_t> {

	template <typename FormatContext>
	auto format(const libv::version_number& var, FormatContext& ctx) const {
		auto out = ctx.out();

		out = formatter<uint16_t>::format(var.major, ctx);
		*out++ = '.';
		ctx.advance_to(out);
		out = formatter<uint16_t>::format(var.minor, ctx);
		// TODO P5: Use a custom formatting syntax to configure these rules like non printed patch or build number
		if (var.patch != 0 || var.build != 0) {
			*out++ = '.';
			ctx.advance_to(out);
			out = formatter<uint16_t>::format(var.patch, ctx);
		}
		if (var.build != 0) {
			*out++ = '.';
			ctx.advance_to(out);
			out = formatter<uint16_t>::format(var.build, ctx);
		}

		ctx.advance_to(out);
		return out;
	}
};

namespace libv { // --------------------------------------------------------------------------------

using build_id = uint32_t;

struct build_info {
	libv::version_number version_number = {0, 0, 0, 0};
	std::string version_name; /// Fictional name of the version: "Espionage"
	std::string variant; /// "Branch variant" Development / Beta / Canary / Live
	libv::build_id build_id = {0};

	// TODO P3: Use correct build_time type
	// TODO P3: Use correct build_uuid type
	// std::chrono::utc_clock::time_point build_time;
	std::string build_time;
	std::string build_uuid;

	// ? uint8_t stage;
	// 	Alpha
	// 	Beta
	// 	Release candidate
	// 	Release
	// 	Stable

	// std::optional<libv::hash::SHA1> git_hash;
	// std::string compiler;
	// std::string build_type;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
