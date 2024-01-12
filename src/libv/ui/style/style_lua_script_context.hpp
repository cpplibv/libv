// Project: libv.lua, File: src/libv/ui/lua/script_style.hpp

#pragma once

#include <sol/forward.hpp>

#include <libv/ui/fwd.hpp>
#include <libv/ui/property_system/property.hpp>
#include <libv/ui/style/style_fwd.hpp>
#include <libv/ui/style/style_state.hpp>
#include <libv/utility/hash_string.hpp>
#include <libv/utility/source_location.hpp>

#include <optional>
#include <string>
#include <unordered_map>



namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class StyleLuaScriptContext {
public:
	using OnErrorFn = std::function<void(const std::string_view&)>;
	using load_fn = std::function<std::optional<PropertyDynamic>(UI&, const sol::object&)>;

private:
	std::unordered_map<std::string, load_fn, libv::hash_string, std::equal_to<>> property_loaders;
	std::unordered_map<std::string, std::string, libv::hash_string, std::equal_to<>> property_aliases;

	UI& ui;

	StyleState current_state_mask = StyleState::none;
	StyleState current_state_value = StyleState::none;

private:
	OnErrorFn onError;

public:
	explicit StyleLuaScriptContext(UI& ui, OnErrorFn onError);
	void bind(sol::state& lua);

private:
	void load_style_property(Style& style, const std::string_view& lookup_key, const sol::object& value);
	void process_state_mask_and_value(Style& style, std::string_view state_str);
	void load_style_table(Style& style, const sol::table& table);

	template <typename... Args>
	void error(const libv::with_source_location<const std::string_view> fmt, const Args&... args);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
