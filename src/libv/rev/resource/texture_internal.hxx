// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// std
#include <string>
#include <string_view>
// libv
#include <libv/glr/texture.hpp>
#include <libv/utility/ordering.hpp>
// pro
#include "basic_resource.hpp"
#include <libv/rev/resource/texture_loader_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct InternalTexture : BasicResource<InternalTexture, InternalTextureLoader> {
	std::string path_;
	libv::glr::Texture texture;

	explicit inline InternalTexture(Loader* loader, std::string_view path, libv::glr::Texture texture) :
		Base(loader),
		path_(path),
		texture(std::move(texture)) {}

	[[nodiscard]] inline int compare(std::string_view path) const noexcept {
		if (auto v = path_ <=> path; v != 0) return libv::ordering_as_int(v);

		return 0;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
