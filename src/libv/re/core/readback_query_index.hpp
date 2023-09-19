// Project: libv.re, File: src/libv/re/core/uniform_buffer.hpp

#pragma once

#include <libv/re/common/async_ring_size.hpp>

#include <array>
#include <cstdint>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class ReadbackQueryIndex {
private:
	static constexpr uint32_t numRingBuffers = maxAsyncRingSize;

private:
	struct Entry {
		uint16_t loopIndex = 0;
		uint16_t dataIndex = 0;
	};
	std::array<Entry, numRingBuffers> indices{};

public:
	void set(uint32_t bufferIndex, uint16_t loopIndex, uint16_t dataIndex) {
		indices[bufferIndex].loopIndex = loopIndex;
		indices[bufferIndex].dataIndex = dataIndex;
	}

public:
	[[nodiscard]] constexpr inline uint16_t loopIndex(uint32_t bufferIndex) const noexcept {
		return indices[bufferIndex].loopIndex;
	}
	[[nodiscard]] constexpr inline uint16_t dataIndex(uint32_t bufferIndex) const noexcept {
		return indices[bufferIndex].dataIndex;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
