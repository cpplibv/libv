// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// std
#include <filesystem>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
// pro
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Font2D;
class Shader;
class Texture2D;

// TODO P4: Find a header for type info ref, and a better name
using TypeInfoRef = std::reference_wrapper<const std::type_info>;

struct TypeInfoRefHasher {
    std::size_t operator()(TypeInfoRef code) const {
        return code.get().hash_code();
    }
};

struct TypeInfoRefEqualTo {
    bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const {
        return lhs.get() == rhs.get();
    }
};

// -------------------------------------------------------------------------------------------------

class ContextUI {
	// TODO P1: The Uniform UI Resource System:
	//			- define
	//			- resolve
	//			- lookup
	//			- cache
	//			- provide
	// TODO P1: Implement higher level cache functions:
	//			auto __ = cacheResolve(cacheFile, filePath);
	//			auto __ = cacheLookup(cacheFile, filePath);
	//			auto __ = cacheStore(cacheFile, filePath, filePath);
	// TODO P5: cleanup weak_ptr references with intrusive ptrs

	std::shared_ptr<Font2D> fallback_font;
	std::shared_ptr<Texture2D> fallback_texture2D;
	std::unordered_map<std::string, std::weak_ptr<Font2D>> cache_font;
//	std::unordered_map<std::string, std::weak_ptr<Shader>> cache_shader;
	std::unordered_map<std::string, std::weak_ptr<Texture2D>> cache_texture2D;
	std::unordered_map<TypeInfoRef, std::weak_ptr<Shader>, TypeInfoRefHasher, TypeInfoRefEqualTo> cache_typed_shader;

	std::filesystem::path path_fonts;
	std::filesystem::path path_shaders;
	std::filesystem::path path_textures;

//	struct Config {
//		struct Resource {
//			bool cache_fallback = true;
//			bool track_every = false;
//			std::unordered_set<std::string> track_specific;
//		};
//		Resource font;
//		Resource shader;
//		Resource texture;
//	};

public:
	ContextUI(
			const std::filesystem::path& path_base = "res",
			const std::filesystem::path& path_fonts = "font",
			const std::filesystem::path& path_shaders = "shader",
			const std::filesystem::path& path_textures = "texture");

	std::shared_ptr<Font2D> font(const std::filesystem::path& path);
	std::shared_ptr<Shader> shader(const std::string& name);
	std::shared_ptr<Texture2D> texture2D(const std::filesystem::path& path);

	template <typename T>
		LIBV_REQUIRES(std::is_base_of_v<Shader, T>)
	std::shared_ptr<T> shader() {
		auto& wp = cache_typed_shader[typeid(T)];
		auto sp = std::static_pointer_cast<T>(wp.lock());
		if (sp) {
			log_ui.trace("Shader cache hit for: {}", typeid(T).name());
		} else {
			log_ui.trace("Shader cache miss for: {}", typeid(T).name());
			wp = sp = std::make_shared<T>();
		}
		return sp;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
