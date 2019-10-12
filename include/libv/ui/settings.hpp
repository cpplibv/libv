// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

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
};

struct Settings {
	std::chrono::milliseconds caret_flash_period{700};
	std::chrono::milliseconds caret_show_period{350};

	float caret_width_max = 5;
	float caret_width_min = 2;
	float caret_width_offset = 12;
	float caret_width_scale = 24;

	// bool enable_lcd_font = true;

	ResourceSettings res_font = {"res/font"};
	ResourceSettings res_shader = {"res/shader"};
	ResourceSettings res_texture = {"res/texture"};
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
