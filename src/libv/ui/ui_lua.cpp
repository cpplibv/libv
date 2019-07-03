// File:   ui.cpp Author: Vader Created on 2019. február 14., 4:55

// hpp
#include <libv/ui/ui_lua.hpp>
// libv
#include <libv/lua/lua.hpp>
#include <libv/lua/object_parser.hpp>
#include <libv/parse/bool.hpp>
#include <libv/parse/color.hpp>
// std
#include <memory>
#include <string>
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

// -------------------------------------------------------------------------------------------------

inline const auto pattern_color() noexcept {
	return libv::lua::one_of(
			libv::lua::string_parse(&libv::parse::parse_color_optional),
			libv::lua::as<Color>(libv::lua::userdata<libv::vec4f>()),
			libv::lua::transform(libv::lua::userdata<libv::vec3f>(), [](const libv::vec3f& vec) { return Color(vec, 1.f); }),
			libv::lua::table<Color>(
				libv::lua::member("r", libv::lua::number<float>()),
				libv::lua::member("g", libv::lua::number<float>()),
				libv::lua::member("b", libv::lua::number<float>()),
				libv::lua::member("a", libv::lua::or_(libv::lua::number<float>(), 1.f))),
			libv::lua::table<Color>(
				libv::lua::member("x", libv::lua::number<float>()),
				libv::lua::member("y", libv::lua::number<float>()),
				libv::lua::member("z", libv::lua::number<float>()),
				libv::lua::member("w", libv::lua::or_(libv::lua::number<float>(), 1.f))),
			libv::lua::table<Color>(
				libv::lua::member(1, libv::lua::number<float>()),
				libv::lua::member(2, libv::lua::number<float>()),
				libv::lua::member(3, libv::lua::number<float>()),
				libv::lua::member(4, libv::lua::or_(libv::lua::number<float>(), 1.f)))
	);
}

template <typename F>
inline const auto pattern_protocol(std::string protocol, F&& func) {
	auto parse_func = [protocol = std::move(protocol)](auto str) {
		if (not str.starts_with(protocol))
			return std::optional<std::string_view>(std::nullopt);

		str.remove_prefix(protocol.size());
		return std::optional<std::string_view>(str);
	};

	return libv::lua::transform(libv::lua::string_parse(std::move(parse_func)), std::forward<F>(func));
}

inline const auto pattern_background(UI& ui) noexcept {
//	return libv::lua::string<std::string>();
	return pattern_protocol("image ", [&ui](const auto path){ return ui.context().texture2D(path); });
//	return libv::lua::variant(
//			pattern_protocol("image ", [&ui](const auto path){ return ui.context().texture2D(path); }),
////			pattern_protocol("color ", ),
////			pattern_protocol("stretch ", ),
//			libv::lua::string_parse(&libv::parse::parse_color_optional)
//	);
}

void process_style_property(UI& ui, Style& style, const std::string_view key, const sol::object& value) {
	const auto property = [&](const std::string_view name, const auto& pattern) {
		if (name != key)
			return false;

		auto reporter = libv::lua::ReportNoop{};
		const auto result = libv::lua::parse(value, pattern, reporter);
		if (result) {
			style.set(std::string(name), *result);
		} else {
			// TODO P4: Detailed error message with libv.lua.object_parser reporter API
			log_ui.warn("Failed to parse style {} property {} value \"{}\". Property is set to fallback value. Reason: {}",
					style.style_name, key, value.as<std::string_view>(), reporter.reason());
		}

		return true;
	};

	if (property("align", libv::lua::string_parse(&libv::ui::parse_align_horizontal_optional))) return;
	if (property("align_vertical", libv::lua::string_parse(&libv::ui::parse_align_vertical_optional))) return;
	if (property("background", pattern_background(ui))) return;
	if (property("color", pattern_color())) return;
	if (property("font", libv::lua::string_accept([&ui](const auto path){ return ui.context().font(path); }))) return;
	if (property("font_color", pattern_color())) return;
	if (property("font_outline", pattern_color())) return;
	if (property("font_size", libv::lua::number<float>())) return;
	if (property("size", libv::lua::string_parse(&libv::ui::parse_size_optional))) return;
//	if (property("font_shader", libv::lua::string())) return;
//	if (property("layout", libv::lua::string_parse(&libv::ui::parse_layout_optional))) return;

	log_ui.warn("Ignoring unrecognized property style {} property {} value \"{}\".",
			style.style_name, key, value.as<std::string_view>());
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
				if (key.get_type() != sol::type::string) {
					log_ui.warn("Style's property key \"{}\" is expected to be a string. Style name: {}, key: {}, value: {}",
							key.as<std::string_view>(), style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());
					continue;
				}

				const auto key_sv = key.as<std::string_view>();
				process_style_property(ui, *style_ip, key_sv, value);
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
