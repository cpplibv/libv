// Project: libv.re, File: src/libv/re/uniform/uniform.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/meta/assignable_to.hpp>
#include <libv/re/common/block_binding.hpp>
#include <libv/re/common/texture_unit.hpp>
#include <libv/re/uniform/concept.hpp>
#include <libv/re/uniform/uniform_block.hpp>

#include <cstdint>
#include <utility>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct UniformLocation {
	struct Block {
		// For uniform blocks stores the byte offset from the beginning of the block
		uint32_t offset = 0;
	};
	struct NonBlock {
		// For freestanding uniforms stores the *indirect* location index
		// (an index into the shaders location index table) for both normal and depth-only shader
		uint16_t indirectNormal = 0;
		uint16_t indirectDepth = 0;
	};
	union {
		Block block;
		NonBlock nonBlock;
	};
	constexpr UniformLocation() : block{0} { }
};
static_assert(sizeof(UniformLocation) == 4);

// -------------------------------------------------------------------------------------------------

template <typename T>
struct Uniform {
	using uniform_wrapper = void;
	using value_type = T;

	UniformLocation location;
	value_type value;

public:
	LIBV_FORCE_INLINE Uniform() = default;
	explicit(false) LIBV_FORCE_INLINE Uniform(value_type value) : value(value) { }

	template <typename K>
	LIBV_FORCE_INLINE Uniform& operator=(K&& newValue) & {
		value = std::forward<K>(newValue);
		return *this;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const Uniform& rhs) const noexcept {
		return value == rhs.value;
	}
};

// Uniform<Texture> is special, it handles both the binding and the actual texture
template <>
struct Uniform<Texture> {
	using uniform_wrapper = void;
	using value_type = libv::gl::TextureUnit;

	UniformLocation location;
	value_type value; // Uniform value for sampler
	Texture_ptr texture; // Texture to be bound

public:
	LIBV_FORCE_INLINE Uniform() : Uniform(TextureUnit{0}) { }
	explicit(false) LIBV_FORCE_INLINE Uniform(libv::gl::TextureUnit channel) : value(channel) { }
	LIBV_FORCE_INLINE Uniform(libv::gl::TextureUnit channel, assignable_to<Texture_ptr&> auto&& texture_) :
		value(channel),
		texture(std::move(texture_)) { }

	LIBV_FORCE_INLINE Uniform& operator=(assignable_to<libv::gl::TextureUnit&> auto&& binding) & {
		value = std::forward<decltype(binding)>(binding);
		return *this;
	}

	LIBV_FORCE_INLINE Uniform& operator=(assignable_to<Texture_ptr&> auto&& texture_) & {
		texture = std::forward<decltype(texture_)>(texture_);
		return *this;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const Uniform<Texture>& rhs) const noexcept {
		return value == rhs.value;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
	requires is_uniform_block<T>
struct Uniform<T> {
	using uniform_block_wrapper = void;
	using block_type = T;

	UniformBlock_ptr<T> block;

public:
	LIBV_FORCE_INLINE Uniform() { }
	explicit(false) LIBV_FORCE_INLINE Uniform(UniformBlock_ptr<T>&& block) : block(std::move(block)) { }
	explicit(false) LIBV_FORCE_INLINE Uniform(const UniformBlock_ptr<T>& block) : block(block) { }

	LIBV_FORCE_INLINE Uniform& operator=(UniformBlock_ptr<T>&& block) & {
		this->block = std::move(block);
		return *this;
	}
	LIBV_FORCE_INLINE Uniform& operator=(const UniformBlock_ptr<T>& block) & {
		this->block = block;
		return *this;
	}

	[[nodiscard]] constexpr inline T* operator->() noexcept {
		return block.operator->();
	}
	[[nodiscard]] constexpr inline const T* operator->() const noexcept {
		return block.operator->();
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const Uniform& rhs) const noexcept {
		return this->block == rhs.block;
	}

	[[nodiscard]] constexpr inline auto bufferBinding() const noexcept {
		return T::binding;
	}
	[[nodiscard]] constexpr inline uint32_t bufferOffset() const noexcept {
		return block.placement().bufferOffset;
	}
	[[nodiscard]] constexpr inline uint32_t bufferSize() const noexcept {
		return sizeof(T);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
