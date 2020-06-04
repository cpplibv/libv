// File:   context_ui.cpp Author: Vader Created on 2019. február 9., 13:01

// hpp
#include <libv/ui/context_ui.hpp>
// ext
#include <clip/clip.h>
// libv
#include <libv/gl/image.hpp>
#include <libv/utility/generic_path.hpp>
#include <libv/utility/is_parent_folder_of.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
// pro
#include <libv/ui/context_event.hpp>
#include <libv/ui/context_mouse.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/raw/font_consolas_min.hpp>
#include <libv/ui/raw/texture2D_white256.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/shader/shader_quad.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>
#include <libv/ui/ui.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ImplContextUI {
	ContextEvent event;
	ContextMouse mouse;

	std::shared_ptr<Font2D> fallback_font;
	std::unordered_map<std::string, std::weak_ptr<Font2D>> cache_font;

	std::shared_ptr<Texture2D> fallback_texture2D;
	std::unordered_map<std::string, std::weak_ptr<Texture2D>> cache_texture2D;

	std::unordered_map<std::string, libv::intrusive_ptr<Style>> styles;

//	std::unordered_map<std::string, std::weak_ptr<Shader>> cache_shader;
//	std::unordered_map<TypeInfoRef, std::weak_ptr<Shader>, TypeInfoRefHasher, TypeInfoRefEqualTo> cache_typed_shader;

	std::weak_ptr<ShaderFont> shader_font;
	std::weak_ptr<ShaderImage> shader_image;
	std::weak_ptr<ShaderQuad> shader_quad;

public:
	template <typename T>
	std::shared_ptr<T> getShader(std::weak_ptr<T>& wp, const std::string_view name) {
		auto sp = wp.lock();
		if (sp) {
			log_ui.trace("Shader cache hit for: {}", name);
		} else {
			log_ui.trace("Shader cache miss for: {}", name);
			wp = sp = std::make_shared<T>();
		}
		return sp;
	}
};

// -------------------------------------------------------------------------------------------------

bool secure_path(const std::filesystem::path& base, const std::filesystem::path& target, const std::filesystem::path& path) {
	std::error_code ec;

	// TODO P1: Nicer file not found error (it will be detected with the canonical call)
	const auto canonical = std::filesystem::canonical(target, ec);
	if (ec) {
		log_ui.error("Failed to determine canonical target path: {} {}"
				"\n\tPath:      {}"
				"\n\tTarget:    {}"
				"\n\tBase:      {}", ec, ec.message(), libv::generic_path(path), libv::generic_path(target), libv::generic_path(base));
		return false;
	}

	const auto relative_to_current = std::filesystem::relative(canonical, ec);
	if (ec) {
		log_ui.error("Failed to determine relative target path: {} {}"
				"\n\tPath:      {}"
				"\n\tTarget:    {}"
				"\n\tBase:      {}", ec, ec.message(), libv::generic_path(path), libv::generic_path(target), libv::generic_path(base));
		return false;
	}

	const auto is_parent = libv::is_parent_folder_of(base, relative_to_current);
	if (!is_parent) {
		log_ui.error("Canonical path is expected to be under the base folder:"
				"\n\tPath:      {}"
				"\n\tCanonical: {}"
				"\n\tBase:      {}", libv::generic_path(path), libv::generic_path(relative_to_current), libv::generic_path(base));
		return false;
	}

	const auto relative_to_base = std::filesystem::relative(relative_to_current, base, ec);
	if (ec) {
		log_ui.error("Failed to determine relative path: {} {}"
				"\n\tPath:      {}"
				"\n\tCanonical: {}"
				"\n\tBase:      {}", ec, ec.message(), libv::generic_path(path), libv::generic_path(relative_to_current), libv::generic_path(base));
		return false;
	}

	if (relative_to_base != path) {
		log_ui.error("Canonical relative path does not matches path"
				"\n\tPath:      {}"
				"\n\tCanonical: {}"
				"\n\tRelative:  {}"
				"\n\tBase:      {}", libv::generic_path(path), libv::generic_path(relative_to_current), libv::generic_path(relative_to_base), libv::generic_path(base));
		return false;
	}

	return true;
}

// -------------------------------------------------------------------------------------------------

