// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// std
#include <string>
#include <string_view>
// libv
#include <libv/utility/memory/intrusive2_ptr.hpp>
// pro
#include <libv/rev/log.hpp>
#include <libv/rev/resource/resource.hpp>
#include <libv/rev/resource/texture_internal.hxx>




#include <libv/gl/image.hpp>
#include <libv/gl/load_image.hpp>
#include <libv/glr/texture.hpp>
#include <libv/utility/read_file.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct InternalTextureLoader : BasicResourceLoader<InternalTextureLoader, InternalTexture> {
	std::string base_path;

	explicit inline InternalTextureLoader(std::string&& base_path) :
		base_path(std::move(base_path)) {}

	libv::intrusive2_ptr<InternalTexture> create_resource(std::string_view path) {
		log_rev.trace("Create new texture: {}", path);

//		switch_to_fs_thread();
		const auto dataTexture0 = libv::read_file_or_throw(base_path + '/' + std::string(path));
//		switch_to_gl_loop();
		auto imageTexture0 = libv::gl::load_image_or_throw(dataTexture0);
//		switch_to_tp();
		libv::glr::Texture texture0;
		texture0.load(std::move(imageTexture0));

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
