// Project: libv.re, File: src/libv/re/resource/texture.cpp

#include <libv/container/dyn_array.hpp>
#include <libv/container/small_vector.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/image.hpp>
#include <libv/gl/texture.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/basic_resource.hxx>
#include <libv/re/resource/texture.hpp>
#include <libv/re/resource/texture_loader.hpp>
#include <libv/utility/hash.hpp>

#include <forward_list>
#include <mutex>
#include <optional>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Texture* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Texture* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

struct ColdTextureData {
	struct ImageDataChunk {
		int32_t level = 0;
		libv::vec3i offset;
		libv::vec3i size;
		libv::gl::DataType dataType = libv::gl::DataType::U8;
		libv::dyn_array<std::byte> data;
	};

	std::mutex mutex;
	Texture_ptr fallback = nullptr;
	bool created = false;
	bool dirtyStorage = false;
	bool dirtyData = false;
	bool dirtyFilter = false;
	bool dirtyWrap1 = false;
	bool dirtyWrap2 = false;
	bool dirtyWrap3 = false;
	bool dirtySwizzle = false;
	bool dirtyDepthStencilMode = false;
	bool dirtyMipmaps = false;

	bool cached = false;

	// Immutable key
	const std::string identifier;
	const VariantID variant;

	// Loading state
	LoadingState loadingState = LoadingState::standby;
	int currentVersion = -1; /// Value -1 means loading never succeeded, using fallback texture instead
	int loadVersion = -1;

	// Storage / Format
	libv::gl::Format format = libv::gl::FormatSized::RGBA8;
	int numLevels = 0;
	libv::vec3i size; // Missing dimension sizes are stored as zero

	// Multisample
	int32_t sampleCount = 0; // not set for ss textures
	bool sampleFixed = false; // not set for ss textures

	// Upload Data
	std::optional<libv::gl::Image> uploadImage;
	std::forward_list<ImageDataChunk> uploadDataQueue;

	// Properties
	libv::gl::MagFilter magFilter = libv::gl::MagFilter::Linear;
	libv::gl::MinFilter minFilter = libv::gl::MinFilter::NearestMipmapLinear;
	libv::gl::Wrap wrapS = libv::gl::Wrap::Repeat;
	libv::gl::Wrap wrapT = libv::gl::Wrap::Repeat;
	libv::gl::Wrap wrapR = libv::gl::Wrap::Repeat;
	libv::gl::Swizzle swizzleR = libv::gl::Swizzle::Red;
	libv::gl::Swizzle swizzleG = libv::gl::Swizzle::Green;
	libv::gl::Swizzle swizzleB = libv::gl::Swizzle::Blue;
	libv::gl::Swizzle swizzleA = libv::gl::Swizzle::Alpha;
	libv::gl::DepthStencilMode depthStencilMode = libv::gl::DepthStencilMode::Depth;

public:
	ColdTextureData(bool cached, LoadingState initState, std::string&& identifier, VariantID variant, const Texture_ptr& fallback) :
		fallback(fallback),
		cached(cached),
		identifier(std::move(identifier)),
		variant(variant),
		loadingState(initState) { }

	template <typename... Args>
	void queueData(Args&&... args) {
		auto last = uploadDataQueue.before_begin();
		auto it = last;
		while (++it != uploadDataQueue.end())
			last = it;
		uploadDataQueue.emplace_after(last, std::forward<Args>(args)...);
	}
};

// -------------------------------------------------------------------------------------------------

Texture::Texture(std::string path, VariantID variant) :
	cold(std::make_unique<ColdTextureData>(false, LoadingState::standby, std::move(path), variant, nullptr)) { }

Texture::Texture(std::string_view path, VariantID variant, CachedTag, bool delayed, const Texture_ptr& fallback) :
	dirty(fallback != nullptr),
	cold(std::make_unique<ColdTextureData>(
			true,
			delayed ? LoadingState::delayed : LoadingState::standby,
			std::string(path),
			variant,
			fallback)) {
}

