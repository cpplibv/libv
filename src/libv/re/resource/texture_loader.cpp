// Project: libv.re, File: src/libv/re/resource/texture_loader.cpp

#include <cppcoro/generator.hpp>

#include <libv/gl/image.hpp>
#include <libv/gl/load_image.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/basic_resource_loader.hxx>
#include <libv/re/resource/texture.hpp>
#include <libv/re/resource/texture_fmt.hpp>
#include <libv/re/resource/texture_load_event.hpp>
#include <libv/re/resource/texture_loader.hpp>
#include <libv/re/settings.hpp>
#include <libv/utility/hash.hpp>
#include <libv/utility/nexus.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

DelayedTextureLoadGuard::~DelayedTextureLoadGuard() {
	if (requestLoadOnDestroy)
		ResourceAccess::requestActivatingLoad(*resource, mode);
}

// -------------------------------------------------------------------------------------------------

struct TextureKey {
	std::string_view identifier;
	VariantID variant;

	[[nodiscard]] std::size_t hash() const noexcept {
		// NOTE: This hash function MUST BE kept in sync with Texture.hash()
		return libv::hash_combine(identifier, variant);
	}

	[[nodiscard]] friend bool operator==(const Texture& lhs, const TextureKey& rhs) {
		return lhs.identifier() == rhs.identifier && lhs.variant() == rhs.variant;
	}
	[[nodiscard]] friend bool operator==(const TextureKey& lhs, const Texture& rhs) {
		return rhs == lhs;
	}
};

struct TraitsTextureLoader {
	static constexpr std::string_view name_resource = "texture";
	static constexpr std::string_view name_Resource = "Texture";
	static constexpr std::string_view name_Loader = "TextureLoader";
	using Resource = Texture;
	using ResourceID = TextureID;
	using ResourceKey = TextureKey;
	using DelayedLoadGuard = DelayedTextureLoadGuard;
};

struct ImplTextureLoader : BasicResourceLoader<ImplTextureLoader, TraitsTextureLoader> {
	Texture_ptr fallback;
	std::unordered_map<std::string_view, Texture_ptr> builtins;

	libv::Nexus nexus;

public:
	explicit ImplTextureLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl) :
		BasicResourceLoader<ImplTextureLoader, TraitsTextureLoader>(settings.texture),
		nexus(nexus) {
		(void) gl;
	}

	~ImplTextureLoader() {
		builtins.clear();
		fallback = nullptr;
		auto lock = std::unique_lock(mutex);
		if (!cache.empty())
			log_re.error("TextureLoader is destroyed before every cached Texture instance is destroyed. {} Texture alive.", cache.size());
		if (!watchedFiles.empty())
			log_re.error("TextureLoader is destroyed before every watched fsw token is destroyed. {} File token alive.", watchedFiles.size());
	}

public:
	void initFallbacks() {
		static constexpr auto color = libv::vec4uc{255, 0, 255, 255}; // magenta
		fallback = Texture_ptr::make("fallback:magenta");
		fallback->storage(libv::gl::TextureTarget::_2D, libv::gl::FormatSized::RGBA8, 1, libv::vec2i{1, 1});
		fallback->image(0, libv::vec2i{0, 0}, libv::vec2i{1, 1}, libv::gl::DataType::U8, reinterpret_cast<const std::byte*>(&color));
	}

	void initBuiltins() {
		const auto solid2D = [this](std::string_view name, libv::vec4uc color) {
			auto resource = Texture_ptr::make(std::string(name));
			resource->storage(libv::gl::TextureTarget::_2D, libv::gl::FormatSized::RGBA8, 1, libv::vec2i{1, 1});
			resource->image(0, libv::vec2i{0, 0}, libv::vec2i{1, 1}, libv::gl::DataType::U8, reinterpret_cast<const std::byte*>(&color));
			builtins.emplace(name, std::move(resource));
		};

		solid2D("builtin:up",          libv::vec4uc{127, 127, 255, 255});
		solid2D("builtin:down",        libv::vec4uc{127, 127,   0, 255});

		solid2D("builtin:black",       libv::vec4uc{  0,   0,   0, 255});
		solid2D("builtin:gray",        libv::vec4uc{127, 127, 127, 255});
		solid2D("builtin:white",       libv::vec4uc{255, 255, 255, 255});
		solid2D("builtin:red",         libv::vec4uc{255,   0,   0, 255});
		solid2D("builtin:green",       libv::vec4uc{  0, 255,   0, 255});
		solid2D("builtin:blue",        libv::vec4uc{  0,   0, 255, 255});
		solid2D("builtin:magenta",     libv::vec4uc{255,   0, 255, 255});

		solid2D("builtin:transparent", libv::vec4uc{  0,   0,   0,   0});
		solid2D("builtin:none",        libv::vec4uc{  0,   0,   0,   0});
	}

