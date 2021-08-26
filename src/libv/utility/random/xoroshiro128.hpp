// Project: libv.utility, File: src/libv/utility/random/xoroshiro128.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Fast and small forkable pseudorandom number generator
/// Based on xoroshiro128++ 1.0 (but added fork, and type safe next function)
/// \source: https://prng.di.unimi.it/xoroshiro128plusplus.c
/// \source: https://www.youtube.com/watch?v=xMdwK9p5qOw
class xoroshiro128 {
public:
	using result_type = uint64_t;

private:
	uint64_t s[2]{18231420318685880306u, 827006149519u};

public:
	constexpr inline xoroshiro128() noexcept = default;

	explicit constexpr inline xoroshiro128(uint64_t s0) noexcept :
		s{s0, s0 ^ 1341429377760273161u} {
	}

	/// The seed cannot be {0, 0}
	constexpr inline xoroshiro128(uint64_t s0, uint64_t s1) noexcept :
		s{s0, s1} {
		assert((s0 != 0 || s1 != 0) && "The seed for xoroshiro128 cannot be {0, 0}");
	}

private:
	[[nodiscard]] constexpr inline result_type aux_next() noexcept {
		const auto s0 = s[0];
		auto s1 = s[1];
		const auto value = std::rotl(s0 + s1, 17) + s0;

		s1 ^= s0;
		s[0] = std::rotl(s0, 49) ^ s1 ^ (s1 << 21);  // a, b
		s[1] = std::rotl(s1, 28);                    // c

		return value;
	}

public:
	// Satisfying the UniformRandomBitGenerator concept

	[[nodiscard]] static constexpr inline result_type min() noexcept {
		return 0;
	}

	[[nodiscard]] static constexpr inline result_type max() noexcept {
		return 18446744073709551615;
	}

	[[nodiscard]] constexpr inline result_type operator()() noexcept {
		return aux_next();
	}

public:
	// Generates the next random number
	template <typename Result>
	[[nodiscard]] constexpr inline Result next() noexcept {
		static_assert(sizeof(Result) <= 8, "Requested Result type is not supported");

		const auto value = aux_next();

		Result result;
		std::memcpy(&result, &value, sizeof(Result));
		return result;
	}

	// Create a new random number generator with the state and the next value from this generator
	[[nodiscard]] constexpr inline xoroshiro128 fork() noexcept {
		const auto s0 = next<uint64_t>();
		return xoroshiro128{s0, s[1]};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv

