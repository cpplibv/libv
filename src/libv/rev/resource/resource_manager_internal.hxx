// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// std
//#include <string>
//#include <string_view>
// libv
#include <libv/utility/memory/intrusive2_ptr.hpp>
// pro
#include <libv/rev/resource/settings.hpp>






//#include <libv/rev/log.hpp>
//#include <libv/rev/resource/resource.hpp>
//#include <libv/rev/resource/texture_internal.hxx>

//#include <libv/gl/image.hpp>
//#include <libv/gl/load_image.hpp>
//#include <libv/glr/texture.hpp>
//#include <libv/utility/read_file.hpp>

//#include <libv/fsw/watcher.hpp>
//#include <libv/utility/nexus.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

//struct InternalResourceManager : BasicResourceLoader<InternalResourceManager, InternalResource> {
struct InternalResourceManager : libv::ref_count_base<InternalResourceManager> {
//	std::string base_path;

//	std::optional<>
//	libv::Nexus nexus;
//	libv::fsw::Watcher fsw;
	Settings settings;

	explicit inline InternalResourceManager(Settings&& settings) :
		settings(std::move(settings)) {}
//		base_path(std::move(base_path)) {}

//	libv::intrusive2_ptr<InternalResource> create_resource(std::string_view path) {
//		log_rev.trace("Create new texture: {}", path);
//
////		switch_to_fs_thread();
//		const auto dataResource0 = libv::read_file_or_throw(base_path + '/' + std::string(path));
////		switch_to_gl_loop();
//		auto imageResource0 = libv::gl::load_image_or_throw(dataResource0);
////		switch_to_tp();
//		libv::glr::Resource texture0;
//		texture0.load(std::move(imageResource0));
//
//		return libv::make_intrusive2_ptr<InternalResource>(this, path, texture0);
//	}

//	libv::intrusive2_ptr<InternalResource> destroy_resource(libv::intrusive2_ptr<InternalResource>) {
//	libv::intrusive2_ptr<InternalResource> destroy_resource(InternalResource& resource) {
//		log_rev.trace("Destroy texture: {}", resource.path_);
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
