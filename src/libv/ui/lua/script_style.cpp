// Project: libv.lua, File: src/libv/ui/lua/script_style.cpp

// hpp
#include <libv/ui/lua/script_style.hpp>
// libv
#include <libv/lua/lua.hpp>
#include <libv/lua/sol_type_to_string.hpp>
#include <libv/parse/bool.hpp>
#include <libv/parse/color.hpp>
#include <libv/range/view_lines_string_view.hpp>
#include <libv/utility/hash_string.hpp>
#include <libv/utility/timer.hpp>
#include <libv/utility/trim.hpp>
#include <libv/lua/convert_color.hpp>
// std
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
// pro
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/parse/parse_align.hpp>
#include <libv/ui/parse/parse_anchor.hpp>
#include <libv/ui/parse/parse_orientation.hpp>
#include <libv/ui/parse/parse_orientation2.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/column_count.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/orientation2.hpp>
#include <libv/ui/property/padding.hpp>
#include <libv/ui/property/size.hpp>
#include <libv/ui/property/spacing.hpp>
#include <libv/ui/property/z_index_offset.hpp>
//#include <libv/ui/property/font_2D.hpp>
//#include <libv/ui/property/scroll_area_mode.hpp>
//#include <libv/ui/property/shader_font.hpp>
//#include <libv/ui/property/shader_image.hpp>
//#include <libv/ui/property/shader_quad.hpp>
//#include <libv/ui/property/snap_to_edge.hpp>
//#include <libv/ui/property/squish.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/ui.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//auto try_parse(sol::object object, auto parse_func);

//template <typename T>
//struct op_result {
//	std::optional<T> value_;
//
//public:
//	explicit operator bool() {
//		return value_.has_value();
//	}
//
//	T* operator->() {
//		return value_ ? &*value_ : nullptr;
//	}
//};
//
//template <typename T>
//struct try_table_ctx {
//	sol::table table;
////	std::optional<T> result_;
//
//public:
//	explicit operator bool() {
////		return table;
//	}
//
//public:
////	template <typename ParseFunc>
////	bool parse(ParseFunc&& func) {}
////
////	template <typename U>
////	bool usertype() {}
////
////	template <typename U, typename Transform>
////	bool usertype(Transform&& func) {}
////
////	template <typename MemberMatching>
////	bool table(MemberMatching&& func) {}
////
////	std::optional<T> result() {
////		return result_;
////	}
//
//	op_result<float> float_(int32_t key) {
//		return table.get(key);
//	}
//
//	op_result<float> float_(std::string_view key) {
//		return table.get(key);
//	}
//
//	op_result<float> float_or(int32_t key, float fallback) {
//		if (auto m = table.get(key))
//			return m;
//		else
//			return fallback;
//	}
//
//	op_result<float> float_or(std::string_view key, float fallback) {
//		if (auto m = table.get(key))
//			return m;
//		else
//			return fallback;
//	}
//};
//
//template <typename T>
//struct try_parse_ctx {
//	sol::object object;
//	std::optional<T> result_;
//
//public:
//	explicit try_parse_ctx(sol::object object) :
//		object(std::move(object)) {}
//
//public:
//	op_result<std::string_view> string() {
////		return table.get(key);
//	}
//
//	template <typename ParseFunc>
//	bool parse(ParseFunc&& func) {}
//
//	template <typename U>
//	bool usertype() {}
//
//	template <typename U, typename Transform>
//	bool usertype(Transform&& func) {}
//
//	try_table_ctx table() {}
//
//	template <typename... Args>
//	bool emplace_result(Args&&... args) {
////		if constexpr(op_result<Args>) unpack
//	}
//
//	template <typename... Args>
//	bool collect(Args&&... args) {
////		if constexpr(op_result<Args>) unpack
//	}
//
//	std::optional<T> result() {
//		return result_;
//	}
//};


//std::optional<PropertyDynamic> convert_color(UI& ui, const sol::object& object) {
//try_parse_ctx<libv::vec4f> matcher;
//
//	if (matcher.parse(libv::parse::parse_color_optional)) return matcher.result();
//	if (auto vec3 = matcher.usertype<libv::vec3f>()) return matcher.emplace_result(vec3, 1.0f);
//	if (matcher.usertype<libv::vec4f>()) return matcher.result();
//	if (auto table = matcher.table()) {
//		if (matcher.collect(table.float_(1), table.float_(2), table.float_(3), table.float_or(4, 1.0f))) return matcher.result();
//		if (matcher.collect(table.float_("x"), table.float_("y"), table.float_("z"), table.float_or("w", 1.0f))) return matcher.result();
//		if (matcher.collect(table.float_("r"), table.float_("g"), table.float_("b"), table.float_or("a", 1.0f))) return matcher.result();
//	}
//}

