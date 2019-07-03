// File:   context_ui.cpp Author: Vader Created on 2019. február 9., 13:01

// hpp
#include <libv/ui/context_ui.hpp>
// libv
#include <libv/gl/image.hpp>
#include <libv/utility/is_parent_folder_of.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// pro
#include <libv/ui/font_2D.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/raw_font_consolas_min.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

bool secure_path(const std::filesystem::path& base, const std::filesystem::path& target, const std::filesystem::path& path) {
	std::error_code ec;
	const auto canonical = std::filesystem::canonical(target, ec);
	if (ec) {
		log_ui.error("Failed to determine canonical target path: {} {}"
				"\n\tPath:      {}"
				"\n\tTarget:    {}"
				"\n\tBase:      {}", ec, ec.message(), path, target, base);
		return false;
	}

	const auto is_parent = libv::is_parent_folder_of(path, canonical);
	if (!is_parent) {
		log_ui.error("Canonical path is expected to be under the base folder:"
				"\n\tPath:      {}"
				"\n\tCanonical: {}"
				"\n\tBase:      {}", path, canonical, base);
		return false;
	}

	const auto relative = std::filesystem::relative(canonical, base, ec);
	if (ec) {
		log_ui.error("Failed to determine relative path: {} {}"
				"\n\tPath:      {}"
				"\n\tCanonical: {}"
				"\n\tBase:      {}", ec, ec.message(), path, canonical, base);
		return false;
	}

	if (relative != path) {
		log_ui.error("Canonical relative path does not matches path"
				"\n\tPath:      {}"
				"\n\tCanonical: {}"
				"\n\tRelative:  {}"
				"\n\tBase:      {}", path, canonical, relative, base);
		return false;
	}

	return true;
}

// -------------------------------------------------------------------------------------------------

ContextUI::ContextUI(
		const std::filesystem::path& path_base,
		const std::filesystem::path& path_fonts,
		const std::filesystem::path& path_shaders,
		const std::filesystem::path& path_textures) {

	std::error_code ec;
	auto res = std::filesystem::canonical(path_base, ec);
	log_ui.info_if(!ec, "Using resource folder: {}", res);
	log_ui.error_if(ec, "Failed to determining resource folder for {}: {} {}.", path_base, ec, ec.message());

	this->path_fonts = std::filesystem::canonical(res / path_fonts, ec);
	log_ui.info_if(!ec, "Using font resource folder: {}", this->path_fonts);
	log_ui.error_if(ec, "Failed to determining font folder for {}: {} {}.", res / path_fonts, ec, ec.message());

	this->path_shaders = std::filesystem::canonical(res / path_shaders, ec);
	log_ui.info_if(!ec, "Using shader resource folder: {}", this->path_shaders);
	log_ui.error_if(ec, "Failed to determining shader folder for {}: {} {}.", res / path_shaders, ec, ec.message());

	this->path_textures = std::filesystem::canonical(res / path_textures, ec);
	log_ui.info_if(!ec, "Using texture resource folder: {}", this->path_textures);
	log_ui.error_if(ec, "Failed to determining texture folder for {}: {} {}.", res / path_textures, ec, ec.message());

	const auto fallback_font_data = raw_font_consolas_min();
	fallback_font = std::make_shared<Font2D>(
			std::string{reinterpret_cast<const char*>(fallback_font_data.first), fallback_font_data.second}
	);
}

ContextUI::~ContextUI() {
}

