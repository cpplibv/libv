// File:   texture.cpp Author: Vader Created on 2019. január 25., 4:40

// hpp
#include <libv/glr/texture.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/texture.hpp>
// pro
#include <libv/glr/assert.hpp>
#include <libv/glr/remote.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

void Texture::implStorage(libv::gl::TextureTarget target, int32_t levels, libv::vec3i size, libv::gl::Format format, libv::gl::DataType type) noexcept {
	remote->dirty = true;
	remote->dirty_storage = true;

	remote->levels.reserve(levels);
	remote->target = target;
	remote->format = format;
	remote->dataType = type;

	const auto sizeInfo = libv::gl::sizeInfo(type);
	const auto pixelSize = sizeInfo.size * libv::gl::dimInfo(format.base) / sizeInfo.pack;

	remote->pixelSize = pixelSize;

	int32_t storage_size = 0;
	for (int32_t i = 0; i < levels; ++i) {
		remote->levels.emplace_back(RemoteTexture::Level{size, storage_size});
		storage_size += size.x * size.y * size.z * pixelSize;
		size = libv::vec::max(size / 2, libv::vec3i{1, 1, 1});
	}
	remote->data.resize(storage_size);
}

void Texture::load(libv::gl::Image image) noexcept {
	remote->image = std::move(image);
	remote->dirty = true;
	remote->dirty_load = true;
}

void Texture::image(int32_t level, int32_t offset, int32_t size, const void* data) noexcept {
	LIBV_GLR_DEBUG_ASSERT(level < static_cast<int32_t>(remote->levels.size()));
	LIBV_GLR_DEBUG_ASSERT(level >= 0);

	const auto target = remote->data.data()
			+ remote->levels[level].offset
			+ offset * remote->pixelSize;

	std::memcpy(target, data, size * remote->pixelSize);

	remote->dirty = true;
	remote->dirty_data = true;
	remote->dirty_dataMin = libv::vec::min(remote->dirty_dataMin, libv::vec3i(offset, 0, 0));
	remote->dirty_dataMax = libv::vec::max(remote->dirty_dataMax, libv::vec3i(offset + size, 0, 0));
}

void Texture::image(int32_t level, libv::vec2i offset, libv::vec2i size, const void* data) noexcept {
	LIBV_GLR_DEBUG_ASSERT(level < static_cast<int32_t>(remote->levels.size()));
	LIBV_GLR_DEBUG_ASSERT(level >= 0);

	if (offset == libv::vec2i{0, 0} && libv::vec3i(size, 1) == remote->levels[level].size) {
		const auto target = remote->data.data() + remote->levels[level].offset;
		std::memcpy(target, data, size.x * size.y * remote->pixelSize);

	} else {
		auto source = reinterpret_cast<const std::byte*>(data);

		for (int32_t i = 0; i < size.y; ++i) {
			const auto target = remote->data.data()
					+ remote->levels[level].offset
					+ (offset.y + i) * remote->levels[level].size.x * remote->pixelSize
					+ offset.x * remote->pixelSize;
			std::memcpy(target, source, size.x * remote->pixelSize);
			source += size.x * remote->pixelSize;
		}
	}

	remote->dirty = true;
	remote->dirty_data = true;
	remote->dirty_dataMin = libv::vec::min(remote->dirty_dataMin, libv::vec3i(offset, 0));
	remote->dirty_dataMax = libv::vec::max(remote->dirty_dataMax, libv::vec3i(offset + size, 0));
}

void Texture::image(int32_t level, libv::vec3i offset, libv::vec3i size, const void* data) noexcept {
	LIBV_GLR_DEBUG_ASSERT(level < static_cast<int32_t>(remote->levels.size()));
	LIBV_GLR_DEBUG_ASSERT(level >= 0);

	if (offset == libv::vec3i{0, 0, 0} && size == remote->levels[level].size) {
		const auto target = remote->data.data() + remote->levels[level].offset;
		std::memcpy(target, data, size.x * size.y * size.z * remote->pixelSize);

	} else {
		auto source = reinterpret_cast<const std::byte*>(data);

		for (int32_t j = 0; j < size.z; ++j) {
			for (int32_t i = 0; i < size.y; ++i) {
				const auto target = remote->data.data()
						+ remote->levels[level].offset
						+ (offset.z + j) * remote->levels[level].size.x * remote->levels[level].size.y * remote->pixelSize
						+ (offset.y + i) * remote->levels[level].size.x * remote->pixelSize
						+ offset.x * remote->pixelSize;
				std::memcpy(target, source, size.x * remote->pixelSize);
				source += size.x * remote->pixelSize;
			}
		}
	}

	remote->dirty = true;
	remote->dirty_data = true;
	remote->dirty_dataMin = libv::vec::min(remote->dirty_dataMin, offset);
	remote->dirty_dataMax = libv::vec::max(remote->dirty_dataMax, offset + size);
}

