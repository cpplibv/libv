// Project: libv.ui, File: src/libv/ui/context/context_resource.hpp

#pragma once

#include <libv/ui/fwd.hpp>
#include <libv/ui/context/context_ui.hpp> // IWYU pragma: export // For convenience

#include <memory>
#include <string_view>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

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
	[[nodiscard]] std::shared_ptr<Font2D> font(std::string_view key);
	[[nodiscard]] std::shared_ptr<Texture2D> texture2D(std::string_view key);
	[[nodiscard]] bool texture2D_exists(std::string_view key);

	// [[nodiscard]] std::shared_ptr<Shader> shader(const std::string_view name);
	// [[nodiscard]] std::shared_ptr<ShaderFont> shaderFont(const std::string_view name);
	// [[nodiscard]] std::shared_ptr<ShaderImage> shaderImage(const std::string_view name);
	// [[nodiscard]] std::shared_ptr<ShaderQuad> shaderQuad(const std::string_view name);

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