// =================================================================================================


//template <typename F>
//inline const auto pattern_protocol(sol::object lua_object, std::string protocol, F&& func) {
////	try_parse_ctx<Background> matcher(lua_object);
//	try_parse_ctx<Texture2D_view> matcher(lua_object);
//
//	if (auto str = matcher.string()) {
//		if (str->starts_with(protocol)) {
//			str->remove_prefix(protocol.size());
//			return matcher.emplace_result(func(str->remove_prefix(str)));
//		}
//	}
//
//	return matcher.result();
//}

//template <typename F>
//inline const auto pattern_protocol(std::string protocol, F&& func) {
//	auto parse_func = [protocol = std::move(protocol)](auto str) {
//		if (not str.starts_with(protocol))
//			return std::optional<std::string_view>(std::nullopt);
//
//		str.remove_prefix(protocol.size());
//		return std::optional<std::string_view>(str);
//	};
//
//	return libv::lua::transform(libv::lua::string_parse(std::move(parse_func)), std::forward<F>(func));
//}

template <typename Enum>
struct convert_enum_value {
	std::optional<PropertyDynamic> operator()(UI&, const sol::object& object) {
		if (object.get_type() != sol::type::number)
			return std::nullopt;

		return PropertyDynamic{static_cast<Enum>(object.as<int64_t>())};
	}
};

template <typename T>
struct convert_userdata {
	std::optional<PropertyDynamic> operator()(UI&, const sol::object& object) {
		if (!object.is<T>())
			return std::nullopt;

		return PropertyDynamic{object.as<T>()};
	}
};

const auto convert_string_parse = [](auto&& parse_fn) {
	return [parse_function = std::forward<decltype(parse_fn)>(parse_fn)](UI&, const sol::object& object) -> std::optional<PropertyDynamic> {
		if (object.get_type() != sol::type::string)
			return std::nullopt;

		auto result_opt = parse_function(object.as<std::string_view>());
		if (!result_opt)
			return std::nullopt;

		return PropertyDynamic{std::move(*result_opt)};
	};
};

template <typename F>
auto direct_fn(F&& fn) {
	if constexpr (requires { fn(std::declval<UI&>(), std::declval<const sol::object&>()); })
		return std::forward<F>(fn);
	else
		return [fn = std::forward<decltype(fn)>(fn)](UI&, const sol::object& object) {
			return fn(object);
		};
}

const auto conv_fn = [](auto&& fn) {
	return [fn = direct_fn(std::forward<decltype(fn)>(fn))](UI& ui, const sol::object& object) -> std::optional<PropertyDynamic> {
		auto result_opt = fn(ui, object);
		if (!result_opt)
			return std::nullopt;

		return PropertyDynamic{std::move(*result_opt)};
	};
};

std::optional<libv::vec2f> convert_extent2(UI&, const sol::object& object) {
	if (object.get_type() == sol::type::number) {
		// left_down_right_top
		return libv::vec2f::one(object.as<float>());

	} else if (object.get_type() == sol::type::userdata) {
		if (object.is<libv::vec2f>())
			// left_right, down_top
			return object.as<libv::vec2f>();

		else
			return std::nullopt;

	} else if (object.get_type() == sol::type::table) {
		auto table = object.as<sol::table>();

		auto size = table.size();
		if (size == 2) {
			auto n1 = table.get<sol::object>(1);
			auto n2 = table.get<sol::object>(2);
			if (n1.get_type() != sol::type::number || n2.get_type() != sol::type::number)
				return std::nullopt;

			const auto v1 = n1.as<float>();
			const auto v2 = n2.as<float>();

			return libv::vec2f(v1, v2);

		} else if (size == 1) {
			auto n1 = table.get<sol::object>(1);
			if (n1.get_type() != sol::type::number)
				return std::nullopt;

			const auto v1 = n1.as<float>();

			return libv::vec2f::one(v1);

		} else {
			return std::nullopt;
		}

	} else {
		return std::nullopt;
	}
}

