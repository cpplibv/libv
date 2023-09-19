// Project: libv.rev, File: src/libv/rev/resource/texture_loader_internal.cpp

// hpp
#include <libv/rev/resource/texture_loader_internal.hxx>
// libv
#include <libv/gl/image.hpp>
#include <libv/gl/load_image.hpp>
#include <libv/utility/read_file.hpp>
// pro
#include <libv/rev/log.hpp>
#include <libv/rev/resource/resource_manager_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

InternalTextureLoader::InternalTextureLoader(const std::shared_ptr<InternalResourceManager>& irm) :
	irm(irm),
	includeDirectories(1, irm->settings.texture.base_path) {

	initDefaultGLR();
	// NOTE: glr default has to be set before first before anyone could request a load (and copy a default inited glr objects)
	//	irm->queue_gl([this]{ initDefaultGLR(); });
}

InternalTextureLoader::~InternalTextureLoader() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

void InternalTextureLoader::initDefaultGLR() {
	defaultGLR = createColorTextureGLR({255, 0, 255, 255});
	fallback_ = libv::make_intrusive2_ptr<InternalTexture>(std::weak_ptr<InternalTextureLoader>{}, "<<fallback>>", defaultGLR, defaultSwizzle);

	initBuiltin("builtin:up",          libv::vec4uc{127, 127, 255, 255});
	initBuiltin("builtin:down",        libv::vec4uc{127, 127,   0, 255});

	initBuiltin("builtin:black",       libv::vec4uc{  0,   0,   0, 255});
//	initBuiltin("builtin:gray",        libv::vec4uc{127, 127, 127, 255});
	initBuiltin("builtin:white",       libv::vec4uc{255, 255, 255, 255});

//	initBuiltin("builtin:red",         libv::vec4uc{255,   0,   0, 255});
//	initBuiltin("builtin:green",       libv::vec4uc{  0, 255,   0, 255});
//	initBuiltin("builtin:blue",        libv::vec4uc{  0,   0, 255, 255});
	initBuiltin("builtin:magenta",     libv::vec4uc{255,   0, 255, 255});

	// initBuiltin("builtin:uvtest", ...);
	// initBuiltin("builtin:uv", ...);
	// initBuiltin("builtin:mango", ...);

	initBuiltin("builtin:transparent", libv::vec4uc{  0,   0,   0,   0});
	initBuiltin("builtin:none",        libv::vec4uc{  0,   0,   0,   0});

	//	add_virtual_resource("lut:ace", ...);
	//	add_virtual_resource("lut:natural", ...);
	//	add_virtual_resource("lut:tonemap", ...);
}

libv::glr::Texture InternalTextureLoader::createColorTextureGLR(libv::vec4uc color) {
	// Let's do 2x2 textures as color for now
	// In the future if array textures are used 8x8 (64 pixel) might be the minimum (has to be verified)
	libv::glr::Texture glr_texture;
	libv::vec4uc data[4] = { color, color, color, color };
	glr_texture.storage(libv::gl::TextureTarget::_2D, 1, libv::vec2i{2, 2}, libv::gl::FormatSized::RGBA8, libv::gl::DataType::U8);
	glr_texture.image(0, libv::vec2i{0, 0}, libv::vec2i{2, 2}, data);
	return glr_texture;
}

void InternalTextureLoader::initBuiltin(std::string name, libv::vec4uc color) {
	builtins.emplace(libv::make_intrusive2_ptr<InternalTexture>(
			std::weak_ptr<InternalTextureLoader>{}, // Builtins are not tracked: loader can be null
			name,
			createColorTextureGLR(color),
			defaultSwizzle
	));
}

// -------------------------------------------------------------------------------------------------

