// Project: libv.re, File: src/libv/re/resource/shader_fmt.hpp

#pragma once

#include <fmt/format.h>

#include <libv/re/resource/shader.hpp>


template <>
struct fmt::formatter<libv::re::Shader> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const libv::re::Shader& var, FormatContext& ctx) {
		if (var.variant() == libv::re::VariantID::Default)
			return fmt::format_to(ctx.out(), "{}", var.name());
		else
			return fmt::format_to(ctx.out(), "{}|{}", var.name(), +var.variant());
	}
};
