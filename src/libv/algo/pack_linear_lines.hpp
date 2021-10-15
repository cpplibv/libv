// Project: libv.algo, File: src/libv/algo/pack_linear_lines.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <span>
#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------
// TODO P4: Refactor API to match rect_bin's pattern

struct PackElementOutput {
	int32_t id;
	libv::vec2i position;
	libv::vec2i size;
};

inline std::vector<PackElementOutput> pack_linear_lines(libv::vec2i texture_size, std::span<libv::vec2i> elements) {
	std::vector<PackElementOutput> result;

	libv::vec2i pen = {0, 0};
	int32_t next_line = 0;
	int32_t id = 0;

	for (const auto& element : elements) {
		if (pen.x + element.x > texture_size.x) {
			// doesn't fit in current line, start next line
			pen.y = next_line;
			pen.x = 0;
		}

		if (pen.y + element.y > texture_size.y) {
			// doesnt fit at all
			return result;
		}

		result.emplace_back(id++, pen, element);

		next_line = std::max(next_line, pen.y + element.y);
		pen.x += element.x;
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace app
