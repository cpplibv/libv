// File: program.hpp - Created on 2017.12.20. 11:27 - Author: Vader

#pragma once

// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/utility/observer_ptr.hpp>
// std
#include <memory>
#include <optional>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/image.hpp>
#include <libv/gl/texture_object.hpp>


namespace libv {
namespace gl {

class GL;

} // namespace gl
namespace glr {

// -------------------------------------------------------------------------------------------------

class Remote;
class AttorneyTextureRemote;

// -------------------------------------------------------------------------------------------------

struct RemoteTexture {
	libv::gl::Texture texture;

	bool created = false;
	bool dirty = false;
	bool dirty_storage = false;
	bool dirty_load = false;
	bool dirty_data = false;
	bool dirty_filter = false;

	//int32_t ref_count = 0;

	// Image
	std::optional<libv::gl::Image> image;

	// Storage
	struct Level {
		libv::vec3i size;
		int32_t offset;
	};
	boost::container::small_vector<Level, 1> levels;
	libv::gl::TextureTarget target;
	libv::gl::Format format;
	libv::gl::DataType dataType;
	int32_t pixelSize;

	// Data
	libv::vec3i dirty_dataMin = {0, 0, 0};
	libv::vec3i dirty_dataMax = {0, 0, 0};
	std::vector<std::byte> data;

	// Properties
	libv::gl::MagFilter magFilter = libv::gl::MagFilter::Linear;
	libv::gl::MinFilter minFilter = libv::gl::MinFilter::NearestMipmapLinear;
//	libv::gl::Wrap warpS = libv::gl::Wrap::Repeat;
//	libv::gl::Wrap warpT = libv::gl::Wrap::Repeat;
//	libv::gl::Wrap warpR = libv::gl::Wrap::Repeat;

private:
	/// Can be nullptr if the object is not yet associated with a remote OpenGL context
	libv::observer_ptr<Remote> remote;

private:
	void create(libv::gl::GL& gl, Remote& remote_) noexcept;
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	void bind(libv::gl::GL& gl, Remote& remote_) noexcept;
	~RemoteTexture() noexcept;
};

// =================================================================================================

class Texture {
	friend class AttorneyTextureRemote;

private:
	std::shared_ptr<RemoteTexture> remote;

public:
	Texture() noexcept :
		remote(std::make_shared<RemoteTexture>()) { }

private:
	void implStorage(libv::gl::TextureTarget target, int32_t levels, libv::vec3i size, libv::gl::Format format, libv::gl::DataType type) noexcept;

public:
	inline void storage(libv::gl::TextureTarget target, int32_t levels, int32_t size, libv::gl::Format format, libv::gl::DataType type) noexcept {
		implStorage(target, levels, {size, 1, 1}, format, type);
	}

	inline void storage(libv::gl::TextureTarget target, int32_t levels, libv::vec2i size, libv::gl::Format format, libv::gl::DataType type) noexcept {
		implStorage(target, levels, {size, 1}, format, type);
	}

	inline void storage(libv::gl::TextureTarget target, int32_t levels, libv::vec3i size, libv::gl::Format format, libv::gl::DataType type) noexcept {
		implStorage(target, levels, size, format, type);
	}

public:
	void load(libv::gl::Image image) noexcept;