std::optional<Padding> convert_extent(UI&, const sol::object& object) {
	if (object.get_type() == sol::type::number) {
		// left_down_right_top
		return libv::vec4f::one(object.as<float>());

	} else if (object.get_type() == sol::type::userdata) {
		if (object.is<libv::vec2f>())
			// left_right, down_top
			return libv::vec4f(xyxy(object.as<libv::vec2f>()));

		else if (object.is<libv::vec4f>())
			return object.as<libv::vec4f>();

		else
			return std::nullopt;

	} else if (object.get_type() == sol::type::table) {
		auto table = object.as<sol::table>();

//		load_members<libv::vec4f, float>(1, 2, 3, m_opt_float<float>(4, 1.0f));
//		load_members<libv::vec4f, float>("r", "g", "b", m_opt_float<float>("a", 1.0f));
//		load_members<libv::vec4f, float>("x", "y", "z", m_opt_float<float>("w", 1.0f));

		auto size = table.size();
		if (size == 4) {
			auto n1 = table.get<sol::object>(1);
			auto n2 = table.get<sol::object>(2);
			auto n3 = table.get<sol::object>(3);
			auto n4 = table.get<sol::object>(4);
			if (n1.get_type() != sol::type::number || n2.get_type() != sol::type::number || n3.get_type() != sol::type::number || n4.get_type() != sol::type::number)
				return std::nullopt;

			const auto v1 = n1.as<float>();
			const auto v2 = n2.as<float>();
			const auto v3 = n3.as<float>();
			const auto v4 = n4.as<float>();
			return libv::vec4f(v1, v2, v3, v4);

		} else if (size == 2) {
			auto n1 = table.get<sol::object>(1);
			auto n2 = table.get<sol::object>(2);
			if (n1.get_type() != sol::type::number || n2.get_type() != sol::type::number)
				return std::nullopt;

			const auto v1 = n1.as<float>();
			const auto v2 = n2.as<float>();
			// left_right, down_top
			return libv::vec4f(v1, v2, v1, v2);

		} else if (size == 1) {
			auto n1 = table.get<sol::object>(1);
			if (n1.get_type() != sol::type::number)
				return std::nullopt;

			const auto v1 = n1.as<float>();
			// left_down_right_top
			return libv::vec4f::one(v1);

		} else {
			return std::nullopt;
		}

	} else {
		return std::nullopt;
	}
}

std::optional<Texture2D_view> convert_texture(UI& ui, const sol::object& object) {
	if (object.get_type() != sol::type::string)
		return std::nullopt;

	return ui.context().texture2D(object.as<std::string_view>());
}

template <typename F, typename T>
auto convert_member(UI& ui, const sol::table& table, std::string_view key, F&& convert_fn, T&& fallback_if_missing) {
	const auto member = table.get<sol::object>(key);

	if (member.valid())
		return convert_fn(ui, member);
	else
		return decltype(convert_fn(ui, member)){std::forward<T>(fallback_if_missing)};
}

template <typename F>
auto convert_member(UI& ui, const sol::table& table, std::string_view key, F&& convert_fn) {
	const auto member = table.get<sol::object>(key);

	if (member.valid())
		return convert_fn(ui, member);
	else
		return decltype(convert_fn(ui, member)){std::nullopt};
}

