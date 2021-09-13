// Project: libv.ui, File: src/libv/ui/ui_lua.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/ui_lua.hpp>
// libv
#include <libv/lua/lua.hpp>
#include <libv/parse/bool.hpp>
#include <libv/parse/color.hpp>
#include <libv/utility/trim.hpp>
//#include <libv/algo/slice.hpp>
// std
#include <memory>
#include <string>
#include <optional>
#include <string_view>
// pro
//#include <libv/ui/component/detail/component.hpp>
//#include <libv/ui/component/label.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/parse/parse_align.hpp>
#include <libv/ui/parse/parse_anchor.hpp>
#include <libv/ui/parse/parse_orientation.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/spacing.hpp>
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

		// TODO P1: Implement the rest of the background types
		// TODO P1: Implement lua table based background loading, rely on other convert functions to do so, generalize convert function API for this

		//	border             (Color color, Texture2D_view texture, ShaderImage_view shader)
		//	pattern            (Color color, Texture2D_view texture)
		if (const auto value = chop_prefix(str, "pattern:")) {
			// TODO P2: Handle not found resource?
			return libv::ui::Background::pattern({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D(*value));
		}

		//	pattern            (Color color, Texture2D_view texture, ShaderImage_view shader)
		//	padding_pattern    (Color color, Texture2D_view texture)
		if (const auto value = chop_prefix(str, "padding_pattern:")) {
			// TODO P2: Handle not found resource?
			return libv::ui::Background::padding_pattern({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D(*value));
		}
		//	padding_pattern    (Color color, Texture2D_view texture, ShaderImage_view shader)
		//	gradient_linear    (std::vector<GradientPoint> points)
		//	gradient_linear    (std::vector<GradientPoint> points, ShaderQuad_view shader)

		// TODO P1: last resort: try parse it as color or texture
	}

//	UI& ui;
//
//	if (not is_string())
//		return;
//
//	pattern_protocol("image ", [&ui](const auto path){ return ui.context().texture2D(path); });
//	return pattern_protocol("color ", [&ui](const auto path){ return ui.context().texture2D(path); });
//	return pattern_protocol("stretch ", [&ui](const auto path){ return ui.context().texture2D(path); });
//	pattern_protocol("color ", ),
//	pattern_protocol("stretch ", ),

	return std::nullopt;
}

std::optional<PropertyDynamic> convert_color(UI&, const sol::object& object) {

	if (object.get_type() == sol::type::string) {
		return {libv::parse::parse_color_optional(object.as<std::string_view>())};

	} else if (object.get_type() == sol::type::userdata) {
		if (object.is<libv::vec3f>())
			return libv::vec4f(object.as<libv::vec3f>(), 1.0f);

		else if (object.is<libv::vec4f>())
			return object.as<libv::vec4f>();

		else
			return std::nullopt;

	} else if (object.get_type() == sol::type::table) {
		auto table = object.as<sol::table>();
		{
			auto n1 = table.get<sol::object>(1);
			auto n2 = table.get<sol::object>(2);
			auto n3 = table.get<sol::object>(3);
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number) {
				auto n4 = table.get<sol::object>(4);
				if (n4.get_type() == sol::type::number)
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), n4.as<float>());
				else
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), 1.0f);
			}
		}
		{
			auto n1 = table.get<sol::object>("r");
			auto n2 = table.get<sol::object>("g");
			auto n3 = table.get<sol::object>("b");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number) {
				auto n4 = table.get<sol::object>("a");
				if (n4.get_type() == sol::type::number)
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), n4.as<float>());
				else
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), 1.0f);
			}
		}
		{
			auto n1 = table.get<sol::object>("x");
			auto n2 = table.get<sol::object>("y");
			auto n3 = table.get<sol::object>("z");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number) {
				auto n4 = table.get<sol::object>("w");
				if (n4.get_type() == sol::type::number)
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), n4.as<float>());
				else
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), 1.0f);
			}
		}

		return std::nullopt;

	} else {
		return std::nullopt;
	}
}