void Texture::ref_count_one() {
	BasicResource::impl_ref_count_one(this, r.texture);
}

Texture::~Texture() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

const std::string& Texture::identifier() const noexcept {
	return cold->identifier;
}
VariantID Texture::variant() const noexcept {
	return cold->variant;
}
int Texture::currentVersion() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->currentVersion;
}
int Texture::loadVersion() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->loadVersion;
}

// -------------------------------------------------------------------------------------------------

std::size_t Texture::hash() const noexcept {
	// NOTE: This hash function MUST BE kept in sync with TextureKey.hash()
	return libv::hash_combine(cold->identifier, cold->variant);
}

bool Texture::operator==(const Texture& rhs) const noexcept {
	const auto& lhs = *this;
	return lhs.cold->identifier == rhs.cold->identifier &&
			lhs.cold->variant == rhs.cold->variant;
}

// -------------------------------------------------------------------------------------------------

void Texture::_markDirty() {
	dirty.store(true, std::memory_order_release);
}

libv::vec3i Texture::_imageSize(int32_t level) const noexcept {
	const auto dim01Mask = libv::vec3i(
		cold->size.x > 0 ? 1 : 0,
		cold->size.y > 0 ? 1 : 0,
		cold->size.z > 0 ? 1 : 0
	);
	const auto levelSize = libv::vec3i(
		cold->size.x >> level,
		cold->size.y >> level,
		cold->size.z >> level
	);
	return libv::vec::max(levelSize, dim01Mask);
}

void Texture::implStorage(libv::gl::TextureTarget target, libv::gl::Format format, int32_t levels, libv::vec3i size, int32_t sampleCount, bool sampleFixed) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (texture.target == target &&
			cold->format == format &&
			cold->numLevels == levels &&
			cold->size == size &&
			cold->sampleCount == sampleCount &&
			cold->sampleFixed == sampleFixed)
		return;

	_markDirty();
	cold->dirtyStorage = true;

	texture.target = target;
	cold->format = format;
	cold->numLevels = levels;
	cold->size = size;
	cold->sampleCount = sampleCount;
	cold->sampleFixed = sampleFixed;
}

void Texture::storage(libv::gl::TextureTarget target, libv::gl::Format format, int32_t levels, int32_t size) noexcept {
	LIBV_RE_ASSERT(target == libv::gl::TextureTarget::_1D);
	implStorage(target, format, levels, {size, 0, 0}, 0, false);
}

void Texture::storage(libv::gl::TextureTarget target, libv::gl::Format format, int32_t levels, libv::vec2i size, int32_t sampleCount, bool sampleFixed) noexcept {
	LIBV_RE_ASSERT(sampleCount <= 1 || levels == 1);
	LIBV_RE_ASSERT(sampleCount <= 1 || target == libv::gl::TextureTarget::_2DMultisample);
	LIBV_RE_ASSERT(sampleCount > 1 || target == libv::gl::TextureTarget::_1DArray || target == libv::gl::TextureTarget::_2D || target == libv::gl::TextureTarget::Rectangle || target == libv::gl::TextureTarget::CubeMap);
	implStorage(target, format, levels, {size, 0}, sampleCount, sampleFixed);
}

void Texture::storage(libv::gl::TextureTarget target, libv::gl::Format format, int32_t levels, libv::vec3i size, int32_t sampleCount, bool sampleFixed) noexcept {
	LIBV_RE_ASSERT(sampleCount <= 1 || levels == 1);
	LIBV_RE_ASSERT(sampleCount <= 1 || target == libv::gl::TextureTarget::_2DMultisampleArray);
	LIBV_RE_ASSERT(sampleCount > 1 || target == libv::gl::TextureTarget::_2DArray || target == libv::gl::TextureTarget::CubeMapArray || target == libv::gl::TextureTarget::_3D);
	implStorage(target, format, levels, size, sampleCount, sampleFixed);
}

