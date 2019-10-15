// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/utility/intrusive_ptr.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
// pro
#include <libv/ui/settings.hpp>
#include <libv/ui/style_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BaseComponent;
class ContextEvent;
class ContextState;
class Font2D;
class MouseTable;
class Shader;
class ShaderFont;
class ShaderImage;
class ShaderQuad;
class Style;
class Texture2D;
class UI;

class ImplContextUI;

class ContextUI {
	// TODO P1: The Uniform UI Resource System:
	//			- define  - tell the system in any way you can what resource are you looking for
	//			- resolve - normalize the resource request
	//			- lookup  - find the resource
	//			- cache   - cache the resource
	//			- provide - yield the result
	// TODO P1: Implement higher level cache functions:
	//			auto __ = cacheResolve(cacheFile, filePath);
	//			auto __ = cacheLookup(cacheFile, filePath);
	//			auto __ = cacheStore(cacheFile, filePath, filePath);
	// TODO P5: cleanup weak_ptr references with intrusive ptrs
	// TODO P5: style unordered_map could be a unordered_set, (generalize dereference hasher)

	std::unique_ptr<ImplContextUI> self;

	UI& ui;

public:
	ContextEvent& event;
	ContextState& state;
	MouseTable& mouse;
	Settings settings;

public:
	explicit ContextUI(UI& ui, ContextState& state, Settings settings = Settings());
	~ContextUI();

public:
	std::string clipboardText();
	void clipboardText(const std::string& string);

public:
	bool isAnyStyleDirty() const noexcept;
	void clearEveryStyleDirty() noexcept;

public:
	void focus(BaseComponent& component);
	void detachFocused(BaseComponent& component);
	void detachFocusLinked(BaseComponent& component);

public:
	std::shared_ptr<Font2D> font(const std::filesystem::path& path);
	std::shared_ptr<Texture2D> texture2D(const std::filesystem::path& path);
	libv::intrusive_ptr<Style> style(const std::string_view style_name);

	std::shared_ptr<Shader> shader(const std::string& name);
	std::shared_ptr<ShaderFont> shaderFont(const std::string_view name);
	std::shared_ptr<ShaderImage> shaderImage(const std::string_view name);
	std::shared_ptr<ShaderQuad> shaderQuad(const std::string_view name);

public:
	std::shared_ptr<ShaderFont> shaderFont();
	std::shared_ptr<ShaderImage> shaderImage();
	std::shared_ptr<ShaderQuad> shaderQuad();
	std::shared_ptr<Font2D> fallbackFont() const;
	std::shared_ptr<Texture2D> fallbackTexture2D() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