InternalTexture_ptr InternalTextureLoader::load(std::string_view name, Swizzle swizzle) {
	auto lock = std::unique_lock(mutex);

	if (name.starts_with("builtin:")) {
		auto it = builtins.find(name);
		if (it == builtins.end()) {
			log_rev.error("Requested builtin texture is missing: {}", name);
			return fallback_;
		}

		return *it;
	}

	auto& ptr = cache.lookup(name, swizzle);

	if (!ptr)
		ptr = create_resource(name, swizzle);

	return ptr;
}

InternalTexture_ptr InternalTextureLoader::fallback() const {
	return fallback_;
}

void InternalTextureLoader::unload(InternalTexture* resource) {
	auto lock = std::unique_lock(mutex);

	if (resource->ref_count() > 1)
		return; // Resource got reacquired while this call was in flight

	cache.remove(resource);
}

// -------------------------------------------------------------------------------------------------

InternalTexture_ptr InternalTextureLoader::create_resource(std::string_view path, Swizzle swizzle) {
	log_rev.trace("Create new texture: {}", path);

	auto loader_sp = shared_from_this();

	// --- Start of real work

	auto res = libv::make_intrusive2_ptr<InternalTexture>(weak_from_this(), std::string(path), defaultGLR, swizzle);

	// --- End of real work

	irm->queue_fs([l = std::move(loader_sp), r = res]() mutable {
		l->process_fs(std::move(r));
	});

	return res;
}

void InternalTextureLoader::process_fs(InternalTexture_ptr&& res) {
	auto loader_sp = shared_from_this();

	// ---  Start of real work

	bool success = false;
	std::string data;

	auto lock = std::unique_lock(includeDirectories_m);
	for (const auto& includeDirectory : includeDirectories) {
		const auto resource_path = includeDirectory + res->name_;

//		std::filesystem::exists(resource_path);
//		irm->fsw.subscribe_file(resource_path, [] {
//		});

		auto resource_data = libv::read_file_ec(resource_path);
		if (resource_data.ec) {
//			errors.emplace_back(resource_path, resource_data.ec);
			log_rev.warn("Failed to read file {} for {}: {}", resource_path, res->name_, resource_data.ec.message());
			continue;
		} else {
			success = true;
			data = std::move(resource_data.data);
			break;
		}
	}

	if (!success) {
		log_rev.error("Failed to load model {}: Failed to load the file", res->name_);
		return;
	}

	// --- End of real work

	irm->queue_cpu([l = std::move(loader_sp), r = std::move(res), d = std::move(data)]() mutable {
		l->process_cpu(std::move(r), std::move(d));
	});
}

void InternalTextureLoader::process_cpu(InternalTexture_ptr&& res, std::string&& data) {
	auto loader_sp = shared_from_this();

	// --- Start of real work

	auto image = libv::gl::load_image(data);
	if (!image.has_value()) {
		log_rev.error("Failed to load texture: {}", res->name_);
		return;
	}

	// --- End of real work

	irm->queue_res([l = std::move(loader_sp), r = std::move(res), img = std::move(image.value())]() mutable {
		l->process_res(std::move(r), std::move(img));
	});
}

void InternalTextureLoader::process_res(InternalTexture_ptr&& res, libv::gl::Image&& image) {
	res->texture = libv::glr::Texture{std::move(image)};
	// TODO P2: filtering, anisotropic filtering, mipmap, usage
	// TODO P2: Cube maps should not generate mips and should set Wrap to ClampToEdge (this currently put into glr and overrides the following settings)
	res->texture.set(libv::gl::MagFilter::Linear);
	res->texture.set(libv::gl::MinFilter::LinearMipmapLinear);
//	res->texture.set(libv::gl::MagFilter::Nearest);
//	res->texture.set(libv::gl::MinFilter::Nearest);
	res->texture.generate_mipmaps();

	if (res->swizzle_ != defaultSwizzle)
		res->texture.set(
				to_libv_gl_swizzle(res->swizzle_.r),
				to_libv_gl_swizzle(res->swizzle_.g),
				to_libv_gl_swizzle(res->swizzle_.b),
				to_libv_gl_swizzle(res->swizzle_.a)
		);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
