// File:   texture.cpp Author: Vader Created on 2019. január 25., 4:40

// hpp
#include <libv/glr/texture.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/image.hpp>
#include <libv/gl/texture.hpp>
#include <libv/utility/observer_ptr.hpp>
// std
#include <optional>
// pro
#include <libv/glr/assert.hpp>
#include <libv/glr/destroy_queue.hpp>
#include <libv/glr/remote.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct RemoteTexture {
	RemoteTextureHead head; // NOTE: head must be the first member to support inline aliasing access

	bool dirty_storage = false;
	bool dirty_storage_ms = false;
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
	libv::gl::Format format;
	libv::gl::DataType dataType;
	int32_t pixelSize;

	// Multisample
	int32_t samples;
	bool fixedSamples;

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

	libv::observer_ptr<DestroyQueues> remote = nullptr;

public:
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	~RemoteTexture() noexcept;
};

// -------------------------------------------------------------------------------------------------

void RemoteTexture::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (remote == nullptr) {
		remote = remote_.destroyQueues();

		if (!dirty_load) {
			// TODO P5: Sort out this create mess, ideal solution would be to improve Image to accept a texture
			gl(head.texture).create();
			gl(head.texture).bind();
		}
	}

	if (dirty_load) {
		head.texture = image->createTexture();
		dirty_load = false;
	}

	if (dirty_storage) {
		gl(head.texture).bind();

		switch (head.texture.target) {
		case libv::gl::TextureTarget::_1D:
			gl(head.texture).storage(static_cast<int32_t>(levels.size()), format, levels[0].size.x);
			break;
		default: // TODO P5: libv.gl: instead of default log and assert invalid input
		case libv::gl::TextureTarget::_1DArray:
		case libv::gl::TextureTarget::_2D:
		case libv::gl::TextureTarget::Rectangle:
		case libv::gl::TextureTarget::CubeMap:
			gl(head.texture).storage(static_cast<int32_t>(levels.size()), format, levels[0].size.x, levels[0].size.y);
			break;
		case libv::gl::TextureTarget::CubeMapArray:
		case libv::gl::TextureTarget::_2DArray:
		case libv::gl::TextureTarget::_3D:
			gl(head.texture).storage(static_cast<int32_t>(levels.size()), format, levels[0].size.x, levels[0].size.y, levels[0].size.z);
			break;
		}
		dirty_storage = false;
	}

	if (dirty_storage_ms) {
		gl(head.texture).bind();

		if (head.texture.target == libv::gl::TextureTarget::_2DMultisample)
			gl(head.texture).storage_ms(format, libv::vec::xy(levels[0].size), samples, fixedSamples);
		else if (head.texture.target == libv::gl::TextureTarget::_2DMultisampleArray)
			gl(head.texture).storage_ms(format, levels[0].size, samples, fixedSamples);

		dirty_storage_ms = false;
	}

	if (dirty_data) {
		gl(head.texture).bind();
		// TODO P1: Implement proper sub image, be mindful that due to data layout the subimage have to be extended to secondary and tertiary dimension
		for (int32_t i = 0; i < static_cast<int32_t>(levels.size()); ++i) {
			switch (head.texture.target) {
			case libv::gl::TextureTarget::_1D:
				gl(head.texture).subImage(i, 0, levels[i].size.x, format.base, dataType, data.data() + levels[i].offset);
				break;
			case libv::gl::TextureTarget::_1DArray:
			case libv::gl::TextureTarget::_2D:
			case libv::gl::TextureTarget::Rectangle:
				gl(head.texture).subImage(i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
				break;
			case libv::gl::TextureTarget::CubeMap:
				// TODO P3: libv.glr: Implement cube map image
				LIBV_GLR_DEBUG_ASSERT(false && "Not implemented yet");
//				gl(head.texture).subImage(libv::gl::CubeSide::PositiveX, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(head.texture).subImage(libv::gl::CubeSide::NegativeX, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(head.texture).subImage(libv::gl::CubeSide::PositiveY, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(head.texture).subImage(libv::gl::CubeSide::NegativeY, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(head.texture).subImage(libv::gl::CubeSide::PositiveZ, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
//				gl(head.texture).subImage(libv::gl::CubeSide::NegativeZ, i, 0, 0, levels[i].size.x, levels[i].size.y, format.base, dataType, data.data() + levels[i].offset);
				break;
			case libv::gl::TextureTarget::_2DArray:
			case libv::gl::TextureTarget::_3D:
				gl(head.texture).subImage(i, 0, 0, 0, levels[i].size.x, levels[i].size.y, levels[i].size.z, format.base, dataType, data.data() + levels[i].offset);
				break;
			case libv::gl::TextureTarget::CubeMapArray:
				// TODO P3: libv.glr: Implement cube map array image
				LIBV_GLR_DEBUG_ASSERT(false && "Not implemented yet");
				break;
			default:
				LIBV_GLR_DEBUG_ASSERT(false && "Not implemented yet or invalid input");
				break;
			}
		}

		dirty_data = false;
		dirty_dataMax = libv::vec3i{};
		dirty_dataMin = levels[0].size;
	}

	if (dirty_filter) {
		gl(head.texture).bind();
		gl(head.texture).setMagFilter(magFilter);
		gl(head.texture).setMinFilter(minFilter);
		dirty_filter = false;
	}

	head.dirty = false;
}

