// Project: libv.re, File: src/libv/re/resource/model_fmt.hpp

#pragma once

#include <fmt/format.h>

#include <libv/re/resource/model.hpp>


template <>
struct fmt::formatter<libv::re::Model> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const libv::re::Model& var, FormatContext& ctx) {
		if (var.variant() == libv::re::VariantID::Default)
			return fmt::format_to(ctx.out(), "{}", var.identifier());
		else
			return fmt::format_to(ctx.out(), "{}|{}", var.identifier(), +var.variant());
	}
};
