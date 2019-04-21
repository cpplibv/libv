// File:   ui.cpp Author: Vader Created on 2019. február 14., 4:55

// hpp
#include <libv/ui/ui_lua.hpp>
// libv
#include <libv/lua/lua.hpp>
#include <libv/parse/bool.hpp>
#include <libv/parse/color.hpp>
// std
#include <memory>
#include <string_view>
// pro
#include <libv/ui/context_ui.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/parse/parse_align.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/ui.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ComponentBase;
class UI;

namespace {

////template <typename T, typename F>
////auto parse(F&& parser) {
////	return [parser_i = std::forward<F>(parser)](Style& style, const std::string_view& key, const sol::object& value) {
////		if (!value.is<T>())
////			return false;
//////		log_ui.warn("Type match {}", value.as<std::string_view>());
////
////		auto result = std::forward<decltype(parser_i)>(parser_i)(value.as<T>());
////		if (!result)
////			return false;
//////		log_ui.warn("Parsed {}", value.as<std::string_view>());
////
////		style.set(std::string(key), *result);
////		return true;
////	};
////}
////
////template <typename T, typename F>
////auto accept_as(F&& parser) {
////	return [parser_i = std::forward<F>(parser)](Style& style, const std::string_view& key, const sol::object& value) {
////		if (!value.is<T>())
////			return false;
//////		log_ui.warn("Type match {}", value.as<std::string_view>());
////
////		auto result = std::forward<decltype(parser_i)>(parser_i)(value.as<T>());
//////		log_ui.warn("Parsed {}", value.as<std::string_view>());
////
////		style.set(std::string(key), result);
////		return true;
////	};
////}
////
////template <typename LuaT, typename CppT = LuaT>
////auto accept() {
////	return [](Style& style, const std::string_view& key, const sol::object& value) {
////		if (!value.is<LuaT>())
////			return false;
//////		log_ui.warn("Type match");
////
//////		log_ui.warn("Accepted {}", value.as<std::string_view>());
////		if constexpr (std::is_same_v<LuaT, CppT>)
////			style.set(std::string(key), value.as<LuaT>());
////		else
////			style.set(std::string(key), CppT{value.as<LuaT>()});
////
////		return true;
////	};
////}
////
////bool property(const auto& key, const auto& style, const auto& value, const auto& name, auto&&... acceptors) {
////	if (name != key)
////		return false;
////
////	return (std::forward<decltype(acceptors)>(acceptors)(*style, key, value) || ...);
//}

template <typename CppT, sol::type LuaType>
struct LuaIndexSV {
	using type = CppT;
	static const sol::type lua_type = LuaType;
	std::string_view index;
};

template <typename CppT, sol::type LuaType>
struct LuaIndexI {
	using type = CppT;
	static const sol::type lua_type = LuaType;
	int index;
};

//template <typename CppT, sol::type LuaType>
//auto lua_arg(const std::string_view key) {
//	return LuaIndexSV<CppT, LuaType>{key};
//}
//
//template <typename CppT, sol::type LuaType>
//auto lua_arg(const int key) {{
//	return LuaIndexI<CppT, LuaType>{key};
//}
//
//template <typename CppT, typename F>
//bool parse_string(F&& func) {
//}
//
//template <typename CppT, typename... Args>
//bool accept_table(Args&&... args) {
//}
//
//template <typename CppT, typename LuaT>
//bool accept_usertype() {
//}
//
//template <typename F>
//bool transform_string(F&& func) {
//}

// -------------------------------------------------------------------------------------------------

//struct LuaObjectParser {
//	Style& style;
//	const sol::object& var;
//	const std::string_view key;
//
//	bool number() {
//	}
//
//	bool string() {
//		if (value.get_type() != sol::type::string)
//			return false;
//		return true;
//	}
//
//	bool usertype() {
//	}
//
//	template <typename CppT, typename... Args>
//	bool table(Args&&... args) {
//		if (value.get_type() != sol::type::table)
//			return false;
//
//
//
//		[&]() {
//		}(args)...;
//		(typename decltype(args)::type(args::lua_type, args.index), ...);
//
//
//		return true;
//	}
//
//	void result() {
//	}
//};

// -------------------------------------------------------------------------------------------------

bool process_style_property(UI& ui, Style& style, const std::string_view key, const sol::object& value) {
//	LuaObjectParser parser(style, key, value);
//
//	if (key == "color") {
//		parser.string<Color>(libv::parse::parse_color_optional) ||
//		parser.usertype<Color, libv::vec4f>() ||
//		parser.table<Color>(
//				parser.index(
//					parser.def_arg<float, sol::type::number>("r"),
//					parser.def_arg<float, sol::type::number>("g"),
//					parser.def_arg<float, sol::type::number>("b"),
//					parser.def_arg<float, sol::type::number>("a")),
//				parser.index(
//					parser.def_arg<float, sol::type::number>("x"),
//					parser.def_arg<float, sol::type::number>("y"),
//					parser.def_arg<float, sol::type::number>("z"),
//					parser.def_arg<float, sol::type::number>("w")),
//				parser.index(
//					parser.def_arg<float, sol::type::number>(1),
//					parser.def_arg<float, sol::type::number>(2),
//					parser.def_arg<float, sol::type::number>(3),
//					parser.def_arg<float, sol::type::number>(4))
//		);
//	}

//	if (property(key_sv, style_ip, value, "color",
//			parse<std::string_view>(libv::parse::parse_color_optional),
//			accept<libv::vec4f>()))
//		continue;
//
//	if (property(key_sv, style_ip, value, "font_size",
//			accept<FontSize>()))
//		continue;
//
//	if (property(key_sv, style_ip, value, "font_color",
//			parse<std::string_view>(libv::parse::parse_color_optional),
//			accept<libv::vec4f>()))
//		continue;
//
//	if (property(key_sv, style_ip, value, "size",
//			parse<std::string_view>(libv::ui::parse_size_optional)))
//		continue;
//
//	if (property(key_sv, style_ip, value, "align",
//			parse<std::string_view>(libv::ui::parse_align_horizontal_optional)))
//		continue;
//
//	if (property(key_sv, style_ip, value, "align_vertical",
//			parse<std::string_view>(libv::ui::parse_align_vertical_optional)))
//		continue;
//
//	if (property(key_sv, style_ip, value, "font",
//			accept_as<std::string_view>([&ui](auto path){ return ui.context().font(path); })))
//		continue;



//				if (property(key_sv, style_ip, value, "background",
//						accept_as<std::string_view>([&ui](auto path){ return ui.context().font(path); })))
//					continue;

//						|| property("background", accept<int>())
//						|| property("font_shader", accept<std::string_view>())
//						|| property("layout", accept<std::string_view>())
//						|| property("layout", accept<int>())

//				log_ui.warn("Style size property value \"{}\" cannot be parsed. Style name: {}, key: {}, value: {}",
//						value.as<std::string_view>(), style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());
//				log_ui.warn("Style size property value has invalid type. Style name: {}, key: {}, value: {}, type: {}",
//						key.as<std::string_view>(), style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());
//				log_ui.info("{}/{} = {}", style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());

	return false;
}

} // namespace

