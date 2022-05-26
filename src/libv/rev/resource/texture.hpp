// Project: libv.rev, File: src/libv/rev/resource/texture.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/glr/fwd.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

/// Handler type
class Texture {
private:
	libv::intrusive2_ptr<InternalTexture> self;

public:
	Texture() noexcept;
	explicit Texture(libv::intrusive2_ptr<InternalTexture> self);
	Texture(const Texture&) noexcept;
	Texture& operator=(const Texture&) & noexcept;
	Texture(Texture&&) noexcept;
	Texture& operator=(Texture&&) & noexcept;
	~Texture();

public:
	[[nodiscard]] libv::glr::Texture& texture() noexcept;
	[[nodiscard]] const libv::glr::Texture& texture() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