void Texture::image(int32_t level, libv::gl::CubeSide side, const void* data) noexcept {
	// Redirect to 3D image
	const auto size_i = static_cast<int32_t>(side) - static_cast<int32_t>(libv::gl::CubeSide::PositiveX);
	image(level, libv::vec3i{0, 0, size_i}, remote->levels[level].size, data);
}

void Texture::image(int32_t level, libv::gl::CubeSide side, int32_t layer, const void* data) noexcept {
	// Redirect to 3D image
	const auto size_i = static_cast<int32_t>(side) - static_cast<int32_t>(libv::gl::CubeSide::PositiveX);
	image(level, libv::vec3i{0, 0, layer * 6 + size_i}, remote->levels[level].size, data);
}

void Texture::set(libv::gl::MagFilter filter) noexcept {
	remote->dirty = true;
	remote->dirty_filter = true;
	remote->magFilter = filter;
}

void Texture::set(libv::gl::MinFilter filter) noexcept {
	remote->dirty = true;
	remote->dirty_filter = true;
	remote->minFilter = filter;
}

// -------------------------------------------------------------------------------------------------

void RemoteTexture::create(libv::gl::GL& gl, Remote& remote_) noexcept {
	(void) gl;

	remote = libv::make_observer(remote_);
	created = true;

	// TODO P5: Sort out this create mess, ideal solution would be to improve Image to accept a texture
	if (!dirty_load) {
		gl(texture).create();
		gl(texture).bind();
	}
}

void RemoteTexture::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (!created)
		create(gl, remote_);

	if (dirty_load) {
		texture = image->createTexture();
		dirty_load = false;
	}

	if (dirty_storage) {
		switch (target) {
		case libv::gl::TextureTarget::_1D:
			gl(texture).storage(static_cast<int32_t>(levels.size()), format, levels[0].size.x);
			break;
		default:
		case libv::gl::TextureTarget::_1DArray:
		case libv::gl::TextureTarget::_2D:
		case libv::gl::TextureTarget::Rectangle:
		case libv::gl::TextureTarget::CubeMap:
			gl(texture).storage(static_cast<int32_t>(levels.size()), format, levels[0].size.x, levels[0].size.y);
			break;
		case libv::gl::TextureTarget::CubeMapArray:
		case libv::gl::TextureTarget::_2DArray:
		case libv::gl::TextureTarget::_3D:
			gl(texture).storage(static_cast<int32_t>(levels.size()), format, levels[0].size.x, levels[0].size.y, levels[0].size.z);
			break;
		}
		dirty_storage = false;
	}

	if (dirty_data) {
		// TODO P1: Implement proper sub image, be mindful that due to data layout the subimage have to be extended to a full dimension
		for (int32_t i = 0; i < static_cast<int32_t>(levels.size()); ++i) {
			switch (target) {
			case libv::gl::TextureTarget::_1D:
				gl(texture).subImage(i, 0, levels[i].size.x, format.base, dataType, data.data() + levels[i].offset);
				break;
			default:
			case libv::gl::TextureTarget::_1DArray:
			case libv::gl::TextureTarget::_2D:
			case libv::gl::TextureTarget::Rectangle:
				gl(texture).subImage(i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
				break;
			case libv::gl::TextureTarget::CubeMap:
				// TODO P1: libv.glr: Implement cube map image
				LIBV_GLR_DEBUG_ASSERT(false);
//				gl(texture).subImage(libv::gl::CubeSide::PositiveX, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(texture).subImage(libv::gl::CubeSide::NegativeX, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(texture).subImage(libv::gl::CubeSide::PositiveY, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(texture).subImage(libv::gl::CubeSide::NegativeY, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(texture).subImage(libv::gl::CubeSide::PositiveZ, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(texture).subImage(libv::gl::CubeSide::NegativeZ, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
				break;
			case libv::gl::TextureTarget::_2DArray:
			case libv::gl::TextureTarget::_3D:
				gl(texture).subImage(i, 0, 0, 0, levels[i].size.x, levels[i].size.y, levels[i].size.z, format.base, dataType, data.data() + levels[i].offset);
				break;
			case libv::gl::TextureTarget::CubeMapArray:
				// TODO P1: libv.glr: Implement cube map array image
				LIBV_GLR_DEBUG_ASSERT(false);
				break;
			}
		}

		dirty_data = false;
	}

	if (dirty_filter) {
		gl(texture).setMagFilter(magFilter);
		gl(texture).setMinFilter(minFilter);
		dirty_filter = false;
	}
	dirty = false;
}

void RemoteTexture::bind(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (dirty)
		update(gl, remote_);
	else
		gl(texture).bind();
}

RemoteTexture::~RemoteTexture() noexcept {
	if (remote)
		remote->gc(texture);
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