RemoteTexture::~RemoteTexture() noexcept {
	if (remote)
		remote->gc(head.texture);
}

// -------------------------------------------------------------------------------------------------

Texture::Texture() noexcept :
	remote(std::make_shared<RemoteTexture>()) {
}

Texture::Texture(std::shared_ptr<RemoteTexture> remote) noexcept :
	remote(std::move(remote)) {
}

void Texture::implStorage(libv::gl::TextureTarget target, int32_t levels, libv::vec3i size, libv::gl::Format format, libv::gl::DataType type) noexcept {
	remote->head.dirty = true;
	remote->dirty_storage = true;

	remote->levels.reserve(levels);
	remote->head.texture.target = target;
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

void Texture::storage_ms(libv::gl::TextureTarget target, libv::gl::Format format, libv::vec2i size, int32_t samples, bool fixedSamples) noexcept {
	LIBV_GLR_DEBUG_ASSERT(target == libv::gl::TextureTarget::_2DMultisample);

	remote->head.dirty = true;
	remote->dirty_storage_ms = true;

	remote->head.texture.target = target;
	remote->format = format;
	remote->levels.emplace_back(RemoteTexture::Level{{size, 0}, 0});
	remote->samples = samples;
	remote->fixedSamples = fixedSamples;
}

void Texture::storage_ms(libv::gl::TextureTarget target, libv::gl::Format format, libv::vec3i size, int32_t samples, bool fixedSamples) noexcept {
	LIBV_GLR_DEBUG_ASSERT(target == libv::gl::TextureTarget::_2DMultisampleArray);

	remote->head.dirty = true;
	remote->dirty_storage_ms = true;

	remote->head.texture.target = target;
	remote->format = format;
	remote->levels.emplace_back(RemoteTexture::Level{size, 0});
	remote->samples = samples;
	remote->fixedSamples = fixedSamples;
}

void Texture::load(libv::gl::Image image) noexcept {
	remote->image = std::move(image);
	remote->head.dirty = true;
	remote->dirty_load = true;
}

void Texture::image(int32_t level, int32_t offset, int32_t size, const void* data) noexcept {
	LIBV_GLR_DEBUG_ASSERT(level < static_cast<int32_t>(remote->levels.size()));
	LIBV_GLR_DEBUG_ASSERT(level >= 0);

	const auto target = remote->data.data()
			+ remote->levels[level].offset
			+ offset * remote->pixelSize;

	std::memcpy(target, data, size * remote->pixelSize);

	remote->head.dirty = true;
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

	remote->head.dirty = true;
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

	remote->head.dirty = true;
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
	remote->head.dirty = true;
	remote->dirty_filter = true;
	remote->magFilter = filter;
}

void Texture::set(libv::gl::MinFilter filter) noexcept {
	remote->head.dirty = true;
	remote->dirty_filter = true;
	remote->minFilter = filter;
}

void Texture::sync_no_bind(libv::gl::GL& gl, Remote& remote_) const noexcept {
	auto previous = gl.bound_texture(remote->head.texture.target);
	remote->update(gl, remote_);
	gl.bind(previous);
}

void Texture::sync_might_bind(libv::gl::GL& gl, Remote& remote_) const noexcept {
	remote->update(gl, remote_);
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
