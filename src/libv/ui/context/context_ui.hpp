// Project: libv.ui, File: src/libv/ui/context/context_ui.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/utility/function_ref.hpp>
#include <libv/utility/intrusive_ptr.hpp>
#include <libv/utility/type_key.hpp>
// std
#include <filesystem>
#include <memory>
#include <string_view>
// pro
#include <libv/ui/event/detail/internal_event_linkage.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/style_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

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

private:
	std::unique_ptr<class ImplContextUI> self;

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
	[[nodiscard]] bool isAnyStyleDirty() const noexcept;
	void clearEveryStyleDirty() noexcept;
	void foreach_style(libv::function_ref<void(std::string_view name, Style& style)> func);

public:
	void focus(CoreComponent& component);
	void detachFocused(CoreComponent& component);
	void detachFocusLinked(CoreComponent& component);

public:
	template <typename Event> inline void broadcast(const Event& event);

public:
	[[nodiscard]] std::shared_ptr<Font2D> font(const std::filesystem::path& path);
	[[nodiscard]] std::shared_ptr<Texture2D> texture2D(const std::filesystem::path& path);
	[[nodiscard]] bool texture2D_exists(const std::filesystem::path& path);
	[[nodiscard]] libv::intrusive_ptr<Style> style(const std::string_view style_name);

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

template <typename Event>
inline void ContextUI::broadcast(const Event& event) {
	detail::internal_fire_global(this->event, libv::type_key<Event>(), &event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
