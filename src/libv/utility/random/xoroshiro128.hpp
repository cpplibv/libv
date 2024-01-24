// Project: libv.utility, File: src/libv/utility/random/xoroshiro128.hpp

#pragma once

// fwd
#include <libv/utility/random/xoroshiro128_fwd.hpp>
// std
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Fast and small forkable pseudorandom number generator
/// Based on xoroshiro128++ 1.0 (but added fork, seed quality protected construction, type safe next function and UniformRandomBitGenerator concept requirements)
/// Performance:
/// 	Construction from low quality seed is 7 times faster than mt19937 and 10 times mt19937_64.
/// 	Construction from fork is 170 times faster than mt19937 and 250 times mt19937_64.
/// 	Construction from high quality seed is 280 times faster than mt19937 and 400 times mt19937_64.
///		Generation is (maybe a 1-10% slower, but mostly) on-par with mt19937 and twice as fast as mt19937_64.
/// Memory footprint: 16 byte (mt19937 and mt19937_64 are 2504 byte)
/// \source: https://prng.di.unimi.it/xoroshiro128plusplus.c
/// \source: https://www.youtube.com/watch?v=xMdwK9p5qOw
class xoroshiro128 {
public:
	using result_type = uint64_t;

private:
	static constexpr uint64_t random_bits_0 = 1341429377760273161u;
	static constexpr uint64_t random_bits_1 = 3340210110105959566u;
	static constexpr uint64_t salt_bit45 = uint64_t{1} << 45u;
	static constexpr uint64_t salt_bit49 = uint64_t{1} << 49u;

	uint64_t s[2]{18231420318685880306u, 827006149519u};

public:
	constexpr inline xoroshiro128() noexcept = default;

	/// Accepts low quality seeds (like IDs or Indices)
	explicit constexpr inline xoroshiro128(uint64_t s0) noexcept :
		s{s0, s0 ^ random_bits_0} { // Seed cannot be {0, 0} and xor will make sure of that
		jump(); // Call to jump() to accept low quality seed
	}

	/// Accepts low quality seeds (like IDs or Indices)
	/// The seed can be {0, 0}
	constexpr inline xoroshiro128(uint64_t s0, uint64_t s1) noexcept :
		s{(s0 ^ random_bits_0) | salt_bit45, (s1 ^ random_bits_1) | salt_bit49} {
		// The seed for xoroshiro128 cannot be {0, 0} so 1-1 salt bit is forced to 1
		// random_bits_0 and random_bits_1 is xor-ed into the seeds to improve entropy for low inputs
		jump(); // Call to jump() to accept low quality seed
	}

private:
	struct HighQ {};
	/// The seed must not be {0, 0}, Seed is required to be high quality
	constexpr inline xoroshiro128(uint64_t s0, uint64_t s1, HighQ) noexcept :
		s{s0, s1} {
		assert((s0 != 0 || s1 != 0) && "The seed for xoroshiro128 cannot be {0, 0}");
	}

public:
	/// Directly seeds the pseudorandom number generator internal state using s0.
	/// The provided seed must be a high quality 64 random bits otherwise the starting sequence's quality will poor
	static constexpr inline xoroshiro128 high_quality_seed(uint64_t s0) {
		return xoroshiro128{~s0 ^ random_bits_0, s0 ^ random_bits_0, HighQ{}};
	}

	/// Directly seeds the pseudorandom number generator internal state using s0 and s1
	/// The provided seed must be a high quality 64 random bits otherwise the starting sequence's quality will poor
	static constexpr inline xoroshiro128 high_quality_seed(uint64_t s0, uint64_t s1) {
		return xoroshiro128{s0, s1, HighQ{}};
	}

public:
	/// State accessors, mostly only useful for serialization
	[[nodiscard]] constexpr inline uint64_t state_s0() const noexcept {
		return s[0];
	}
	/// State accessors, mostly only useful for serialization
	[[nodiscard]] constexpr inline uint64_t state_s1() const noexcept {
		return s[1];
	}
	/// State accessors, mostly only useful for serialization
	constexpr inline void state_s0(uint64_t s0) noexcept {
		s[0] = s0;
	}
	/// State accessors, mostly only useful for serialization
	constexpr inline void state_s1(uint64_t s1) noexcept {
		s[1] = s1;
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
		return 18446744073709551615u;
	}

	[[nodiscard]] constexpr inline result_type operator()() noexcept {
		return aux_next();
	}

public:
	/// This is the jump function for the generator. It is equivalent
	/// to 2^64 calls to next(); it can be used to generate 2^64
	/// non-overlapping subsequences for parallel computations.
	constexpr inline void jump() noexcept {
		constexpr uint64_t JUMP[] = {0x2BD7A6A6E99C2DDCu, 0x0992CCAF6A6FCA05u};

		uint64_t s0 = 0;
		uint64_t s1 = 0;

		for (uint64_t i : JUMP)
			for (uint64_t b = 0; b < 64; b++) {
				if (i & 1u << b) {
					s0 ^= s[0];
					s1 ^= s[1];
				}
				[[maybe_unused]] auto ignore = aux_next();
			}

		s[0] = s0;
		s[1] = s1;
	}

	/// Generates the next random number
	template <typename Result>
	[[nodiscard]] constexpr inline Result next() noexcept {
		static_assert(sizeof(Result) <= 8, "Requested Result type is not supported");

		const auto value = aux_next();

		Result result;
		std::memcpy(&result, &value, sizeof(Result));
		return result;
	}

	/// Create a new random number generator with the state and the next value from this generator
	[[nodiscard]] constexpr inline xoroshiro128 fork() noexcept {
		const auto s0 = next<uint64_t>();
		const auto s1 = next<uint64_t>();
		return high_quality_seed(s0, s1);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv

