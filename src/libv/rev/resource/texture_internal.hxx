// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/glr/texture.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
#include <libv/utility/ordering.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/rev/gl/swizzle.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class InternalTexture : public libv::ref_count_base<InternalTexture> {
	friend libv::ref_count_access;

public:
	libv::glr::Texture texture;
	Swizzle swizzle_;

	std::string name_;

	std::weak_ptr<InternalTextureLoader> loader_;

public:
	explicit inline InternalTexture(std::weak_ptr<InternalTextureLoader> loader, std::string name, libv::glr::Texture texture, Swizzle swizzle) :
		texture(std::move(texture)),
		swizzle_(std::move(swizzle)),
		name_(std::move(name)),
		loader_(std::move(loader)) {}

	[[nodiscard]] inline int compare(std::string_view name, Swizzle swizzle) const noexcept {
		if (const auto v = name_ <=> name; v != 0) return libv::ordering_as_int(v);
		if (const auto v = swizzle_ <=> swizzle; v != 0) return libv::ordering_as_int(v);

		return 0;
	}

private:
	void ref_count_one() noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
