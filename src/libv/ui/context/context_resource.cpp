// Project: libv.ui, File: src/libv/ui/context/context_resource.cpp

// hpp
#include <libv/ui/context/context_resource.hpp>
// libv
#include <libv/gl/load_image.hpp>
#include <libv/res/resource_path.hpp>
#include <libv/utility/hash_string.hpp>
#include <libv/utility/is_parent_folder_of.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <filesystem>
#include <memory>
#include <mutex>
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

// struct Traits {
// 	static constexpr std::string_view resource_name;
// 	[[nullable]] std::shared_ptr<T> create(Args&&...);
// };

template <typename T, typename CRTP>
class ResourceCache {
private:
	mutable std::mutex mutex;
	std::shared_ptr<T> fallback_;
	std::unordered_map<std::string, std::weak_ptr<T>, libv::hash_string, std::equal_to<>> map;

public:
	explicit ResourceCache(std::shared_ptr<T>&& fallback_) : fallback_(std::move(fallback_)) {}

private:
	[[nodiscard]] constexpr inline bool isNormalizedKey(std::string_view key) const {
		return !key.contains('\\') &&
				!key.starts_with("../") &&
				!key.starts_with("./") &&
				!key.starts_with('/') &&
				!key.contains("/../") &&
				!key.contains(':');
	}

public:
	[[nodiscard]] std::shared_ptr<T> load(std::string_view key) {
		auto& self = static_cast<CRTP&>(*this);
		auto lock = std::unique_lock(mutex);

		auto it = map.find(key);
		if (it != map.end()) {
			if (auto sp = it->second.lock()) { // Cache hit
				return sp;
			} else { // Cache expired
				log_ui.trace("{} cache expired: {}", self.resource_Name, key);
			}
		} else { // Cache miss
			log_ui.trace("{} cache miss: {}", self.resource_Name, key);
			it = map.emplace(key, std::weak_ptr<T>()).first;
		}

		const auto normalized = isNormalizedKey(key);
		if (!normalized) {
			if (self.res_settings().restrict_under_base) {
				log_ui.error("{} request is not normalized: {}. Using fallback {}", self.resource_Name, key, self.resource_name);
				return nullptr;
			} else {
				log_ui.warn("{} request is not normalized: {}", self.resource_Name, key);
			}
		}

		const auto filepath_str = self.res_settings().base_path + std::string(key);

		std::filesystem::path filepath;
		if (!self.settings.use_libv_res_resource_path) {
			filepath = filepath_str;
		} else {
			try {
				const auto resolved_str = libv::res::resource_path(filepath_str);
				log_ui.trace_if(filepath_str != resolved_str, "{} mapping: {} to {}", self.resource_Name, filepath_str, resolved_str);
				filepath = std::filesystem::path(resolved_str);
			} catch (const std::exception& ex) {
				log_ui.error("{} resolve failed: {}. Using fallback {}", self.resource_Name, filepath_str, self.resource_name);
				return nullptr;
			}
		}

		auto sp = self.create(filepath);
		if (!sp)
			sp = fallback_;
		it->second = sp;
		return sp;
	}

	[[nodiscard]] bool exists(std::string_view key) const {
		auto& self = static_cast<const CRTP&>(*this);
		auto lock = std::unique_lock(mutex);

		if (self.res_settings().restrict_under_base && !isNormalizedKey(key))
			return false;

		const auto filepath_str = self.res_settings().base_path + std::string(key);
		try {
			const auto resolved_str = libv::res::resource_path(filepath_str);

			std::error_code ignore_ec;
			return std::filesystem::exists(resolved_str, ignore_ec);
		} catch (const std::exception& ex) {
			return false;
		}
	}

	[[nodiscard]] inline const std::shared_ptr<T>& fallback() const {
		return fallback_;
	}
};

// =================================================================================================

class FontCache : public ResourceCache<Font2D, FontCache> {
public:
	static constexpr const std::string_view resource_name = "font";
	static constexpr const std::string_view resource_Name = "Font";
	const Settings& settings;

public:
	explicit inline FontCache(Settings& settings) :
			ResourceCache<Font2D, FontCache>([]{
				const auto fallback_font_data = raw_font_consolas_min();
				return std::make_shared<Font2D>(
						std::string{reinterpret_cast<const char*>(fallback_font_data.first), fallback_font_data.second});
			}()),
	        settings(settings) {}

	[[nodiscard]] const ResourceSettings& res_settings() const {
		return settings.res_font;
	}

