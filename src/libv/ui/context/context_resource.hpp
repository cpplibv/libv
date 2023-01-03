// Project: libv.ui, File: src/libv/ui/context/context_resource.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// std
#include <filesystem>
#include <memory>
#include <string_view>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// TODO P1: The Uniform UI Resource System:
//			- define  - tell the system in any way you can what resource are you looking for
//			- resolve - normalize the resource request
//			- lookup  - find the resource
//			- cache   - cache the resource
//			- provide - yield the result
//			+ tracking
//			+ preprocess/include/recurse
// TODO P1: Implement higher level cache functions:
//			auto __ = cacheResolve(cacheFile, filePath);
//			auto __ = cacheLookup(cacheFile, filePath);
//			auto __ = cacheStore(cacheFile, filePath, filePath);

class ImplContextResource;

class ContextResource {
private:
	std::unique_ptr<ImplContextResource> self;

public:
	explicit ContextResource(Settings& settings);
	~ContextResource();

	ContextResource(const ContextResource&) = delete;
	ContextResource(ContextResource&&) = delete;
	ContextResource& operator=(const ContextResource&) = delete;
	ContextResource& operator=(ContextResource&&) = delete;

public:
	[[nodiscard]] std::shared_ptr<Font2D> font(const std::filesystem::path& path);
	[[nodiscard]] std::shared_ptr<Texture2D> texture2D(const std::filesystem::path& path);
	[[nodiscard]] bool texture2D_exists(const std::filesystem::path& path);

	[[nodiscard]] std::shared_ptr<Shader> shader(const std::string_view name);
	[[nodiscard]] std::shared_ptr<ShaderFont> shaderFont(const std::string_view name);
	[[nodiscard]] std::shared_ptr<ShaderImage> shaderImage(const std::string_view name);
	[[nodiscard]] std::shared_ptr<ShaderQuad> shaderQuad(const std::string_view name);

public:
	[[nodiscard]] std::shared_ptr<ShaderFont> shaderFont();
	[[nodiscard]] std::shared_ptr<ShaderImage> shaderImage();
	[[nodiscard]] std::shared_ptr<ShaderQuad> shaderQuad();
	[[nodiscard]] std::shared_ptr<Font2D> fallbackFont() const;
	[[nodiscard]] std::shared_ptr<Texture2D> fallbackTexture2D() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
