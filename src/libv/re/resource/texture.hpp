// Project: libv.re, File: src/libv/re/resource/texture.hpp

#pragma once

#include <libv/gl/enum_fwd.hpp>
#include <libv/gl/fwd.hpp>
#include <libv/gl/image_fwd.hpp>
#include <libv/math/vec_fwd.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/core/safe_gl.hpp>
#include <libv/re/resource/common_resource.hpp>
#include <libv/re/common/texture_unit.hpp>

#include <atomic>
#include <memory>
#include <string>
#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct ColdTextureData;

class Texture : public libv::ref_count_base {
public:
	friend libv::ref_count_access;
	friend ResourceAccess;
	friend BasicResource;
	using ptr = Texture_ptr;
	using cptr = Texture_cptr;

private:
	std::atomic_bool dirty = false;
	// OpenGL handler - GL Thread access only
	GLSafeTexture texture;
	// Cold data
	const std::unique_ptr<ColdTextureData> cold;

public:
	/// @thread any
	explicit Texture(std::string identifier, VariantID variant = VariantID::Default);
	/// @thread any
	Texture(std::string_view identifier, VariantID variant, CachedTag, bool delayed, const Texture_ptr& fallback);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) & = delete;
	Texture(Texture&&) = delete;
	Texture& operator=(Texture&&) & = delete;
private:
	void ref_count_one();
public:
	/// @thread any
	~Texture();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(std::string identifier = "noname") {
		return ptr::make(std::move(identifier));
	}

public:
	/// Not an OpenGL ID, it is a stable C++ Unique ID that can be used as key
	/// @thread any
	[[nodiscard]] TextureID id() const noexcept {
		return std::bit_cast<TextureID>(this);
	}
	/// @thread any
	[[nodiscard]] const std::string& identifier() const noexcept;
	/// @thread any
	[[nodiscard]] VariantID variant() const noexcept;
	/// @thread any
	[[nodiscard]] int currentVersion() const noexcept;
	/// @thread any
	[[nodiscard]] int loadVersion() const noexcept;

	/// @thread any
	[[nodiscard]] std::size_t hash() const noexcept;
	/// @thread any
	[[nodiscard]] bool operator==(const Texture& rhs) const noexcept;

private:
	void _markDirty();
	[[nodiscard]] libv::vec3i _imageSize(int32_t level) const noexcept;
	void implStorage(libv::gl::TextureTarget target, libv::gl::Format format, int32_t levels, libv::vec3i size, int32_t sampleCount, bool sampleFixed) noexcept;

public:
	/// @thread any
	void storage(libv::gl::TextureTarget target, libv::gl::Format format, int32_t levels, int32_t size) noexcept;
	/// @thread any
	void storage(libv::gl::TextureTarget target, libv::gl::Format format, int32_t levels, libv::vec2i size, int32_t sampleCount = 0, bool sampleFixed = false) noexcept;
	/// @thread any
	void storage(libv::gl::TextureTarget target, libv::gl::Format format, int32_t levels, libv::vec3i size, int32_t sampleCount = 0, bool sampleFixed = false) noexcept;

	/// \c data is copied into internal storage
	/// @thread any
	void image(int32_t level, int32_t offset, int32_t size, libv::gl::DataType type, const std::byte* data) noexcept;
	/// \c data is copied into internal storage
	/// @thread any
	void image(int32_t level, libv::vec2i offset, libv::vec2i size, libv::gl::DataType type, const std::byte* data) noexcept;
	/// \c data is copied into internal storage
	/// @thread any
	void image(int32_t level, libv::vec3i offset, libv::vec3i size, libv::gl::DataType type, const std::byte* data) noexcept;
	/// \c data is copied into internal storage
	/// @thread any
	void image(int32_t level, libv::gl::CubeSide side, libv::gl::DataType type, const std::byte* data)  noexcept;
	/// \c data is copied into internal storage
	/// @thread any
	void image(int32_t level, libv::gl::CubeSide side, int32_t layer, libv::gl::DataType type, const std::byte* data) noexcept;

	/// GL Image upload is mutually exclusive with any other storage function usage
	/// @thread any
	void image(libv::gl::Image&& image) noexcept;

	/// @thread any
	void set(libv::gl::MagFilter filter) noexcept;
	/// @thread any
	void set(libv::gl::MinFilter filter) noexcept;
	/// @thread any
	void set(libv::gl::Wrap warpS) noexcept;
	/// @thread any
	void set(libv::gl::Wrap warpS, libv::gl::Wrap warpT) noexcept;
	/// @thread any
	void set(libv::gl::Wrap warpS, libv::gl::Wrap warpT, libv::gl::Wrap warpR) noexcept;
	/// @thread any
	void set(libv::gl::Swizzle r, libv::gl::Swizzle g, libv::gl::Swizzle b, libv::gl::Swizzle a) noexcept;

	/// @thread any
	void set(libv::gl::DepthStencilMode mode) noexcept;

	/// Set swizzle channels parsed from the string provided
	/// The string length must be 4 and contain only "rgba01" characters.
	/// If the string does not match the expected format it will have no effect and an error log will be produced
	/// @thread any
	void swizzle(std::string_view rgba) noexcept;

	/// Runtime mipmap generation is not possible for block compressed, rectangle, buffer or multisample textures
	/// @thread any
	void generateMipmaps() noexcept;

public:
	[[nodiscard]] const libv::gl::Format& format() const noexcept;
	/// Missing dimension sizes are stored as zero
	[[nodiscard]] libv::vec3i size() const noexcept;

private:
	void requestLoad(LoadingMode mode) noexcept;
	void requestActivatingLoad(LoadingMode mode) noexcept;
	void recordLoadSuccess() noexcept;
	void recordLoadFailure() noexcept;
	void recordUnloadFromCache() noexcept;

	void _gl_update(libv::GL& gl);

public:
	/// @thread GL
	void gl_bind(libv::GL& gl, TextureUnit unit);
	/// @thread GL
	void gl_bindImage(libv::GL& gl, TextureUnit unit, uint32_t level, libv::gl::BufferAccessFull access);
	/// @thread GL
	[[nodiscard]] libv::gl::Texture gl_id(libv::GL& gl);
	/// @thread GL
	void gl_destroy(libv::GL& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