void Texture::image(int32_t level, int32_t offset, int32_t size, libv::gl::DataType type, const std::byte* data) noexcept {
	image(level, libv::vec3i(offset, 0, 0), libv::vec3i(size, 0, 0), type, data);
}

void Texture::image(int32_t level, libv::vec2i offset, libv::vec2i size, libv::gl::DataType type, const std::byte* data) noexcept {
	image(level, libv::vec3i(offset, 0), libv::vec3i(size, 0), type, data);
}

void Texture::image(int32_t level, libv::vec3i offset, libv::vec3i size, libv::gl::DataType type, const std::byte* data) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	LIBV_RE_ASSERT(level < cold->numLevels);
	LIBV_RE_ASSERT(level >= 0);
	LIBV_RE_ASSERT(offset.x >= 0);
	LIBV_RE_ASSERT(offset.y >= 0);
	LIBV_RE_ASSERT(offset.z >= 0);
	LIBV_RE_ASSERT(offset.x + size.x <= cold->size.x);
	LIBV_RE_ASSERT(offset.y + size.y <= cold->size.y);
	LIBV_RE_ASSERT(offset.z + size.z <= cold->size.z);
	LIBV_RE_ASSERT(size.x >= 0);
	LIBV_RE_ASSERT(size.y >= 0);
	LIBV_RE_ASSERT(size.z >= 0);

	const auto formatDim = libv::gl::dimInfo(cold->format.base);
	const auto dataTypeInfo = libv::gl::sizeInfo(type);

	LIBV_RE_ASSERT(formatDim % dataTypeInfo.pack == 0 &&
			"Packed data types must be divisible by the format channel count");

	const auto pixelSize = dataTypeInfo.size * formatDim / dataTypeInfo.pack;

	if (cold->created)
		_markDirty();
	cold->dirtyData = true;
	cold->queueData(
			level,
			offset,
			size,
			type,
			libv::dyn_array<std::byte>(
					data,
					std::max(size.x, 1) * std::max(size.y, 1) * std::max(size.z, 1) * pixelSize
			)
	);
}

void Texture::image(int32_t level, libv::gl::CubeSide side, libv::gl::DataType type, const std::byte* data) noexcept {
	// Redirect to 3D image
	const auto layer_i = libv::gl::sideIndex(side);
	image(level, libv::vec3i{0, 0, layer_i}, _imageSize(level), type, data);
}

void Texture::image(int32_t level, libv::gl::CubeSide side, int32_t layer, libv::gl::DataType type, const std::byte* data) noexcept {
	// Redirect to 3D image
	const auto layer_i = libv::gl::sideIndex(side);
	image(level, libv::vec3i{0, 0, layer * 6 + layer_i}, _imageSize(level), type, data);
}

void Texture::image(libv::gl::Image&& image) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();

	// These fields could be set (and if we want to allow manipulation of the image data in file loaded textures
	// it must be set) by extending the gl::Image API (most likely with a proper rework)
	//
	// texture.target = image.type(); not set for gl::Image loading
	// cold->format = not set for gl::Image loading
	// cold->numLevels = not set for gl::Image loading
	// cold->size = image.size(); not set for gl::Image loading
	// cold->sampleCount = not set for ss textures;
	// cold->sampleFixed = not set for ss textures;
	cold->uploadImage = std::move(image);
}

void Texture::set(libv::gl::MagFilter filter) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (cold->magFilter == filter)
		return;

	if (cold->created)
		_markDirty();
	cold->dirtyFilter = true;
	cold->magFilter = filter;
}

void Texture::set(libv::gl::MinFilter filter) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (cold->minFilter == filter)
		return;
	if (cold->created)
		_markDirty();
	cold->dirtyFilter = true;
	cold->minFilter = filter;
}

void Texture::set(libv::gl::Wrap wrapS) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (cold->wrapS == wrapS)
		return;
	if (cold->created)
		_markDirty();
	cold->dirtyWrap1 = true;
	cold->wrapS = wrapS;
}