std::shared_ptr<Font2D> ContextUI::font(const std::filesystem::path& path) {
	std::shared_ptr<Font2D> sp;

	const auto lexically_normal = path.lexically_normal();
//	auto key = lexically_normal.generic_u8string();
	auto key = lexically_normal.generic_string();

	const auto it = cache_font.find(key);
	if (it != cache_font.end()) {
		sp = it->second.lock();
		if (sp) {
			// Cache hit
			log_ui.trace("Font cache hit for: {} as {}", path, key);
			return sp;

		} else {
			// Cache expired
			log_ui.trace("Font cache expired for: {} as {}", path, key);
			cache_font.erase(it);
		}
	} else {
		// Cache miss
		log_ui.trace("Font cache miss for: {} as {}", path, key);
	}

	libv::Timer timer;

	const auto target = path_fonts / lexically_normal;

	if (!secure_path(path_fonts, target, lexically_normal)) {
		log_ui.error("Path validation failed. Using fallback font");
		sp = fallback_font;
		return sp;
	}

	const auto time_scan = timer.timef_ms();

	auto file = libv::read_file_ec(target);
	if (file.ec) {
		log_ui.error("Failed to read font file: {}: {} {}. Using fallback font", target, file.ec, file.ec.message());
		sp = fallback_font;
		return sp;
	}

	const auto time_read = timer.timef_ms();

	sp = std::make_shared<Font2D>(std::move(file.data));

	const auto time_load = timer.timef_ms();
	log_ui.trace("Font loaded: scan:{:5.1f}ms, read:{:5.1f}ms, load:{:5.1f}ms for {} as {}",
			time_scan.count(), time_read.count(), time_load.count(), path, key);

	cache_font.emplace(std::move(key), sp);
	return sp;
}

std::shared_ptr<Texture2D> ContextUI::texture2D(const std::filesystem::path& path) {
	std::shared_ptr<Texture2D> sp;

	const auto lexically_normal = path.lexically_normal();
//	auto key = lexically_normal.generic_u8string();
	auto key = lexically_normal.generic_string();

	const auto it = cache_texture2D.find(key);
	if (it != cache_texture2D.end()) {
		sp = it->second.lock();
		if (sp) {
			// Cache hit
			log_ui.trace("Texture2D cache hit for: {} as {}", path, key);
			return sp;

		} else {
			// Cache expired
			log_ui.trace("Texture2D cache expired for: {} as {}", path, key);
			cache_texture2D.erase(it);
		}
	} else {
		// Cache miss
		log_ui.trace("Texture2D cache miss for: {} as {}", path, key);
	}

	libv::Timer timer;

	const auto target = path_textures / lexically_normal;

	if (!secure_path(path_textures, target, lexically_normal)) {
		log_ui.error("Path validation failed. Using fallback texture2D");
		sp = fallback_texture2D;
		return sp;
	}

	const auto time_scan = timer.timef_ms();

	auto file = libv::read_file_ec(target);
	if (file.ec) {
		log_ui.error("Failed to read texture2D file: {}: {} {}. Using fallback texture2D", target, file.ec, file.ec.message());
		sp = fallback_texture2D;
		return sp;
	}

	const auto time_read = timer.timef_ms();

	auto image = libv::gl::load_image(file.data);
	if (!image) {
		log_ui.error("Failed to load texture2D file: {}. Using fallback texture2D", target);
		sp = fallback_texture2D;
		return sp;
	}

	sp = std::make_shared<Texture2D>(std::move(*image));

	const auto time_load = timer.timef_ms();
	log_ui.trace("Texture2D loaded: scan:{:5.1f}ms, read:{:5.1f}ms, create:{:5.1f}ms for {} as {}",
			time_scan.count(), time_read.count(), time_load.count(), path, key);

	cache_texture2D.emplace(std::move(key), sp);
	return sp;
}

libv::intrusive_ptr<Style> ContextUI::style(const std::string_view style_name) {
	// TODO P5: std::string(string_view) for hash lookup, I know there is or there will be a solution for it
	const auto it = styles.find(std::string(style_name));
	if (it == styles.end()) {
		// TODO P5: std::string(string_view) for hash lookup, I know there is or there will be a solution for it
//		const auto result = styles.emplace(std::string(style_name), libv::make_intrusive<Style>(std::string(style_name))).first->second;
//		return result;
		return styles.emplace(std::string(style_name), libv::make_intrusive<Style>(std::string(style_name))).first->second;

	} else {
		return it->second;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
