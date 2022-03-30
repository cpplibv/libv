// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// std
#include <string>
#include <string_view>
// libv
#include <libv/utility/memory/intrusive2_ptr.hpp>
// pro
#include <libv/rev/log.hpp>
#include <libv/rev/resource/basic_resource.hpp>
#include <libv/rev/resource/resource_manager_internal.hxx>
#include <libv/rev/resource/texture_internal.hxx>



#include <libv/gl/image.hpp>
#include <libv/gl/load_image.hpp>
#include <libv/glr/texture.hpp>
#include <libv/utility/read_file.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct InternalTextureLoader : BasicResourceLoader<InternalTextureLoader, InternalTexture> {
	libv::intrusive2_ptr<InternalResourceManager> irm;
	std::string base_path;

	explicit inline InternalTextureLoader(const libv::intrusive2_ptr<InternalResourceManager>& irm) :
		irm(irm),
		base_path(irm->settings.texture.base_path) {

//		add_virtual_resource("builtin:up", libv::vec2i{1, 1}, libv::vec4uc{127, 127, 255, 255});
//		add_virtual_resource("builtin:down", libv::vec2i{1, 1}, libv::vec4uc{127, 127, 0, 255});
//		add_virtual_resource("builtin:black", libv::vec2i{1, 1}, libv::vec4uc{0, 0, 0, 255});
//		add_virtual_resource("builtin:white", libv::vec2i{1, 1}, libv::vec4uc{255, 255, 255, 255});
//		add_virtual_resource("builtin:gray", libv::vec2i{1, 1}, libv::vec4uc{127, 127, 127, 255});
//		add_virtual_resource("builtin:transparent", libv::vec2i{1, 1}, libv::vec4uc{0, 0, 0, 0});

//		add_virtual_resource("lut:ace", ...);
//		add_virtual_resource("lut:natural", ...);
//		add_virtual_resource("lut:tonemap", ...);
	}

	libv::intrusive2_ptr<InternalTexture> create_resource(std::string_view path) {
		log_rev.trace("Create new texture: {}", path);

		const auto resource_path = base_path + std::string(path);
//		fs_thread();
		const auto dataTexture0 = libv::read_file_or_throw(resource_path);
//		gl_loop();
		auto imageTexture0 = libv::gl::load_image_or_throw(dataTexture0);
//		any_thread();
		libv::glr::Texture texture0;
		texture0.load(std::move(imageTexture0));

//		irm->fsw.subscribe_file(resource_path, [] {
//
//		});

		return libv::make_intrusive2_ptr<InternalTexture>(this, path, texture0);
	}

//	libv::intrusive2_ptr<InternalTexture> destroy_resource(libv::intrusive2_ptr<InternalTexture>) {
//	libv::intrusive2_ptr<InternalTexture> destroy_resource(InternalTexture& resource) {
//		log_rev.trace("Destroy texture: {}", resource.path_);
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
