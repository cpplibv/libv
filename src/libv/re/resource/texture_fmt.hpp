// Project: libv.re, File: src/libv/re/resource/texture_fmt.hpp

#pragma once

#include <fmt/format.h>

#include <libv/re/resource/texture.hpp>


template <>
struct fmt::formatter<libv::re::Texture> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const libv::re::Texture& var, FormatContext& ctx) {
		if (var.variant() == libv::re::VariantID::Default)
			return fmt::format_to(ctx.out(), "{}", var.identifier());
		else
			return fmt::format_to(ctx.out(), "{}|{}", var.identifier(), +var.variant());
	}
};
