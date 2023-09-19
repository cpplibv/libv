// Project: libv.re, File: src/libv/re/core/canvas.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Canvas : public libv::ref_count_base {
public:
	friend libv::ref_count_access;
	using ptr = Canvas_ptr;
	using cptr = Canvas_cptr;

private:
	libv::vec2i size_;
	libv::vec2i position_;

public:
	Canvas() { }
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}

public:
	constexpr inline void size(libv::vec2i value) noexcept {
		size_ = value;
	}
	[[nodiscard]] constexpr inline libv::vec2i sizei() const noexcept {
		return size_;
	}
	[[nodiscard]] constexpr inline libv::vec2f sizef() const noexcept {
		return size_.cast<float>();
	}
	constexpr inline void position(libv::vec2i value) noexcept {
		position_ = value;
	}
	[[nodiscard]] constexpr inline libv::vec2i positioni() const noexcept {
		return position_;
	}
	[[nodiscard]] constexpr inline libv::vec2f positionf() const noexcept {
		return position_.cast<float>();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
