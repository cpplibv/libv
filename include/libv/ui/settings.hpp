// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/meta/reflection_access.hpp>
#include <libv/serialization/enable.hpp>
// std
#include <filesystem>
//#include <string>
#include <chrono>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ResourceSettings {
	std::filesystem::path base_path; /// Relative resource path base
	// bool relative_path_only = true; /// Forbid requests with absolute path
	// bool restict_under_base = true; /// Forbid requests that would leave the base path
	// bool cache_fallback = true; /// Insert failed resource lookups into cache as the fallback value
	// bool track_every = false; /// Track every resource and reload resource upon file change
	// std::unordered_set<std::filesystem::path> track; /// Track specific resource and reload resource upon file change

	LIBV_REFLECTION_ACCESS(base_path);

	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct Settings {
	std::chrono::milliseconds cursor_flash_period{700};
	std::chrono::milliseconds cursor_show_period{350};

	float cursor_width_max = 5;
	float cursor_width_min = 2;
	float cursor_width_offset = 12;
	float cursor_width_scale = 24;

	// bool enable_lcd_font = true;

	ResourceSettings res_font = {"res/font"};
	ResourceSettings res_shader = {"res/shader"};
	ResourceSettings res_texture = {"res/texture"};

	LIBV_REFLECTION_ACCESS(cursor_flash_period);
	LIBV_REFLECTION_ACCESS(cursor_show_period);
	LIBV_REFLECTION_ACCESS(cursor_width_max);
	LIBV_REFLECTION_ACCESS(cursor_width_min);
	LIBV_REFLECTION_ACCESS(cursor_width_offset);
	LIBV_REFLECTION_ACCESS(cursor_width_scale);
	// LIBV_REFLECTION_ACCESS(enable_lcd_font);
	LIBV_REFLECTION_ACCESS(res_font);
	LIBV_REFLECTION_ACCESS(res_shader);
	LIBV_REFLECTION_ACCESS(res_texture);

	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