	[[nodiscard]] std::shared_ptr<Font2D> create(const std::filesystem::path& filepath) {
		// TODO P1: Detect isLoadedAlready
		// log_ui.warn_if(isLoadedAlready(filepath), "the same resource loaded multiple times with different name");

		libv::Timer timer;
		auto file = libv::read_file_ec(filepath);
		if (file.ec) {
			log_ui.error("{} load failed: IO Failure: {}: {} - {}. Using fallback {}", resource_Name, filepath.generic_string(), file.ec, file.ec.message(), resource_name);
			return nullptr;
		}
		const auto time_read = timer.timef_ms();

		try {
			auto sp = std::make_shared<Font2D>(std::move(file.data));
			const auto time_load = timer.timef_ms();
			log_ui.trace("{} loaded: read:{:5.1f}ms, load:{:5.1f}ms for {}",
					resource_Name, time_read.count(), time_load.count(), filepath.generic_string());
			return sp;
		} catch (const std::exception& ex) {
			log_ui.error("{} load failed: {}. Using fallback {}", resource_Name, filepath.generic_string(), resource_name);
			return nullptr;
		}
	}
};

// =================================================================================================

class Texture2DCache : public ResourceCache<Texture2D, Texture2DCache> {
public:
	static constexpr const std::string_view resource_name = "texture";
	static constexpr const std::string_view resource_Name = "Texture";
	const Settings& settings;

public:
	explicit inline Texture2DCache(Settings& settings) :
			ResourceCache<Texture2D, Texture2DCache>([]{
				const auto fallback_texture2D_data = raw_texture2D_white256();
				return std::make_shared<Texture2D>(libv::gl::load_image_or_throw(
						{reinterpret_cast<const char*>(fallback_texture2D_data.first), fallback_texture2D_data.second})
				);
			}()),
	        settings(settings) {}

	[[nodiscard]] const ResourceSettings& res_settings() const {
		return settings.res_texture;
	}

	std::shared_ptr<Texture2D> create(const std::filesystem::path& filepath) {
		// TODO P1: Detect isLoadedAlready
		// log_ui.warn_if(isLoadedAlready(filepath), "the same resource loaded multiple times with different name");

		libv::Timer timer;
		auto file = libv::read_file_ec(filepath);
		if (file.ec) {
			log_ui.error("{} load failed: IO Failure: {}: {} - {}. Using fallback {}", resource_Name, filepath.generic_string(), file.ec, file.ec.message(), resource_name);
			return nullptr;
		}
		const auto time_read = timer.timef_ms();

		auto image = libv::gl::load_image(file.data);
		if (!image) {
			log_ui.error("{} load failed: {}. Using fallback {}", resource_Name, filepath.generic_string(), resource_name);
			return nullptr;
		}
		const auto time_load = timer.timef_ms();

		log_ui.trace("{} loaded: read:{:5.1f}ms, load:{:5.1f}ms for {}",
				resource_Name, time_read.count(), time_load.count(), filepath.generic_string());

		return std::make_shared<Texture2D>(std::move(*image));
	}
};

// =================================================================================================

class ImplContextResource {
public:
	Settings& settings;

	FontCache cacheFont;
	Texture2DCache cacheTexture2D;

//	std::unordered_map<std::string, std::weak_ptr<Shader>> cache_shader;
//	std::unordered_map<TypeInfoRef, std::weak_ptr<Shader>, TypeInfoRefHasher, TypeInfoRefEqualTo> cache_typed_shader;

	std::shared_ptr<ShaderFont> shader_font;
	std::shared_ptr<ShaderImage> shader_image;
	std::shared_ptr<ShaderQuad> shader_quad;

public:
	explicit ImplContextResource(Settings& settings) :
		settings(settings),
		cacheFont(settings),
		cacheTexture2D(settings) {}

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

ContextResource::ContextResource(Settings& settings) :
	self(std::make_unique<ImplContextResource>(settings)) {

	log_ui.info("Working directory:     {}", std::filesystem::current_path().generic_string());
	log_ui.info("Resource base font:    {}", self->settings.res_font.base_path);
	log_ui.info("Resource base shader:  {}", self->settings.res_shader.base_path);
	log_ui.info("Resource base texture: {}", self->settings.res_texture.base_path);
}

ContextResource::~ContextResource() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

std::shared_ptr<Font2D> ContextResource::font(std::string_view key) {
	return self->cacheFont.load(key);
}

std::shared_ptr<Texture2D> ContextResource::texture2D(std::string_view key) {
	return self->cacheTexture2D.load(key);
}

[[nodiscard]] bool ContextResource::texture2D_exists(std::string_view key) {
	return self->cacheTexture2D.exists(key);
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
	return self->cacheFont.fallback();
}

std::shared_ptr<Texture2D> ContextResource::fallbackTexture2D() const {
	return self->cacheTexture2D.fallback();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