std::optional<PropertyDynamic> convert_margin_padding(UI&, const sol::object& object) {
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

std::optional<PropertyDynamic> convert_font(UI& ui, const sol::object& object) {
	if (object.get_type() != sol::type::string)
		return std::nullopt;

	return ui.context().font(object.as<std::string_view>());
}

std::optional<PropertyDynamic> convert_image(UI& ui, const sol::object& object) {
	if (object.get_type() != sol::type::string)
		return std::nullopt;

	return ui.context().texture2D(object.as<std::string_view>());
}

template <typename Enum>
struct convert_enum_value {
	std::optional<PropertyDynamic> operator()(UI&, const sol::object& object) {
		if (object.get_type() != sol::type::number)
			return std::nullopt;

		return {static_cast<Enum>(object.as<int64_t>())};
	}
};

template <typename T>
struct convert_userdata {
	std::optional<PropertyDynamic> operator()(UI&, const sol::object& object) {
		if (!object.is<T>())
			return std::nullopt;

		return {object.as<T>()};
	}
};

auto convert_string_parse = [](auto parse_function) {
	return [parse_function](UI&, const sol::object& object) -> std::optional<PropertyDynamic> {
		if (object.get_type() != sol::type::string)
			return std::nullopt;

		return {parse_function(object.as<std::string_view>())};
	};
};

using load_fn = std::function<std::optional<PropertyDynamic>(UI&, const sol::object&)>;

// =================================================================================================

struct LoadLuaContext {
	std::unordered_map<std::string, load_fn> property_loaders;
	std::unordered_map<std::string, std::string> property_aliases;

	UI& ui;

public:
	explicit LoadLuaContext(UI& ui, lua::State& lua) :
		ui(ui) {

		property_loaders.emplace(pnm::align_horizontal, convert_string_parse(&libv::ui::parse_align_horizontal_optional));
		property_loaders.emplace(pnm::align_vertical, convert_string_parse(&libv::ui::parse_align_vertical_optional));
		property_loaders.emplace(pnm::anchor, convert_string_parse(&libv::ui::parse_anchor_optional));
		//		property_loaders.emplace(pnm::area_position, _______);
		//		property_loaders.emplace(pnm::area_size, _______);
		property_loaders.emplace(pnm::background, convert_background);
		property_loaders.emplace(pnm::bar_color, convert_color);
		property_loaders.emplace(pnm::bar_image, convert_image);
//		property_loaders.emplace(pnm::bar_shader, _______);
		property_loaders.emplace(pnm::bg_color, convert_color);
		property_loaders.emplace(pnm::bg_image, convert_image);
//		property_loaders.emplace(pnm::bg_shader, _______);
		//		property_loaders.emplace(pnm::caret, _______);
		property_loaders.emplace(pnm::caret_color, convert_color);
		//		property_loaders.emplace(pnm::caret_shader, _______);
		property_loaders.emplace(pnm::color, convert_color);
//		property_loaders.emplace(pnm::column_count, _______);
//		property_loaders.emplace(pnm::focus_select_policy, _______);
		property_loaders.emplace(pnm::font, convert_font);
//		property_loaders.emplace(pnm::font_outline, convert_font_outline);
		property_loaders.emplace(pnm::font_color, convert_color);
//		property_loaders.emplace(pnm::font_shader, _______);
		property_loaders.emplace(pnm::font_size, convert_enum_value<FontSize>());
		property_loaders.emplace(pnm::margin, convert_margin_padding);
		property_loaders.emplace(pnm::orientation, convert_string_parse(&libv::ui::parse_orientation_optional));
//		property_loaders.emplace(pnm::orientation2, _______);
		property_loaders.emplace(pnm::padding, convert_margin_padding);
//		property_loaders.emplace(pnm::quad_shader, _______);
//		property_loaders.emplace(pnm::scroll_area_mode, _______);
		property_loaders.emplace(pnm::size, convert_string_parse(&libv::ui::parse_size_optional));
//		property_loaders.emplace(pnm::snap_to_edge, _______);
		property_loaders.emplace(pnm::spacing, convert_enum_value<Spacing>());
//		property_loaders.emplace(pnm::spacing2, _______);
//		property_loaders.emplace(pnm::squish, _______);
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
	//	ui_table["ORIENT_BOTTOM_TO_TOP"] = Orientation::BOTTOM_TO_TOP;
	//	ui_table["ORIENT_LEFT_TO_RIGHT"] = Orientation::LEFT_TO_RIGHT;
	//	ui_table["ORIENT_RIGHT_TO_LEFT"] = Orientation::RIGHT_TO_LEFT;
	//	ui_table["ORIENT_TOP_TO_BOTTOM"] = Orientation::TOP_TO_BOTTOM;

		ui_table.set_function("style", [this](const std::string_view style_name) mutable {
			auto style_ip = ui.context().style(style_name);

			return [style_ip = std::move(style_ip), this](const sol::table& table) {
				style_ip->clear();

				for (const auto& [key, value] : table) {
					if (key.get_type() != sol::type::string) {
						log_ui.warn("Style's property key \"{}\" is expected to be a string. Entry ignored: Style name: {}, key: {}, value: {}",
								key.as<std::string_view>(), style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());
						continue;
					}

					load_style_property(*style_ip, key.as<std::string>(), value);
				}
			};
		});

	////	ui_table.set_function("color", [](const std::string_view color_str) -> sol::object {
	////		auto color = libv::parse::parse_color_optional(color_str);
	////		if (color)
	////			return *color;
	////		else
	////			return sol::nil;
	////	});
	//
	//	ui_table.set_function("image", [](const std::string_view) { return "Not implemented yet."; });
	//	ui_table.set_function("stretch", [](const std::string_view) { return "Not implemented yet."; });
	//	ui_table.set_function("layout_line", [](const std::string_view) { return "Not implemented yet."; });
	}

	// std::string key because key will be consumed by style.set anyways
	void load_style_property(Style& style, std::string key, const sol::object& value) {

		auto it = property_loaders.find(key);
		if (it == property_loaders.end())
			return log_ui.warn("Ignoring unrecognized property style {} property {} value \"{}\".",
					style.style_name, key, value.as<std::string_view>());

		const auto& loader = it->second;
		auto result = loader(ui, value);
		if (!result)
			return log_ui.warn("Failed to load style {} property {} value \"{}\"",
					style.style_name, key, value.as<std::string_view>());

		style.set(std::move(key), std::move(*result));
	}
};

void script_style(UI& ui, lua::State& lua, const std::string_view script_str) {
	LoadLuaContext context(ui, lua);

//	try {
		// TODO P1: Create a sandbox around the script execution
		const auto result = lua.safe_script(script_str, sol::script_pass_on_error);

		if (!result.valid()) {
			sol::error err = result;
			return log_ui.error("Script loading failed: {}", err.what());
		}

		if (result.get_type() != sol::type::none)
			log_ui.warn("Script return value is unused: {} - {}", libv::to_value(result.get_type()), std::string(result));

		log_ui.info("Script loading successful");

//	} catch (const std::exception& e) {
//		log_ui.error("Failed to load texture due to exception: {}", e.what());
//	}
}

// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------

//Component script_file(UI& ui, lua::State& lua, const std::filesystem::path& file) {
//	auto ui_table = lua.create_table();
//	ui_table["bottom"] = "bottom";
//	ui_table["center"] = "center";
//	ui_table["justify"] = "justify";
//	ui_table["justifyall"] = "justifyall";
//	ui_table["left"] = "left";
//	ui_table["right"] = "right";
//	ui_table["top"] = "top";
//
//	ui_table["ORIENT_BOTTOM_TO_TOP"] = Orientation::BOTTOM_TO_TOP;
//	ui_table["ORIENT_LEFT_TO_RIGHT"] = Orientation::LEFT_TO_RIGHT;
//	ui_table["ORIENT_RIGHT_TO_LEFT"] = Orientation::RIGHT_TO_LEFT;
//	ui_table["ORIENT_TOP_TO_BOTTOM"] = Orientation::TOP_TO_BOTTOM;
//
//	std::unordered_map<std::string, libv::intrusive_ptr<Style>> styles;
//
//	ui_table.set_function("style", [&styles, &ui](const std::string_view style_name) {
//		auto& style_ip = styles[std::string(style_name)]; // TODO P5: C++20: std::string(std::string_view) temp string for hash lookup
//		if (!style_ip)
//			style_ip = libv::make_intrusive<Style>(std::string(style_name));
//
//		return [style_ip, &ui](const sol::table& table) {
//			for (const auto& [key, value] : table) {
//				if (key.get_type() != sol::type::string) {
//					log_ui.warn("Style's property key \"{}\" is expected to be a string. Style name: {}, key: {}, value: {}",
//							key.as<std::string_view>(), style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());
//					continue;
//				}
//
//				const auto key_sv = key.as<std::string_view>();
//				process_style_property(ui, *style_ip, key_sv, value);
//			}
//		};
//	});
//
////	ui_table.set_function("component", [](const std::string_view component_name) {
////		return [component_name = std::string(component_name)](const sol::table& table) {
////			for (const auto& [key, value] : table) {
////				log_ui.info("{}/{} = {}", component_name, key.as<std::string_view>(), value.as<std::string_view>());
////			}
////		};
////	});
////
////	ui_table.set_function("color", [](const std::string_view color_str) -> sol::object {
////		auto color = libv::parse::parse_color_optional(color_str);
////		if (color)
////			return *color;
////		else
////			return sol::nil;
////	});
//
//	ui_table.set_function("component", [](const std::string_view) { return "Not implemented yet."; });
//	ui_table.set_function("image", [](const std::string_view) { return "Not implemented yet."; });
//	ui_table.set_function("stretch", [](const std::string_view) { return "Not implemented yet."; });
//	ui_table.set_function("layout_line", [](const std::string_view) { return "Not implemented yet."; });
//}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
