// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/utility/memory/intrusive2_ptr.hpp>
// std
#include <string>
#include <string_view>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class TextureLoader {
private:
	libv::intrusive2_ptr<InternalTextureLoader> self;

public:
	explicit TextureLoader(std::string base_path);
	TextureLoader(const TextureLoader&) noexcept;
	TextureLoader& operator=(const TextureLoader&) & noexcept;
	TextureLoader(TextureLoader&&) noexcept;
	TextureLoader& operator=(TextureLoader&&) & noexcept;
	~TextureLoader();

public:
	Texture load(std::string_view path);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
