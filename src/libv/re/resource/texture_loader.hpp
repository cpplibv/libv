// Project: libv.re, File: src/libv/re/resource/texture_loader.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/r.hpp> // IWYU pragma: export // Convenience include to access the libv::re::r.texture object
#include <libv/re/resource/common_resource.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/nexus_fwd.hpp>

#include <memory>
#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct DelayedTextureLoadGuard {
	using Resource_ptr = Texture_ptr;
	Resource_ptr resource;
	LoadingMode mode;
	bool requestLoadOnDestroy = false;

	DelayedTextureLoadGuard(Resource_ptr&& resource, LoadingMode mode, bool requestLoadOnDestroy) :
			resource(std::move(resource)), mode(mode), requestLoadOnDestroy(requestLoadOnDestroy) {}

	DelayedTextureLoadGuard(const DelayedTextureLoadGuard&) = delete;
	DelayedTextureLoadGuard& operator=(const DelayedTextureLoadGuard&) & = delete;
	DelayedTextureLoadGuard(DelayedTextureLoadGuard&&) = delete;
	DelayedTextureLoadGuard& operator=(DelayedTextureLoadGuard&&) & = delete;

	inline Resource_ptr::value_type* operator->() const {
		return resource.get();
	}

	inline Resource_ptr& operator*() {
		return resource;
	}

	inline explicit(false) operator const Resource_ptr&() const& {
		return resource;
	}

	[[nodiscard]] constexpr inline bool isFirstLoad() const noexcept {
		return requestLoadOnDestroy;
	}

	/// The destructor dispatches the async loading of the resource
	~DelayedTextureLoadGuard();
};

// -------------------------------------------------------------------------------------------------

class ImplTextureLoader;

/// Builtin single color 2D textures:
///                            R    G    B    A
///     builtin:up          = 127, 127, 255, 255
///     builtin:down        = 127, 127,   0, 255
///     builtin:black       =   0,   0,   0, 255
///     builtin:gray        = 127, 127, 127, 255
///     builtin:white       = 255, 255, 255, 255
///     builtin:red         = 255,   0,   0, 255
///     builtin:green       =   0, 255,   0, 255
///     builtin:blue        =   0,   0, 255, 255
///     builtin:magenta     = 255,   0, 255, 255
///     builtin:transparent =   0,   0,   0,   0
///     builtin:none        =   0,   0,   0,   0
///
/// @Nexus emit global libv::re::TextureLoadSuccess
/// @Nexus emit global libv::re::TextureLoadFailure
/// @Nexus emit global libv::re::TextureUnload
class TextureLoader {
private:
	std::unique_ptr<ImplTextureLoader> self;

public:
	TextureLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl);
	void init();
	void reset();
	~TextureLoader();

public:
	/// @thread any
	Texture_ptr load(std::string_view identifier, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	Texture_ptr load(std::string_view identifier, VariantID variant, LoadingMode mode, Texture_ptr fallback);
	/// @thread any
	Texture_ptr load_async(std::string_view identifier, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedTextureLoadGuard load_delayed(std::string_view identifier, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	DelayedTextureLoadGuard load_delayed(std::string_view identifier, VariantID variant, LoadingMode mode, Texture_ptr fallback);
	/// @thread any
	DelayedTextureLoadGuard load_delayed_async(std::string_view identifier, VariantID variant = VariantID::Default);

	/// @thread any
	Texture_ptr fallback();

private:
	friend BasicResource;
	void executeLoad(Texture_ptr texture, LoadingMode mode); // Hook to execute resource loading
	void unload(Texture* texture); // Hook to unregister from the cache

public:
	/// @thread any, but the callback cannot reenter the object as a mutex is held during the whole iteration
	void foreach_texture(libv::function_ref<void(const Texture_ptr&)> func);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