void Texture::set(libv::gl::Wrap wrapS, libv::gl::Wrap wrapT) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (cold->wrapS == wrapS && cold->wrapT == wrapT)
		return;
	if (cold->created)
		_markDirty();
	cold->dirtyWrap2 = true;
	cold->wrapS = wrapS;
	cold->wrapT = wrapT;
}

void Texture::set(libv::gl::Wrap wrapS, libv::gl::Wrap wrapT, libv::gl::Wrap wrapR) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (cold->wrapS == wrapS && cold->wrapT == wrapT && cold->wrapR == wrapR)
		return;
	if (cold->created)
		_markDirty();
	cold->dirtyWrap3 = true;
	cold->wrapS = wrapS;
	cold->wrapT = wrapT;
	cold->wrapR = wrapR;
}

void Texture::set(libv::gl::Swizzle r, libv::gl::Swizzle g, libv::gl::Swizzle b, libv::gl::Swizzle a) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (cold->swizzleR == r && cold->swizzleG == g && cold->swizzleB == b && cold->swizzleA == a)
		return;
	if (cold->created)
		_markDirty();
	cold->dirtySwizzle = true;
	cold->swizzleR = r;
	cold->swizzleG = g;
	cold->swizzleB = b;
	cold->swizzleA = a;
}

void Texture::set(libv::gl::DepthStencilMode mode) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (cold->depthStencilMode == mode)
		return;
	if (cold->created)
		_markDirty();
	cold->dirtyDepthStencilMode = true;
	cold->depthStencilMode = mode;
}

void Texture::swizzle(std::string_view string) noexcept {
	if (string.size() != 4)
		return log_re.error("Invalid swizzle string. Length must be 4 but it is {} for: \"{}\"", string.size(), string);

	libv::gl::Swizzle value[4];
	for (int i = 0; i < 4; ++i) {
		const auto c = string[i];
		switch(std::tolower(c)) {
		case 'r': value[i] = libv::gl::Swizzle::Red; break;
		case 'g': value[i] = libv::gl::Swizzle::Green; break;
		case 'b': value[i] = libv::gl::Swizzle::Blue; break;
		case 'a': value[i] = libv::gl::Swizzle::Alpha; break;
		case '0': value[i] = libv::gl::Swizzle::Zero; break;
		case '1': value[i] = libv::gl::Swizzle::One; break;
		default:
			return log_re.error("Invalid swizzle string. Character at index {} in \"{}\" is '{}' but it must be one of \"rgba01\"", i, string, c);
		}
	}

	set(value[0], value[1], value[2], value[3]);
}

void Texture::generateMipmaps() noexcept {
	auto lock = std::unique_lock(cold->mutex);
	if (cold->created)
		_markDirty();
	cold->dirtyMipmaps = true;
}

// -------------------------------------------------------------------------------------------------

const libv::gl::Format& Texture::format() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->format;
}

libv::vec3i Texture::size() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->size;
}

// -------------------------------------------------------------------------------------------------

void Texture::requestLoad(LoadingMode mode) noexcept {
	BasicResource::implRequestLoad(this, r.texture, mode);
}
void Texture::requestActivatingLoad(LoadingMode mode) noexcept {
	BasicResource::implRequestActivatingLoad(this, r.texture, mode);
}
void Texture::recordLoadSuccess() noexcept {
	BasicResource::implRecordLoadSuccess(this, r.texture);
}
void Texture::recordLoadFailure() noexcept {
	BasicResource::implRecordLoadFailure(this, r.texture);
}
void Texture::recordUnloadFromCache() noexcept {
	BasicResource::implRecordUnloadFromCache(this);
}

// -------------------------------------------------------------------------------------------------