std::optional<PropertyDynamic> convert_background(UI& ui, const sol::object& object) {

	if (object.get_type() == sol::type::string) {

		const auto chop_prefix = [](std::string_view str, std::string_view prefix) -> std::optional<std::string_view> {
			if (!str.starts_with(prefix))
				return std::nullopt;

			str.remove_prefix(prefix.size());
			return libv::trim(str);
		};

		auto str = libv::trim(object.as<std::string_view>());

		//	none               ()
		if (str == "none")
			return libv::ui::Background::none();

		//	color              (Color color)
		if (const auto value = chop_prefix(str, "color:")) {
			const auto color = libv::parse::parse_color_optional(*value);
			if (!color)
				return std::nullopt;

			return libv::ui::Background::color(*color);
		}

		//	color              (Color color, ShaderQuad_view shader)
		//	texture            (Color color, Texture2D_view texture)
		if (const auto value = chop_prefix(str, "texture:")) {
			// TODO P2: Handle not found resource?
			return libv::ui::Background::texture({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D(*value));
		}

		//	texture            (Color color, Texture2D_view texture, ShaderImage_view shader)
		//	border             (Color color, Texture2D_view texture)
		if (const auto value = chop_prefix(str, "border:")) {
			// TODO P2: Handle not found resource?
			return libv::ui::Background::border({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D(*value));
		}

		//	border             (Color color, Texture2D_view texture, ShaderImage_view shader)
		//	pattern            (Color color, Texture2D_view texture)
		if (const auto value = chop_prefix(str, "pattern:")) {
			// TODO P2: Handle not found resource?
			return libv::ui::Background::pattern({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D(*value));
		}

		//	pattern            (Color color, Texture2D_view texture, ShaderImage_view shader)
		//	padding_pattern    (Color color, Padding inner_padding, Texture2D_view texture)
		if (const auto value = chop_prefix(str, "padding_pattern:")) {
			// TODO P2: Handle not found resource?
			return libv::ui::Background::padding_pattern({1.f, 1.f, 1.f, 1.f}, {0, 0, 0, 0}, ui.context().texture2D(*value));
		}
		//	padding_pattern    (Color color, Padding inner_padding, Texture2D_view texture, ShaderImage_view shader)
		//	gradient_linear    (std::vector<GradientPoint> points)
		//	gradient_linear    (std::vector<GradientPoint> points, ShaderQuad_view shader)

		// Last resort 1/2: try parse it as color
		const auto color = libv::parse::parse_color_optional(str);
		if (color)
			return libv::ui::Background::color(*color);

		// Last resort 2/2: try parse it as texture
		if (ui.context().texture2D_exists(str))
			return libv::ui::Background::texture({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D(str));

		return std::nullopt;
	}

	if (object.get_type() == sol::type::table) {
		const auto table = object.as<sol::table>();

		const auto member_type = table.get<sol::object>("type");
		if (member_type.get_type() != sol::type::string)
			return std::nullopt;

		const auto type_str = member_type.as<std::string_view>();

		if (type_str == "none") {
			return libv::ui::Background::none();

		} else if (type_str == "color") {
			const auto color = convert_member(ui, table, "color", direct_fn(libv::lua::convert_color));
			if (!color)
				return std::nullopt;

//			const auto shader = convert_shader(table.get<sol::object>("shader"));
//			if (shader)
//				return libv::ui::Background::color(*color, *shader);
//			else
			return libv::ui::Background::color(*color);

		} else if (type_str == "texture") {
			const auto color = convert_member(ui, table, "color", direct_fn(libv::lua::convert_color), Color{1, 1, 1, 1});
			if (!color)
				return std::nullopt;

			const auto texture = convert_member(ui, table, "texture", convert_texture);
			if (!texture)
				return std::nullopt;

//			const auto shader = convert_shader(table.get<sol::object>("shader"));
//			if (shader)
//				return libv::ui::Background::texture(color, *texture, *shader);
//			else
			return libv::ui::Background::texture(*color, *texture);

		} else if (type_str == "border") {
			const auto color = convert_member(ui, table, "color", direct_fn(libv::lua::convert_color), Color{1, 1, 1, 1});
			if (!color)
				return std::nullopt;

			const auto texture = convert_member(ui, table, "texture", convert_texture);
			if (!texture)
				return std::nullopt;

//			const auto shader = convert_shader(table.get<sol::object>("shader"));
//			if (shader)
//				return libv::ui::Background::border(*color, *texture, *shader);
//			else
			return libv::ui::Background::border(*color, *texture);

		} else if (type_str == "pattern") {
			const auto color = convert_member(ui, table, "color", direct_fn(libv::lua::convert_color), Color{1, 1, 1, 1});
			if (!color)
				return std::nullopt;

			const auto texture = convert_member(ui, table, "texture", convert_texture);
			if (!texture)
				return std::nullopt;

//			const auto shader = convert_shader(table.get<sol::object>("shader"));
//			if (shader)
//				return libv::ui::Background::border(*color, *texture, *shader);
//			else
			return libv::ui::Background::pattern(*color, *texture);

		} else if (type_str == "padding_pattern") {
			const auto color = convert_member(ui, table, "color", direct_fn(libv::lua::convert_color), Color{1, 1, 1, 1});
			if (!color)
				return std::nullopt;

			const auto inner_padding = convert_member(ui, table, "inner_padding", convert_extent, libv::vec4f{0, 0, 0, 0});
			if (!inner_padding)
				return std::nullopt;

			const auto texture = convert_member(ui, table, "texture", convert_texture);
			if (!texture)
				return std::nullopt;

//			const auto shader = convert_shader(table.get<sol::object>("shader"));
//			if (shader)
//				return libv::ui::Background::padding_pattern(*color, *inner_padding, *texture, *shader);
//			else
			return libv::ui::Background::padding_pattern(*color, *inner_padding, *texture);

		} else if (type_str == "border_padding_pattern") {
			const auto color_border = convert_member(ui, table, "color_border", direct_fn(libv::lua::convert_color), Color{1, 1, 1, 1});
			if (!color_border)
				return std::nullopt;

			const auto color_pattern = convert_member(ui, table, "color_pattern", direct_fn(libv::lua::convert_color), Color{1, 1, 1, 1});
			if (!color_pattern)
				return std::nullopt;

			const auto inner_padding = convert_member(ui, table, "inner_padding", convert_extent, libv::vec4f{0, 0, 0, 0});
			if (!inner_padding)
				return std::nullopt;

			const auto border_extent = convert_member(ui, table, "border_extent", convert_extent, libv::vec4f{0, 0, 0, 0});
			if (!border_extent)
				return std::nullopt;

			const auto texture = convert_member(ui, table, "texture", convert_texture);
			if (!texture)
				return std::nullopt;

//			const auto shader = convert_shader(table.get<sol::object>("shader"));
//			if (shader)
//				return libv::ui::Background::border_padding_pattern(*color, *inner_padding, *texture, *shader);
//			else
			return libv::ui::Background::border_padding_pattern(*color_border, *color_pattern, *inner_padding, *border_extent, *texture);

//		} else if (type_str == "padding_pattern") {
//			//	gradient_linear    (std::vector<GradientPoint> points)
//			//	gradient_linear    (std::vector<GradientPoint> points, ShaderQuad_view shader)
		}
	}

	return std::nullopt;
}

std::optional<PropertyDynamic> convert_font(UI& ui, const sol::object& object) {
	if (object.get_type() != sol::type::string)
		return std::nullopt;

	return PropertyDynamic{ui.context().font(object.as<std::string_view>())};
}

using load_fn = std::function<std::optional<PropertyDynamic>(UI&, const sol::object&)>;

// =================================================================================================

struct LoadLuaContext {
	std::unordered_map<std::string, load_fn, libv::hash_string, std::equal_to<>> property_loaders;
	std::unordered_map<std::string, std::string, libv::hash_string, std::equal_to<>> property_aliases;

	UI& ui;

	StyleState current_state_mask = StyleState::none;
	StyleState current_state_value = StyleState::none;

public:
	explicit LoadLuaContext(UI& ui, lua::State& lua) :
		ui(ui) {

		property_loaders.emplace(pnm::align_horizontal, convert_string_parse(&libv::ui::parse_align_horizontal_optional));
		property_loaders.emplace(pnm::align_vertical, convert_string_parse(&libv::ui::parse_align_vertical_optional));
		property_loaders.emplace(pnm::anchor, convert_string_parse(&libv::ui::parse_anchor_optional));
		//		property_loaders.emplace(pnm::area_position, _______);
		//		property_loaders.emplace(pnm::area_size, _______);
		property_loaders.emplace(pnm::background, convert_background);
		property_loaders.emplace(pnm::bar_color, conv_fn(libv::lua::convert_color));
		property_loaders.emplace(pnm::bar_image, conv_fn(convert_texture));
//		property_loaders.emplace(pnm::bar_shader, _______);
		//		property_loaders.emplace(pnm::caret, _______);
		property_loaders.emplace(pnm::caret_color, conv_fn(libv::lua::convert_color));
		//		property_loaders.emplace(pnm::caret_shader, _______);
		property_loaders.emplace(pnm::color, conv_fn(libv::lua::convert_color));
		property_loaders.emplace(pnm::column_count, convert_enum_value<ColumnCount>());
//		property_loaders.emplace(pnm::focus_select_policy, _______);
		property_loaders.emplace(pnm::font, convert_font);
//		property_loaders.emplace(pnm::font_outline, convert_font_outline);
		property_loaders.emplace(pnm::font_color, conv_fn(libv::lua::convert_color));
//		property_loaders.emplace(pnm::font_shader, _______);
		property_loaders.emplace(pnm::font_size, convert_enum_value<FontSize>());
		property_loaders.emplace(pnm::margin, conv_fn(convert_extent));
		property_loaders.emplace(pnm::orientation, convert_string_parse(&libv::ui::parse_orientation_optional));
		property_loaders.emplace(pnm::orientation2, convert_string_parse(&libv::ui::parse_orientation2_optional));
		property_loaders.emplace(pnm::padding, conv_fn(convert_extent));
//		property_loaders.emplace(pnm::quad_shader, _______);
//		property_loaders.emplace(pnm::scroll_area_mode, _______);
		property_loaders.emplace(pnm::size, convert_string_parse(&libv::ui::parse_size_optional));
//		property_loaders.emplace(pnm::snap_to_edge, _______);
		property_loaders.emplace(pnm::spacing, convert_enum_value<Spacing>());
		property_loaders.emplace(pnm::spacing2, conv_fn(convert_extent2));
//		property_loaders.emplace(pnm::squish, _______);
		property_loaders.emplace(pnm::z_index_offset, convert_enum_value<ZIndexOffset>());
		//		property_loaders.emplace(pnm::text, _______);
		//		property_loaders.emplace(pnm::value, _______);
		//		property_loaders.emplace(pnm::value_max, _______);
		//		property_loaders.emplace(pnm::value_min, _______);
		//		property_loaders.emplace(pnm::value_range, _______);
		//		property_loaders.emplace(pnm::value_step, _______);

		property_loaders.emplace("test_number_3", convert_enum_value<FontSize>());
		property_loaders.emplace("test_string_3", convert_enum_value<FontSize>());

		// TODO P3: Use aliases
		property_aliases.emplace("align", pnm::align_horizontal);
		property_aliases.emplace("orient", pnm::orientation);

		bind(lua);
	}

	void load_style_property(Style& style, const std::string_view& key, const sol::object& value) {

		auto it = property_loaders.find(key);
		if (it == property_loaders.end())
			return log_ui.warn("Ignoring unrecognized property style {} property {} value \"{}\".",
					style.style_name, key, value.as<std::string_view>());

		const auto& loader = it->second;
		auto result = loader(ui, value);
		if (!result)
			return log_ui.error("Failed to load style {} property {} value \"{}\"",
					style.style_name, key, value.as<std::string_view>());

		style.set(current_state_mask, current_state_value, std::string(key), std::move(*result));
	}

	void load_style_table(Style& style, const sol::table& table) {
		// Process normal key value pairs first
		for (const auto& [key, value] : table) {
			if (key.get_type() != sol::type::string)
				continue; // Skip non key-value pairs (where key is string and therefore yields a property)

			const auto key_str = key.as<std::string_view>();
			if (key_str == "state_condition")
				continue; // Skip state_condition marker, it will be read by another get

			load_style_property(style, key_str, value);
		}

		// Process Sub Tables (They are most likely stated property tables)
		// NOTE: These tables are loaded after the key-value pairs were already loaded on the current level
		for (const auto& [key, value] : table) {
			if (key.get_type() != sol::type::number)
				continue;

			const auto sub_table = sol::table(value);

			const auto state_member = sub_table.get<sol::object>("state_condition");
			if (!state_member.valid() || state_member.get_type() != sol::type::string) {
				log_ui.warn_if(state_member.valid(), "Expected state_condition member in style sub-table of {} to be a string but was {}. Processing the table without constraint", style.style_name, libv::lua::lua_type_to_string(state_member.get_type()));

				// Recurse into stated sub table without extra state_condition
				load_style_table(style, sub_table);

			} else {
				const auto state_str = state_member.as<std::string_view>();

				const auto old_state_mask = current_state_mask;
				const auto old_state_value = current_state_value;

				for (auto state_str_member : libv::view::lines_string_view(state_str, ',')) {
					if (state_str_member.ends_with(','))
						state_str_member.remove_suffix(1);
					auto s = libv::trim(state_str_member);

//				for (const std::string_view state_str_member : std::views::split(state_str, ',')) { // GCC 12
//					const auto s = libv::trim(state_str_member);

					const auto negate = s.starts_with('!');
					if (negate) {
						s.remove_prefix(1);
						s = libv::trim_front(s);
					}

					const auto process_state = [&](std::string_view state_name, StyleState state_enum) {
						if (s != state_name)
							return false;

						current_state_mask = current_state_mask | state_enum;
						if (negate)
							current_state_value = current_state_value & ~state_enum;
						else
							current_state_value = current_state_value | state_enum;

						return true;
					};

					if (process_state("disable", StyleState::disable)) continue;
					if (process_state("hover", StyleState::hover)) continue;
					if (process_state("focus", StyleState::focus)) continue;
					if (process_state("active", StyleState::active)) continue;
					if (process_state("select", StyleState::select)) continue;
					if (process_state("progress", StyleState::progress)) continue;
					if (process_state("done", StyleState::done)) continue;
					if (process_state("error", StyleState::error)) continue;

					if (process_state("custom0", StyleState::custom0)) continue;
					if (process_state("custom1", StyleState::custom1)) continue;
					if (process_state("custom2", StyleState::custom2)) continue;
					if (process_state("custom3", StyleState::custom3)) continue;
					if (process_state("custom4", StyleState::custom4)) continue;
					if (process_state("custom5", StyleState::custom5)) continue;
					if (process_state("custom6", StyleState::custom6)) continue;
					if (process_state("custom7", StyleState::custom7)) continue;
					if (process_state("custom8", StyleState::custom8)) continue;
					if (process_state("custom9", StyleState::custom9)) continue;
					if (process_state("custom10", StyleState::custom10)) continue;
					if (process_state("custom11", StyleState::custom11)) continue;
					if (process_state("custom12", StyleState::custom12)) continue;
					if (process_state("custom13", StyleState::custom13)) continue;
					if (process_state("custom14", StyleState::custom14)) continue;
					if (process_state("custom15", StyleState::custom15)) continue;

					log_ui.warn("Ignoring unrecognized state {} in style {}", s, style.style_name);
				}

				// Recurse into stated sub table
				load_style_table(style, sub_table);

				current_state_mask = old_state_mask;
				current_state_value = old_state_value;
			}
		}

		// Warn on any unexpected types
		for (const auto& [key, value] : table) {
			if (key.get_type() == sol::type::string || key.get_type() == sol::type::number)
				continue;

			log_ui.warn("Style's property key \"{}\" is expected to be a string. Entry ignored: Style name: {}, key: {}, value: {}",
					key.as<std::string_view>(), style.style_name, key.as<std::string_view>(), value.as<std::string_view>());
		}
	}

	void bind(lua::State& lua) {
		auto ui_table = lua.create_named_table("ui");

	//	ui_table["bottom"] = "bottom";
	//	ui_table["center"] = "center";
	//	ui_table["justify"] = "justify";
	//	ui_table["justifyall"] = "justifyall";
	//	ui_table["left"] = "left";
	//	ui_table["right"] = "right";
	//	ui_table["top"] = "top";
	//
	//	ui_table["ORIENT_Orientation::up"] = Orientation::up;
	//	ui_table["ORIENT_Orientation::right"] = Orientation::right;
	//	ui_table["ORIENT_Orientation::left"] = Orientation::left;
	//	ui_table["ORIENT_Orientation::down"] = Orientation::down;
	//
	//	ui_table.set_function("color", [](const std::string_view color_str) -> sol::object {
	//		auto color = libv::parse::parse_color_optional(color_str);
	//		if (color)
	//			return *color;
	//		else
	//			return sol::nil;
	//	});

		ui_table.set_function("style", [this](const std::string_view style_name) mutable {
			auto style_ip = ui.context().style(style_name);

			return [style_ip = std::move(style_ip), this](const sol::table& table) {
				style_ip->clear();

				load_style_table(*style_ip, table);
			};
		});

		lua.set_function("state", [](std::string state_string) {
			return [state_string = std::move(state_string)](sol::table table) {
				table["state_condition"] = state_string;
				return table;
			};
		});
	}
};

void script_style(UI& ui, lua::State& lua, const std::string_view script_str) {
	libv::Timer timer;
	LoadLuaContext context(ui, lua);

	try {
		// TODO P1: Create a sandbox around the script execution
		const auto result = lua.safe_script(script_str, sol::script_throw_on_error);

		if (!result.valid()) {
			sol::error err = result;
			return log_ui.error("Style script execution failed: {}", err.what());
		}

		if (result.get_type() != sol::type::none)
			log_ui.warn("Style script return value is unused: {}:{} - {}", libv::to_underlying(result.get_type()), libv::lua::lua_type_to_string(result.get_type()), std::string(result));

		log_ui.info("Style script loading successful in {:7.3f}ms", timer.timef_ms().count());

	} catch (const std::exception& e) {
		log_ui.error("Style script execution failed: {}", e.what());
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
