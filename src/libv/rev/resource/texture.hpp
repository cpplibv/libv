// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/glr/fwd.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class Texture {
private:
	libv::intrusive2_ptr<InternalTexture> self;

public:
	explicit Texture(libv::intrusive2_ptr<InternalTexture> self);
	Texture(const Texture&) noexcept;
	Texture& operator=(const Texture&) & noexcept;
	Texture(Texture&&) noexcept;
	Texture& operator=(Texture&&) & noexcept;
	~Texture();

public:
	const libv::glr::Texture& texture() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
