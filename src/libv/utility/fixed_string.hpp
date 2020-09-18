// Project: libv.utility, File: src/libv/utility/fixed_string.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdint>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <size_t N>
class fixed_string {
private:
	char content[N + 1] = {};

public:
	template <typename T>
	constexpr inline fixed_string(const T (&input)[N + 1]) noexcept {
		for (size_t i = 0; i != N; ++i)
			content[i] = input[i];
		content[N] = 0;
	}
	constexpr inline fixed_string(const char* const input) noexcept {
		for (size_t i = 0; i != N; ++i)
			content[i] = input[i];
		content[N] = 0;
	}
	constexpr inline fixed_string(const fixed_string& other) noexcept {
		for (size_t i{0}; i < N; ++i)
			content[i] = other.content[i];
		content[N] = 0;
	}

	constexpr inline size_t size() const noexcept {
		return N;
	}
	constexpr inline const char* begin() const noexcept {
		return content;
	}
	constexpr inline const char* end() const noexcept {
		return content + N;
	}
	constexpr inline char operator[](size_t i) const noexcept {
		return content[i];
	}
	constexpr inline operator const char*() const noexcept {
		return content;
	}
    constexpr inline operator std::string_view() const noexcept {
		return std::string_view{content, N};
	}

	template <size_t M>
	constexpr inline bool operator==(const fixed_string<M> & rhs) const noexcept {
		if (N != M)
			return false;
		for (size_t i = 0; i != N; ++i) {
			if (content[i] != rhs[i])
				return false;
		}
		return true;
	}
};

template <size_t N> fixed_string(const char (&)[N]) -> fixed_string<N - 1>;
template <size_t N> fixed_string(fixed_string<N>) -> fixed_string<N>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