void Texture::_gl_update(libv::GL& gl) {
	dirty.store(false, std::memory_order_release);

	if (cold->created && cold->dirtyStorage) {
		// Storage change requires recreation of the whole GL texture
		gl(texture).destroy();
		cold->created = false;

		if (cold->minFilter != libv::gl::MinFilter::NearestMipmapLinear || cold->magFilter != libv::gl::MagFilter::Linear)
			cold->dirtyFilter = true;
		if (cold->wrapR != libv::gl::Wrap::Repeat)
			cold->dirtyWrap3 = true;
		else if (cold->wrapT != libv::gl::Wrap::Repeat)
			cold->dirtyWrap2 = true;
		else if (cold->wrapS != libv::gl::Wrap::Repeat)
			cold->dirtyWrap1 = true;
		if (cold->swizzleR != libv::gl::Swizzle::Red || cold->swizzleG != libv::gl::Swizzle::Green || cold->swizzleB != libv::gl::Swizzle::Blue || cold->swizzleA != libv::gl::Swizzle::Alpha)
			cold->dirtySwizzle = true;
		if (cold->depthStencilMode != libv::gl::DepthStencilMode::Depth)
			cold->dirtyDepthStencilMode = true;
	}

	if (!cold->created) {
		cold->created = true;

		if (!cold->uploadImage)
			gl(texture).dsa_create();
	}

	if (cold->uploadImage) {
		LIBV_RE_ASSERT(!cold->dirtyStorage);
		if (texture.id != 0)
			gl(texture).destroy();
		// A better solution could be that the gl::Image accepts the re::Texture
		const auto glt = cold->uploadImage->createTexture();
		texture.target = glt.target;
		texture.id = glt.id;
		cold->uploadImage.reset();
	}

	if (cold->dirtyStorage) {
		cold->dirtyStorage = false;

		switch (texture.target) {
		case libv::gl::TextureTarget::_1D:
			gl(texture).dsa_storage(cold->numLevels, cold->format, cold->size.x);
			break;
		case libv::gl::TextureTarget::_1DArray: [[fallthrough]];
		case libv::gl::TextureTarget::_2D: [[fallthrough]];
		case libv::gl::TextureTarget::Rectangle: [[fallthrough]];
		case libv::gl::TextureTarget::CubeMap:
			gl(texture).dsa_storage(cold->numLevels, cold->format, cold->size.x, cold->size.y);
			break;
		case libv::gl::TextureTarget::CubeMapArray: [[fallthrough]];
		case libv::gl::TextureTarget::_2DArray: [[fallthrough]];
		case libv::gl::TextureTarget::_3D:
			gl(texture).dsa_storage(cold->numLevels, cold->format, cold->size.x, cold->size.y, cold->size.z);
			break;
		case libv::gl::TextureTarget::_2DMultisample:
			gl(texture).dsa_storage_ms(cold->format, libv::vec::xy(cold->size), cold->sampleCount, cold->sampleFixed);
			break;
		case libv::gl::TextureTarget::_2DMultisampleArray:
			gl(texture).dsa_storage_ms(cold->format, cold->size, cold->sampleCount, cold->sampleFixed);
			break;
		case libv::gl::TextureTarget::Buffer:
			LIBV_RE_ASSERT(false && "Internal error: dirtyStorage with Buffer target");
		}
	}

	if (cold->dirtyData) {
		cold->dirtyData = false;
		for (; !cold->uploadDataQueue.empty(); cold->uploadDataQueue.pop_front()) {
			auto& task = cold->uploadDataQueue.front();

			switch (texture.target) {
			case libv::gl::TextureTarget::_1D:
				gl(texture).dsa_subImage(
						task.level,
						task.offset.x,
						task.size.x,
						cold->format.base, task.dataType,
						task.data.data());
				break;
			case libv::gl::TextureTarget::_1DArray:
			case libv::gl::TextureTarget::_2D:
			case libv::gl::TextureTarget::Rectangle:
				gl(texture).dsa_subImage(
						task.level,
						task.offset.x, task.offset.y,
						task.size.x, task.size.y,
						cold->format.base, task.dataType,
						task.data.data());
				break;
			case libv::gl::TextureTarget::CubeMap:
			case libv::gl::TextureTarget::CubeMapArray:
			case libv::gl::TextureTarget::_2DArray:
			case libv::gl::TextureTarget::_3D:
				gl(texture).dsa_subImage(
						task.level,
						task.offset.x, task.offset.y, task.offset.z,
						task.size.x, task.size.y, task.size.z,
						cold->format.base, task.dataType,
						task.data.data());
				break;
			default:
				LIBV_RE_ASSERT(false && "Not implemented yet or invalid input");
				break;
			}
		}
	}

	if (cold->dirtyFilter) {
		cold->dirtyFilter = false;
		gl(texture).dsa_setMagFilter(cold->magFilter);
		gl(texture).dsa_setMinFilter(cold->minFilter);
	}

	if (cold->dirtyWrap1) {
		cold->dirtyWrap1 = false;
		gl(texture).dsa_setWrap(cold->wrapS);
	}

	if (cold->dirtyWrap2) {
		cold->dirtyWrap2 = false;
		gl(texture).dsa_setWrap(cold->wrapS, cold->wrapT);
	}

	if (cold->dirtyWrap3) {
		cold->dirtyWrap3 = false;
		gl(texture).dsa_setWrap(cold->wrapS, cold->wrapT, cold->wrapR);
	}

	if (cold->dirtySwizzle) {
		cold->dirtySwizzle = false;
		gl(texture).dsa_setSwizzle(cold->swizzleR, cold->swizzleG, cold->swizzleB, cold->swizzleA);
	}

	if (cold->dirtyDepthStencilMode) {
		cold->dirtyDepthStencilMode = false;
		gl(texture).dsa_setDepthStencilMode(cold->depthStencilMode);
	}

	if (cold->dirtyMipmaps) {
		cold->dirtyMipmaps = false;
		const auto size = gl(texture).dsa_getSize2D();
		const auto level_count = std::bit_width(static_cast<uint32_t>(std::max(size.x, size.y)));

		gl(texture).dsa_setMaxLevel(level_count);
		gl(texture).dsa_generateMipmap();
	}
}

