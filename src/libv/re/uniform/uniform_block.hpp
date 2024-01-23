// Project: libv.re, File: src/libv/re/uniform/uniform_block.hpp

#pragma once

#include <libv/re/uniform/concept.hpp>

#include <cstdint>
#include <memory>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct UniformBlockBufferPlacement {
	uint32_t bufferOffset = 0;
	uint32_t generationIndex = 0;
};

template <typename T>
class UniformBlock_ptr {
	using block_type = T;

private:
	struct Pack {
		UniformBlockBufferPlacement bufferPlacement;
		T block;
	};
	std::shared_ptr<Pack> ptr;

private:
	explicit constexpr inline UniformBlock_ptr(std::shared_ptr<Pack>&& pack) : ptr(std::move(pack)) {};
public:
	constexpr inline UniformBlock_ptr() : ptr(nullptr) {};
	constexpr inline UniformBlock_ptr(const UniformBlock_ptr&) noexcept = default;
	constexpr inline UniformBlock_ptr& operator=(const UniformBlock_ptr&) & noexcept = default;
	constexpr inline UniformBlock_ptr(UniformBlock_ptr&&) noexcept = default;
	constexpr inline UniformBlock_ptr& operator=(UniformBlock_ptr&&) & noexcept = default;

	template <typename... Args>
	[[nodiscard]] static constexpr inline UniformBlock_ptr create(Args&&... args) {
		static_assert(is_uniform_block<block_type>);
		return UniformBlock_ptr(std::make_shared<Pack>(std::forward<Args>(args)...));
	}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const UniformBlock_ptr& rhs) const noexcept {
		return this->ptr == rhs.ptr;
	}

public:
	[[nodiscard]] constexpr inline T* operator->() noexcept {
		return assert(ptr != nullptr), &ptr->block;
	}
	[[nodiscard]] constexpr inline const T* operator->() const noexcept {
		return assert(ptr != nullptr), &ptr->block;
	}
	[[nodiscard]] constexpr inline T& operator*() noexcept {
		return assert(ptr != nullptr), ptr->block;
	}
	[[nodiscard]] constexpr inline const T& operator*() const noexcept {
		return assert(ptr != nullptr), ptr->block;
	}
	[[nodiscard]] constexpr inline UniformBlockBufferPlacement& placement() noexcept {
		return assert(ptr != nullptr), ptr->bufferPlacement;
	}
	[[nodiscard]] constexpr inline const UniformBlockBufferPlacement& placement() const noexcept {
		return assert(ptr != nullptr), ptr->bufferPlacement;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
