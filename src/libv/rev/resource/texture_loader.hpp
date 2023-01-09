// Project: libv.rev, File: src/libv/rev/resource/texture_loader.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// std
#include <memory>
#include <string_view>
// pro
#include <libv/rev/gl/swizzle.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

/// Handler type
class TextureLoader {
private:
	std::shared_ptr<InternalTextureLoader> self;

public:
	explicit TextureLoader(const std::shared_ptr<InternalResourceManager>& irm);
	inline TextureLoader(const TextureLoader&) noexcept = default;
	inline TextureLoader& operator=(const TextureLoader&) & noexcept = default;
	inline TextureLoader(TextureLoader&&) noexcept = default;
	inline TextureLoader& operator=(TextureLoader&&) & noexcept = default;
	inline ~TextureLoader() = default;

public:
	[[nodiscard]] Texture load(std::string_view name, Swizzle swizzle = {});
	[[nodiscard]] Texture fallback() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