void Texture::gl_bind(libv::GL& gl, TextureUnit unit) {
	// Thread safety:
	// This solution is similar to a 'double check locking' pattern.
	// A single atomic load decides if we are dirty or not at the point of binding.
	// If the dirty signal is missed it must have been coming from a concurrent event/thread therefore
	// ordering is not guaranteed anyway. It will be picked up during the next binding or in the next frame.
	// The dirty flag can also indicate if the fallback state is active.
	if (dirty.load(std::memory_order_acquire)) {
		auto lock = std::unique_lock(cold->mutex);

		if (cold->fallback) {
			// The fallback state is active, redirect to it.
			// To keep using the fallback texture the dirty has to be kept set.
			// Keep every dirty flag as-is and will execute everything once the fallback state is cleared
			cold->fallback->gl_bind(gl, unit);
			return;
		}
		_gl_update(gl);
	}

	gl(texture).bindUnit(unit);
}

void Texture::gl_bindImage(libv::GL& gl, TextureUnit unit, uint32_t level, libv::gl::BufferAccessFull access) {
	// Access of cold->format forces this mutex lock to always happen
	auto lock = std::unique_lock(cold->mutex);

	if (dirty.load(std::memory_order_acquire)) {
		if (cold->fallback) {
			// The fallback state is active, redirect to it.
			// To keep using the fallback texture the dirty has to be kept set.
			// Keep every dirty flag as-is and will execute everything once the fallback state is cleared
			cold->fallback->gl_bindImage(gl, unit, 0, access);
			return;
		}
		_gl_update(gl);
	}

	gl.bindImageTexture(+unit, texture, level, access, cold->format);
}

libv::gl::Texture Texture::gl_id(libv::GL& gl) {
	if (dirty.load(std::memory_order_acquire)) {
		auto lock = std::unique_lock(cold->mutex);
		LIBV_RE_ASSERT(!cold->fallback && "Direct GL ID access is forbidden for fallback enabled textures");
		_gl_update(gl);
	}

	return texture;
}

void Texture::gl_destroy(libv::GL& gl) {
	gl(texture).destroy();
	cold->created = false;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
