// Created by Vader on 2023.04.17.

// std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline uint32_t log2_ceil(uint64_t x) noexcept {
    const uint64_t t[6] = {
            0xFFFFFFFF00000000ull,
            0x00000000FFFF0000ull,
            0x000000000000FF00ull,
            0x00000000000000F0ull,
            0x000000000000000Cull,
            0x0000000000000002ull
    };

    int y = (((x & (x - 1)) == 0) ? 0 : 1);
    int j = 32;

    for (const uint64_t i : t) {
        const int k = (((x & i) == 0) ? 0 : j);
        y += k;
        x >>= k;
        j >>= 1;
    }

    return static_cast<uint32_t>(y);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