ContextUI::ContextUI(UI& ui, ContextState& state, Settings settings) :
	self(std::make_unique<ImplContextUI>()),
	ui(ui),
	event(self->event),
	state(state),
	mouse(self->mouse),
	settings(std::move(settings)) {

	log_ui.info("Resource base font:    {}", libv::generic_path(this->settings.res_font.base_path));
	log_ui.info("Resource base shader:  {}", libv::generic_path(this->settings.res_shader.base_path));
	log_ui.info("Resource base texture: {}", libv::generic_path(this->settings.res_texture.base_path));

	const auto fallback_font_data = raw_font_consolas_min();
	self->fallback_font = std::make_shared<Font2D>(
			std::string{reinterpret_cast<const char*>(fallback_font_data.first), fallback_font_data.second}
	);

	const auto fallback_texture2D_data = raw_texture2D_white256();
	self->fallback_texture2D = std::make_shared<Texture2D>(libv::gl::load_image_or_throw(
			{reinterpret_cast<const char*>(fallback_texture2D_data.first), fallback_texture2D_data.second})
	);
}

ContextUI::~ContextUI() {
	// For forward declared unique_ptr
}

// -------------------------------------------------------------------------------------------------

std::string ContextUI::clipboardText() {
	std::string result;
	bool success = clip::get_text(result);
	log_ui.error_if(!success, "Failed to get clipboard");

	std::erase(result, '\r'); // NOTE: Cleanup any crlf line ending by removing every cr

	return result;
}

void ContextUI::clipboardText(const std::string& string) {
	bool success = clip::set_text(string);
	log_ui.error_if(!success, "Failed to set clipboard text of length {}: \"{}\"", string.size(), string);
}

// -------------------------------------------------------------------------------------------------

bool ContextUI::isAnyStyleDirty() const noexcept {
	for (const auto& [_, style_] : self->styles)
		if (style_->isDirty())
			return true;

	return false;
}

void ContextUI::clearEveryStyleDirty() noexcept {
	for (const auto& [_, style_] : self->styles)
		style_->clearDirty();
}

// -------------------------------------------------------------------------------------------------

void ContextUI::focus(BaseComponent& component) {
	ui.focus(component);
}

void ContextUI::detachFocused(BaseComponent& component) {
	ui.detachFocused(component);
}

void ContextUI::detachFocusLinked(BaseComponent& component) {
	ui.detachFocusLinked(component);
}

// -------------------------------------------------------------------------------------------------

std::shared_ptr<Font2D> ContextUI::font(const std::filesystem::path& path) {
	std::shared_ptr<Font2D> sp;

	const auto lexically_normal = path.lexically_normal();
	auto key = libv::generic_path(lexically_normal);

	const auto it = self->cache_font.find(key);
	if (it != self->cache_font.end()) {
		sp = it->second.lock();
		if (sp) {
			// Cache hit
			log_ui.trace("Font cache hit for: {} as {}", libv::generic_path(path), key);
			return sp;

		} else {
			// Cache expired
			log_ui.trace("Font cache expired for: {} as {}", libv::generic_path(path), key);
			self->cache_font.erase(it);
		}
	} else {
		// Cache miss
		log_ui.trace("Font cache miss for: {} as {}", libv::generic_path(path), key);
	}

	libv::Timer timer;

	const auto target = settings.res_font.base_path / lexically_normal;

	if (!secure_path(settings.res_font.base_path, target, lexically_normal)) {
		log_ui.error("Path validation failed. Using fallback font");
		sp = self->fallback_font;
		return sp;
	}

	const auto time_scan = timer.timef_ms();

	auto file = libv::read_file_ec(target);
	if (file.ec) {
		log_ui.error("Failed to read font file: {}: {} {}. Using fallback font", libv::generic_path(target), file.ec, file.ec.message());
		sp = self->fallback_font;
		return sp;
	}

	const auto time_read = timer.timef_ms();

	sp = std::make_shared<Font2D>(std::move(file.data));

	const auto time_load = timer.timef_ms();
	log_ui.trace("Font loaded: scan:{:5.1f}ms, read:{:5.1f}ms, load:{:5.1f}ms for {} as {}",
			time_scan.count(), time_read.count(), time_load.count(), libv::generic_path(path), key);

	self->cache_font.emplace(std::move(key), sp);
	return sp;
}