public:
	// NOTE: Events could broadcast inline or on any other thread, but for easier UI interactions they are switched to GL thread
	inline void broadcast(TextureLoadSuccess&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcast(TextureLoadFailure&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcast(TextureUnload&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcastResourceUnload(Texture_ptr&& resource) const {
		broadcast(TextureUnload{resource->id(), std::move(resource)});
	}

	[[nodiscard]] Texture_ptr _loadBuiltin(std::string_view identifier, VariantID variant) {
		if (!identifier.starts_with("builtin:"))
			return nullptr;

		log_re.warn_if(variant != VariantID::Default, "Requested builtin texture '{}' with non-default variant: {}", identifier, +variant);
		const auto it = builtins.find(identifier);
		if (it == builtins.end()) {
			log_re.error("Requested texture is not a valid builtin: '{}'", identifier);
			return fallback;
		} else {
			return it->second;
		}
	}

	cppcoro::generator<ThreadAffinity> loadingRoutine(Texture_ptr texture, LoadingMode mode) {
		// NOTE: Owning ptr keeps alive the resource during the whole loading process
		log_re.trace("{} {} texture: '{}'",
				texture->loadVersion() == -1 ? "Loading" : "Reloading", to_string(mode), *texture);

		std::vector<WatchedFile>* watchTokens = nullptr;
		std::vector<WatchedFile> oldTokens;
		{
			auto lock = std::unique_lock(mutex);
			if (trackFiles) {
				// NOTE: Taking the address relies on that std::unordered_map is stable
				watchTokens = &watchedFiles[texture->id()];
				// Moving out old fsw tokens and keeping them alive until the new watches are created to avoid subscription changes in fsw
				std::swap(*watchTokens, oldTokens);
			}
		}

		co_yield ThreadAffinity::FS; // -------------------------------------------------------------------

		auto fileData = _fs_loadFile(
				watchTokens,
				r.settings.texture.resourceMappings,
				texture->identifier(),
				texture->id());

		if (!fileData) {
			ResourceAccess::recordLoadFailure(*texture);
			broadcast(TextureLoadFailure{
				texture->id(),
				texture,
				TextureLoadFailure::IOFailure{
						std::move(fileData.error().unmatchedMappings),
						std::move(fileData.error().mappingErrors),
						std::move(fileData.error().physicalErrors)
				},
				std::nullopt
			});
			co_return;
		}

		oldTokens.clear(); // Discard old fsw tokens, as the new tokens are already in place

		co_yield ThreadAffinity::CPU; // ------------------------------------------------------------------

		auto image = libv::gl::load_image(*fileData);
		if (!image) {
			log_re.error("Failed to load texture: '{}'. {}", *texture, "Invalid or unsupported image data or format.");
			ResourceAccess::recordLoadFailure(*texture);
			broadcast(TextureLoadFailure{
				texture->id(),
				texture,
				std::nullopt,
				"Invalid or unsupported image format or data."
			});
			co_return;
		}

		const auto texture_type = image->type();
		texture->image(std::move(*image));

		// TODO P2: filtering, anisotropic filtering, mipmap, generic 'usage' flag that auto sets these
		if (texture_type == libv::gl::TextureTarget::CubeMap || texture_type == libv::gl::TextureTarget::CubeMapArray) {
			// Cube maps should set Wrap to ClampToEdge and should not generate mips
			texture->set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
			texture->set(libv::gl::MagFilter::Linear);
			texture->set(libv::gl::MinFilter::LinearMipmapLinear);
		} else {
			// Everything else should have mipmaps
			// (Could be improved as UI textures don't need mipmaps)
			texture->set(libv::gl::Wrap::MirroredRepeat, libv::gl::Wrap::MirroredRepeat);
			texture->set(libv::gl::MagFilter::Linear);
			texture->set(libv::gl::MinFilter::LinearMipmapLinear);
			texture->generateMipmaps();
		}

		// No need to switch to ThreadAffinity::GL as the current resource API-s will update themselves during render
		// co_yield ThreadAffinity::GL; --------------------------------------------------------------------

		// The OpenGL parts of the texture loading cannot fail (without internal errors), report success now
		ResourceAccess::recordLoadSuccess(*texture);
		broadcast(TextureLoadSuccess{texture->id(), texture});

		co_return;
	}
};

// -------------------------------------------------------------------------------------------------

TextureLoader::TextureLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl) :
	self(std::make_unique<ImplTextureLoader>(settings, nexus, gl)) { }

void TextureLoader::init() {
	self->initFallbacks();
	self->initBuiltins();
}

void TextureLoader::reset() {
	self.reset();
}

TextureLoader::~TextureLoader() {
	// For the sake of forward declared types
}

Texture_ptr TextureLoader::load(std::string_view identifier, VariantID variant, LoadingMode mode) {
	return self->load(mode, self->fallback, identifier, variant);
}

Texture_ptr TextureLoader::load(std::string_view identifier, VariantID variant, LoadingMode mode, Texture_ptr fallback) {
	return self->load(mode, fallback, identifier, variant);
}

Texture_ptr TextureLoader::load_async(std::string_view identifier, VariantID variant) {
	return self->load(LoadingMode::async, self->fallback, identifier, variant);
}

DelayedTextureLoadGuard TextureLoader::load_delayed(std::string_view identifier, VariantID variant, LoadingMode mode) {
	return self->load_delayed(mode, self->fallback, identifier, variant);
}

DelayedTextureLoadGuard TextureLoader::load_delayed(std::string_view identifier, VariantID variant, LoadingMode mode, Texture_ptr fallback) {
	return self->load_delayed(mode, fallback, identifier, variant);
}

DelayedTextureLoadGuard TextureLoader::load_delayed_async(std::string_view identifier, VariantID variant) {
	return self->load_delayed(LoadingMode::async, self->fallback, identifier, variant);
}

Texture_ptr TextureLoader::fallback() {
	return self->fallback;
}

void TextureLoader::executeLoad(Texture_ptr resource, LoadingMode mode) {
	self->executeLoad(std::move(resource), mode);
}

void TextureLoader::unload(Texture* resource) {
	self->requestUnload(resource);
}

void TextureLoader::foreach_texture(libv::function_ref<void(const Texture_ptr&)> func) {
	const auto lock = std::unique_lock(self->mutex);
	for (const auto& texture : self->cache)
		func(texture);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