// -------------------------------------------------------------------------------------------------

std::shared_ptr<ComponentBase> script_file(UI& ui, lua::State& lua, const std::filesystem::path& file) {

	auto ui_table = lua.create_table();
	ui_table["bottom"] = "bottom";
	ui_table["center"] = "center";
	ui_table["justify"] = "justify";
	ui_table["justifyall"] = "justifyall";
	ui_table["left"] = "left";
	ui_table["right"] = "right";
	ui_table["top"] = "top";

	ui_table["ORIENT_BOTTOM_TO_TOP"] = Orientation2::BOTTOM_TO_TOP;
	ui_table["ORIENT_LEFT_TO_RIGHT"] = Orientation2::LEFT_TO_RIGHT;
	ui_table["ORIENT_RIGHT_TO_LEFT"] = Orientation2::RIGHT_TO_LEFT;
	ui_table["ORIENT_TOP_TO_BOTTOM"] = Orientation2::TOP_TO_BOTTOM;

	std::unordered_map<std::string, libv::intrusive_ptr<Style>> styles;

	ui_table.set_function("style", [&styles, &ui](const std::string_view style_name) {
		auto& style_ip = styles[std::string(style_name)]; // TODO P5: C++20: std::string(std::string_view) temp string for hash lookup
		if (!style_ip)
			style_ip = libv::make_intrusive<Style>(std::string(style_name));

		return [style_ip, &ui](const sol::table& table) {
			for (const auto& [key, value] : table) {
				if (value.get_type() != sol::type::string) {
					log_ui.warn("Style's property key \"{}\" is expected to be a string. Style name: {}, key: {}, value: {}",
							key.as<std::string_view>(), style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());
					continue;
				}

				const auto key_sv = key.as<std::string_view>();
				const auto accepted = process_style_property(ui, *style_ip, key_sv, value);

				log_ui.warn_if(!accepted, "Style property key \"{}\" is unrecognized. Style name: {}, key: {}, value: {}",
						key.as<std::string_view>(), style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());
			}
		};
	});

//	ui_table.set_function("component", [](const std::string_view component_name) {
//		return [component_name = std::string(component_name)](const sol::table& table) {
//			for (const auto& [key, value] : table) {
//				log_ui.info("{}/{} = {}", component_name, key.as<std::string_view>(), value.as<std::string_view>());
//			}
//		};
//	});
//
//	ui_table.set_function("color", [](const std::string_view color_str) -> sol::object {
//		auto color = libv::parse::parse_color_optional(color_str);
//		if (color)
//			return *color;
//		else
//			return sol::nil;
//	});

	ui_table.set_function("component", [](const std::string_view) { return "Not implemented yet."; });
	ui_table.set_function("image", [](const std::string_view) { return "Not implemented yet."; });
	ui_table.set_function("stretch", [](const std::string_view) { return "Not implemented yet."; });
	ui_table.set_function("layout_line", [](const std::string_view) { return "Not implemented yet."; });

	lua["ui"] = ui_table;
	const auto table = lua.script_file(file.string());
//	const auto table = lua.safe_script_file(file.string());

	(void) ui;



	// -------------------------------------------------------------------------------------------------

	log_ui.trace("Parsed styles:");
	for (const auto& [name, style] : styles)
		style->foreach([&name](const auto& key, const auto& value) {
			std::visit([&](const auto& var) {
				log_ui.trace("{}/{} = {}", name, key, var);
			}, value);
		});


//	std::shared_ptr<ComponentBase>
	return nullptr;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