std::shared_ptr<Texture2D> ContextUI::texture2D(const std::filesystem::path& path) {
	std::shared_ptr<Texture2D> sp;

	const auto lexically_normal = path.lexically_normal();
//	auto key = lexically_normal.generic_u8string();
	auto key = libv::generic_path(lexically_normal);

	const auto it = self->cache_texture2D.find(key);
	if (it != self->cache_texture2D.end()) {
		sp = it->second.lock();
		if (sp) {
			// Cache hit
			log_ui.trace("Texture2D cache hit for: {} as {}", libv::generic_path(path), key);
			return sp;

		} else {
			// Cache expired
			log_ui.trace("Texture2D cache expired for: {} as {}", libv::generic_path(path), key);
			self->cache_texture2D.erase(it);
		}
	} else {
		// Cache miss
		log_ui.trace("Texture2D cache miss for: {} as {}", libv::generic_path(path), key);
	}

	libv::Timer timer;

	const auto target = settings.res_texture.base_path / lexically_normal;

	if (!secure_path(settings.res_texture.base_path, target, lexically_normal)) {
		log_ui.error("Path validation failed. Using fallback texture2D");
		sp = self->fallback_texture2D;
		return sp;
	}

	const auto time_scan = timer.timef_ms();

	auto file = libv::read_file_ec(target);
	if (file.ec) {
		log_ui.error("Failed to read texture2D file: {}: {} {}. Using fallback texture2D", libv::generic_path(target), file.ec, file.ec.message());
		sp = self->fallback_texture2D;
		return sp;
	}

	const auto time_read = timer.timef_ms();

	auto image = libv::gl::load_image(file.data);
	if (!image) {
		log_ui.error("Failed to load texture2D file: {}. Using fallback texture2D", libv::generic_path(target));
		sp = self->fallback_texture2D;
		return sp;
	}

	sp = std::make_shared<Texture2D>(std::move(*image));

	const auto time_load = timer.timef_ms();
	log_ui.trace("Texture2D loaded: scan:{:5.1f}ms, read:{:5.1f}ms, create:{:5.1f}ms for {} as {}",
			time_scan.count(), time_read.count(), time_load.count(), libv::generic_path(path), key);

	self->cache_texture2D.emplace(std::move(key), sp);
	return sp;
}

libv::intrusive_ptr<Style> ContextUI::style(const std::string_view style_name) {
	// TODO P5: std::string(string_view) for hash lookup, I know there is or there will be a solution for it
	const auto it = self->styles.find(std::string(style_name));
	if (it != self->styles.end())
		return it->second;

	const auto result = self->styles.emplace(style_name, libv::make_intrusive<Style>(std::string(style_name))).first->second;
	const auto lastDot = style_name.rfind('.');

	if (lastDot != style_name.npos)
		// Auto inherit based on dot naming hierarchy
		result->inherit(style(style_name.substr(0, lastDot)));

	log_ui.trace("Created style {}", style_name);

	return result;
}

// -------------------------------------------------------------------------------------------------

//std::shared_ptr<Shader> ContextUI::shader(const std::string& name) {
//	// Not implemented yet.
//}
//
//std::shared_ptr<ShaderFont> ContextUI::shaderFont(const std::string_view name) {
//	// Not implemented yet.
//}
//
//std::shared_ptr<ShaderImage> ContextUI::shaderImage(const std::string_view name) {
//	// Not implemented yet.
//}
//
//std::shared_ptr<ShaderQuad> ContextUI::shaderQuad(const std::string_view name) {
//	// Not implemented yet.
//}

// -------------------------------------------------------------------------------------------------

std::shared_ptr<ShaderFont> ContextUI::shaderFont() {
	return self->getShader(self->shader_font, "ShaderFont");
}

std::shared_ptr<ShaderImage> ContextUI::shaderImage() {
	return self->getShader(self->shader_image, "ShaderImage");
}

std::shared_ptr<ShaderQuad> ContextUI::shaderQuad() {
	return self->getShader(self->shader_quad, "ShaderQuad");
}

// -------------------------------------------------------------------------------------------------

std::shared_ptr<Font2D> ContextUI::fallbackFont() const {
	return self->fallback_font;
}

std::shared_ptr<Texture2D> ContextUI::fallbackTexture2D() const {
	return self->fallback_texture2D;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
