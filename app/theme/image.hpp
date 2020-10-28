// Project: libv, File: app/theme/image.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/container/vector_2d.hpp>
#include <libv/math/vec.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Image {
private:
	libv::vector_2D<libv::vec4f> color_0;
	libv::vector_2D<float> sdistance_field;

public:
	explicit Image(libv::vec2z size) :
		color_0(size, libv::vec4f{0, 0, 0, 0}),
		sdistance_field(size, 0.0f) {
	}

public:
	[[nodiscard]] inline libv::vec4f& color(size_t x, size_t y) noexcept {
		return color_0(x, y);
	}

	[[nodiscard]] inline const libv::vec4f& color(size_t x, size_t y) const noexcept {
		return color_0(x, y);
	}

	[[nodiscard]] inline float& sdistance(size_t x, size_t y) noexcept {
		return sdistance_field(x, y);
	}

	[[nodiscard]] inline const float& sdistance(size_t x, size_t y) const noexcept {
		return sdistance_field(x, y);
	}

	[[nodiscard]] inline libv::vec2z size() const noexcept {
		return color_0.size();
	}

public:
	[[nodiscard]] libv::vector_2D<libv::vec4uc> generate_8bit_channels() const {
		libv::vector_2D<libv::vec4uc> result{size(), libv::vec4uc{}};

		for (size_t y = 0; y < size().y; ++y) {
			for (size_t x = 0; x < size().x; ++x) {
				result(x, y) = (libv::vec::clamp(color(x, y), 0.0f, 1.0f) * 255.f).cast<uint8_t>();
			}
		}

		return result;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
