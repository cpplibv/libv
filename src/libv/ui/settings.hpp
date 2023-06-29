// Project: libv.ui, File: src/libv/ui/settings.hpp

#pragma once

// std
#include <chrono>
#include <functional>
#include <string>
#include <vector>
// pro
#include <libv/ui/chrono.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ResourceSettings {
	std::string base_path; /// Relative resource path base
	// bool relative_path_only = true; /// Forbid requests with absolute path
	 bool restrict_under_base = true; /// Forbid requests that would leave the base path
	// bool cache_fallback = true; /// Insert failed resource lookups into cache as the fallback value
	// bool track_every = false; /// Track every resource and reload resource upon file change
	// std::unordered_set<std::filesystem::path> track; /// Track specific resource and reload resource upon file change
};

struct Settings {
public:
	bool use_libv_res_resource_path = false; /// Migration path

public:
	ResourceSettings res_font = {"res/font/"};
	ResourceSettings res_shader = {"res/shader/"};
	ResourceSettings res_texture = {"res/texture/"};

public:
	// bool enable_lcd_font = true;
	std::chrono::milliseconds caret_flash_period{700};
	std::chrono::milliseconds caret_show_period{350};

	float caret_width_max = 5;
	float caret_width_min = 2;
	float caret_width_offset = 32;
	float caret_width_scale = 24;

public:
	double scroll_step_button = 3.0;
	double scroll_step_scroll = 3.0;
	double scroll_step_track = std::numeric_limits<double>::infinity();
	double scroll_hold_button = scroll_step_button * 10.0;
	double scroll_hold_scroll = scroll_step_scroll * 10.0;
	double scroll_hold_track = scroll_step_track * 10.0;

	bool track_style_scripts = false;
//	bool track_fonts = false;
//	bool track_shaders = false;
//	bool track_textures = false;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