	void image(int32_t level, int32_t offset, int32_t size, const void* data) noexcept;
	void image(int32_t level, libv::vec2i offset, libv::vec2i size, const void* data) noexcept;
	void image(int32_t level, libv::vec3i offset, libv::vec3i size, const void* data) noexcept;
	void image(int32_t level, libv::gl::CubeSide side, const void* data)  noexcept;
	void image(int32_t level, libv::gl::CubeSide side, int32_t layer, const void* data) noexcept;

//	void* at(int32_t level, libv::vec3i position) noexcept;

public:
	void set(libv::gl::MagFilter filter) noexcept;
	void set(libv::gl::MinFilter filter) noexcept;
};

class AttorneyTextureRemote {
	friend class Queue;
	static inline RemoteTexture& remote(const Texture& texture) noexcept {
		return *texture.remote;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename Params>
struct Texture1D_t : Texture {
	using storage_type = typename Params::storage_type;

	inline void storage(int32_t levels, int32_t size) noexcept {
		static_cast<Texture&>(*this).storage(Params::target, levels, size, Params::format, Params::dataType);
	}
	inline void image(int32_t level, int32_t offset, int32_t size, const storage_type* data) noexcept {
		static_cast<Texture&>(*this).image(level, offset, size, data);
	}
};

template <typename Params>
struct Texture2D_t : Texture {
	using storage_type = typename Params::storage_type;

	inline void storage(int32_t levels, libv::vec2i size) noexcept {
		static_cast<Texture&>(*this).storage(Params::target, levels, size, Params::format, Params::dataType);
	}
	inline void image(int32_t level, libv::vec2i offset, libv::vec2i size, const storage_type* data) noexcept {
		static_cast<Texture&>(*this).image(level, offset, size, data);
	}
};

template <typename Params>
struct Texture2DRect_t : Texture {
	using storage_type = typename Params::storage_type;

	inline void storage(libv::vec2i size) noexcept {
		static_cast<Texture&>(*this).storage(Params::target, 1, size, Params::format, Params::dataType);
	}
	inline void image(libv::vec2i offset, libv::vec2i size, const storage_type* data) noexcept {
		static_cast<Texture&>(*this).image(0, offset, size, data);
	}
};

template <typename Params>
struct Texture2DCube_t : Texture {
	using storage_type = typename Params::storage_type;

	inline void storage(int32_t levels, libv::vec2i size) noexcept {
		static_cast<Texture&>(*this).storage(Params::target, levels, size, Params::format, Params::dataType);
	}
	inline void image(int32_t level, libv::gl::CubeSide side, const storage_type* data) noexcept {
		static_cast<Texture&>(*this).image(level, side, data);
	}
};

template <typename Params>
struct Texture3D_t : Texture {
	using storage_type = typename Params::storage_type;

	inline void storage(int32_t levels, libv::vec3i size) noexcept {
		static_cast<Texture&>(*this).storage(Params::target, levels, size, Params::format, Params::dataType);
	}
	inline void image(int32_t level, libv::vec3i offset, libv::vec3i size, const storage_type* data) noexcept {
		static_cast<Texture&>(*this).image(level, offset, size, data);
	}
};

template <typename Params>
struct Texture3DCube_t : Texture {
	using storage_type = typename Params::storage_type;

	inline void storage(int32_t levels, libv::vec3i size) noexcept{
		static_cast<Texture&>(*this).storage(Params::target, levels, size, Params::format, Params::dataType);
	}
	inline void image(int32_t level, libv::gl::CubeSide side, int32_t layer, const storage_type* data) noexcept {
		static_cast<Texture&>(*this).image(level, side, layer, data);
	}
};

// -------------------------------------------------------------------------------------------------

template <template <typename> class TS, libv::gl::TextureTarget Target>
struct TextureFormats_t {
private:
	template <auto Format, libv::gl::DataType DataType, typename Texel>
	struct Params {
		static constexpr libv::gl::TextureTarget target = Target;
		static constexpr libv::gl::Format format = {Format};
		static constexpr libv::gl::DataType dataType = DataType;
		using storage_type = Texel;
	};

	using FS = libv::gl::FormatSized;
	using FC = libv::gl::FormatCompressed;
	using FD = libv::gl::FormatDepth;

public:
	using R8          = TS<Params<FS::R8,                 libv::gl::DataType::U8, uint8_t>>;
	using R8_G8       = TS<Params<FS::RG8,                libv::gl::DataType::U8, libv::vec2uc>>;
	using R8_G8_B8    = TS<Params<FS::RGB8,               libv::gl::DataType::U8, libv::vec3uc>>;
	using R8_G8_B8_A8 = TS<Params<FS::RGBA8,              libv::gl::DataType::U8, libv::vec4uc>>;
//	using R16F        = TS<Params<FS::R16F,               libv::gl::DataType::F16, short float>>; // C++20: short float
	using R16F        = TS<Params<FS::R16F,               libv::gl::DataType::F32, float>>;
	using R32F        = TS<Params<FS::R32F,               libv::gl::DataType::F32, float>>;

	using RGB_DXT1    = TS<Params<FC::RGB_S3TC_DXT1_EXT,  libv::gl::DataType::U8, libv::vec3uc>>;
	using RGBA_DXT1   = TS<Params<FC::RGBA_S3TC_DXT1_EXT, libv::gl::DataType::U8, libv::vec4uc>>;
	using RGBA_DXT3   = TS<Params<FC::RGBA_S3TC_DXT3_EXT, libv::gl::DataType::U8, libv::vec4uc>>;
	using RGBA_DXT5   = TS<Params<FC::RGBA_S3TC_DXT5_EXT, libv::gl::DataType::U8, libv::vec4uc>>;

	using D16         = TS<Params<FD::DEPTH_COMPONENT16,  libv::gl::DataType::U16, uint16_t>>;
	using D24         = TS<Params<FD::DEPTH_COMPONENT24,  libv::gl::DataType::U32, int32_t>>;
	using D32         = TS<Params<FD::DEPTH_COMPONENT32,  libv::gl::DataType::U32, int32_t>>;
	using D32F        = TS<Params<FD::DEPTH_COMPONENT32F, libv::gl::DataType::F32, float>>;
};

using Texture1D        = TextureFormats_t<Texture1D_t,     libv::gl::TextureTarget::_1D>;
using Texture1DArray   = TextureFormats_t<Texture2D_t,     libv::gl::TextureTarget::_1DArray>;
using Texture2D        = TextureFormats_t<Texture2D_t,     libv::gl::TextureTarget::_2D>;
using TextureRect      = TextureFormats_t<Texture2DRect_t, libv::gl::TextureTarget::Rectangle>;
using TextureCube      = TextureFormats_t<Texture2DCube_t, libv::gl::TextureTarget::CubeMap>;
using Texture2DArray   = TextureFormats_t<Texture3D_t,     libv::gl::TextureTarget::_2DArray>;
using Texture3D        = TextureFormats_t<Texture3D_t,     libv::gl::TextureTarget::_3D>;
using TextureCubeArray = TextureFormats_t<Texture3DCube_t, libv::gl::TextureTarget::CubeMapArray>;

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
