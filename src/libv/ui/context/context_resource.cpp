// Project: libv.ui, File: src/libv/ui/context/context_resource.cpp

// hpp
#include <libv/ui/context/context_resource.hpp>
// libv
#include <libv/gl/load_image.hpp>
#include <libv/utility/generic_path.hpp>
#include <libv/utility/is_parent_folder_of.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/resource/font_2D.hpp>
#include <libv/ui/resource/raw/font_consolas_min.hpp>
#include <libv/ui/resource/raw/texture2D_white256.hpp>
#include <libv/ui/resource/shader_font.hpp>
#include <libv/ui/resource/shader_image.hpp>
#include <libv/ui/resource/shader_quad.hpp>
#include <libv/ui/resource/texture_2D.hpp>
#include <libv/ui/settings.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextResource {
public:
	Settings& settings;

	std::shared_ptr<Font2D> fallback_font;
	std::unordered_map<std::string, std::weak_ptr<Font2D>> cache_font;

	std::shared_ptr<Texture2D> fallback_texture2D;
	std::unordered_map<std::string, std::weak_ptr<Texture2D>> cache_texture2D;

//	std::unordered_map<std::string, std::weak_ptr<Shader>> cache_shader;
//	std::unordered_map<TypeInfoRef, std::weak_ptr<Shader>, TypeInfoRefHasher, TypeInfoRefEqualTo> cache_typed_shader;

	std::shared_ptr<ShaderFont> shader_font;
	std::shared_ptr<ShaderImage> shader_image;
	std::shared_ptr<ShaderQuad> shader_quad;

public:
	explicit ImplContextResource(Settings& settings) :
		settings(settings) {}

public:
	template <typename T>
//	std::shared_ptr<T> getShader(std::weak_ptr<T>& wp, const std::string_view name) {
//		auto sp = wp.lock();
	std::shared_ptr<T> getShader(std::shared_ptr<T>& wp, const std::string_view name) {
		auto sp = wp;
		if (sp) {
//			log_ui.trace("Shader cache hit for: {}", name);
		} else {
			log_ui.trace("Shader cache miss for: {}", name);
			sp = std::make_shared<T>();
			wp = sp;
		}
		return sp;
	}
};

// -------------------------------------------------------------------------------------------------

bool secure_path(const std::filesystem::path& base, bool restict_under_base, const std::filesystem::path& target, const std::filesystem::path& path) {
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

	const auto is_parent = !restict_under_base || libv::is_parent_folder_of(base, relative_to_current);
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

ContextResource::ContextResource(Settings& settings) :
	self(std::make_unique<ImplContextResource>(settings)) {

	log_ui.info("Working directory:     {}", libv::generic_path(std::filesystem::current_path()));
	log_ui.info("Resource base font:    {}", libv::generic_path(self->settings.res_font.base_path));
	log_ui.info("Resource base shader:  {}", libv::generic_path(self->settings.res_shader.base_path));
	log_ui.info("Resource base texture: {}", libv::generic_path(self->settings.res_texture.base_path));

	const auto fallback_font_data = raw_font_consolas_min();
	self->fallback_font = std::make_shared<Font2D>(
			std::string{reinterpret_cast<const char*>(fallback_font_data.first), fallback_font_data.second}
	);

	const auto fallback_texture2D_data = raw_texture2D_white256();
	self->fallback_texture2D = std::make_shared<Texture2D>(libv::gl::load_image_or_throw(
			{reinterpret_cast<const char*>(fallback_texture2D_data.first), fallback_texture2D_data.second})
	);
}

ContextResource::~ContextResource() {
	// For the sake of forward declared ptr
}

// -------------------------------------------------------------------------------------------------

std::shared_ptr<Font2D> ContextResource::font(const std::filesystem::path& path) {
	std::shared_ptr<Font2D> sp;

	const auto lexically_normal = path.lexically_normal();
	auto key = libv::generic_path(lexically_normal);

	const auto it = self->cache_font.find(key);
	if (it != self->cache_font.end()) {
		sp = it->second.lock();
		if (sp) {
			// Cache hit
//			log_ui.trace("Font cache hit for: {} as {}", libv::generic_path(path), key);
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

	const auto target = self->settings.res_font.base_path / lexically_normal;

	if (!secure_path(self->settings.res_font.base_path, self->settings.res_font.restict_under_base, target, lexically_normal)) {
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

std::shared_ptr<Texture2D> ContextResource::texture2D(const std::filesystem::path& path) {
	std::shared_ptr<Texture2D> sp;

	const auto lexically_normal = path.lexically_normal();
//	auto key = lexically_normal.generic_u8string();
	auto key = libv::generic_path(lexically_normal);

	const auto it = self->cache_texture2D.find(key);
	if (it != self->cache_texture2D.end()) {
		sp = it->second.lock();
		if (sp) {
			// Cache hit
//			log_ui.trace("Texture2D cache hit for: {} as {}", libv::generic_path(path), key);
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

	const auto target = self->settings.res_texture.base_path / lexically_normal;

	if (!secure_path(self->settings.res_texture.base_path, self->settings.res_texture.restict_under_base, target, lexically_normal)) {
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

[[nodiscard]] bool ContextResource::texture2D_exists(const std::filesystem::path& path) {
	const auto lexically_normal = path.lexically_normal();
	const auto target = self->settings.res_texture.base_path / lexically_normal;

	if (!secure_path(self->settings.res_texture.base_path, self->settings.res_texture.restict_under_base, target, lexically_normal))
		return false;

	std::error_code ignore_ec;
	return std::filesystem::exists(target, ignore_ec);
}

// -------------------------------------------------------------------------------------------------

//std::shared_ptr<Shader> ContextResource::shader(const std::string& name) {
//	// Not implemented yet.
//}
//
//std::shared_ptr<ShaderFont> ContextResource::shaderFont(const std::string_view name) {
//	// Not implemented yet.
//}
//
//std::shared_ptr<ShaderImage> ContextResource::shaderImage(const std::string_view name) {
//	// Not implemented yet.
//}
//
//std::shared_ptr<ShaderQuad> ContextResource::shaderQuad(const std::string_view name) {
//	// Not implemented yet.
//}

// -------------------------------------------------------------------------------------------------

std::shared_ptr<ShaderFont> ContextResource::shaderFont() {
	return self->getShader(self->shader_font, "ShaderFont");
}

std::shared_ptr<ShaderImage> ContextResource::shaderImage() {
	return self->getShader(self->shader_image, "ShaderImage");
}

std::shared_ptr<ShaderQuad> ContextResource::shaderQuad() {
	return self->getShader(self->shader_quad, "ShaderQuad");
}

// -------------------------------------------------------------------------------------------------

std::shared_ptr<Font2D> ContextResource::fallbackFont() const {
	return self->fallback_font;
}

std::shared_ptr<Texture2D> ContextResource::fallbackTexture2D() const {
	return self->fallback_texture2D;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
