// Project: libv.ui, File: src/libv/ui/context/context_ui.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/intrusive_ptr.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <typeindex>
// pro
#include <libv/ui/settings.hpp>
#include <libv/ui/style_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreComponent;
class ContextEvent;
class ContextMouse;
class ContextState;
class Font2D;
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
	//			+ tracking
	//			+ preprocess/include/recurse
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
	ContextMouse& mouse;
	Settings settings;

public:
	explicit ContextUI(UI& ui, ContextState& state, Settings settings = Settings());
	~ContextUI();

	ContextUI(const ContextUI&) = delete;
	ContextUI(ContextUI&&) = delete;
	ContextUI& operator=(const ContextUI&) = delete;
	ContextUI& operator=(ContextUI&&) = delete;

public:
	[[nodiscard]] std::string clipboardText();
	void clipboardText(const std::string& string);

public:
	[[nodiscard]] bool isAnyStyleDirty() const noexcept;
	void clearEveryStyleDirty() noexcept;

public:
	void focus(CoreComponent& component);
	void detachFocused(CoreComponent& component);
	void detachFocusLinked(CoreComponent& component);

public:
	template <typename Event> inline void fire(const Event& event);

public:
	[[nodiscard]] std::shared_ptr<Font2D> font(const std::filesystem::path& path);
	[[nodiscard]] std::shared_ptr<Texture2D> texture2D(const std::filesystem::path& path);
	[[nodiscard]] libv::intrusive_ptr<Style> style(const std::string_view style_name);

	[[nodiscard]] std::shared_ptr<Shader> shader(const std::string& name);
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

namespace detail {
// Skipping the inclusion for this function only #include <libv/ui/basic_event_proxy.hpp> (or the creation of a new header)
void internal_fire_global(ContextEvent& ctx, std::type_index event_type, const void* event_ptr);
} // namespace detail

template <typename Event>
inline void ContextUI::fire(const Event& event) {
	detail::internal_fire_global(this->event, std::type_index(typeid(Event)), &event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
