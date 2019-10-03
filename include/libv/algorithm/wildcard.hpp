// File:   bisect.hpp Author: Vader Created on 02 August 2019, 05:44

#pragma once

// std
#include <string_view>
#include <memory>
#include <iostream>

namespace libv {

// -------------------------------------------------------------------------------------------------

inline bool match_wildcard(const std::string_view str, const std::string_view pattern) noexcept {
	static constexpr auto WILDCARD_SINGLE = '?';
	static constexpr auto WILDCARD_ANY = '*';
//	static constexpr auto WILDCARD_ESCAPE = '\\';
//	static constexpr auto WILDCARD_ESCAPE_ESCAPE = "\\\\";

	// micro optimaliztion for the empty cases
	if (pattern.empty())
		return str.empty();

	if (str.empty()) {
		for (const auto& c : pattern)
			if (c != WILDCARD_ANY)
				return false;
		return true;
	}

	const auto array = std::make_unique<bool[]>((str.size() + 1) * (pattern.size() + 1)); // Yep, vla...
	const auto index = [&](size_t i, size_t j) -> bool& {
		return array[(pattern.size() + 1) * i + j];
	};

	// clear
	for (size_t i = 0; i <= str.size(); ++i)
		for (size_t j = 0; j <= pattern.size(); ++j)
			index(i, j) = false;

	// accept any leading WILDCARD_ANY as str start
	for (size_t j = 1; j <= pattern.size() && pattern[j - 1] == WILDCARD_ANY; ++j)
		index(0, j) = true;

	// base case
	index(0, 0) = true;

	// solve
	for (size_t i = 1; i <= str.size(); i++) {
		for (size_t j = 1; j <= pattern.size(); j++) {
			if (str[i - 1] == pattern[j - 1] || pattern[j - 1] == WILDCARD_SINGLE)
				index(i, j) = index(i - 1, j - 1);

			else if (pattern[j - 1] == WILDCARD_ANY)
				index(i, j) = index(i, j - 1) || index(i - 1, j);
		}
	}

	return index(str.size(), pattern.size());
}

inline bool match_wildcard_layer(const std::string_view str, const std::string_view pattern) noexcept {
	static constexpr auto WILDCARD_SINGLE = '?';
	static constexpr auto WILDCARD_LAYER = '*';
	static constexpr auto WILDCARD_ANY = '$';
	static constexpr auto WILDCARD_SEPARATOR = '/';
//	static constexpr auto WILDCARD_RANGE = "[abc]";
//	static constexpr auto WILDCARD_RANGE = "[a-z]";
//	static constexpr auto WILDCARD_NEGATED_RANGE = "[!abc]";
//	static constexpr auto WILDCARD_NEGATED_RANGE = "[!a-z]";
//	static constexpr auto WILDCARD_ALTERNATE = "a|b";
//	static constexpr auto WILDCARD_GROUP = "(a.txt|b.jpg)";
//	static constexpr auto WILDCARD_ESCAPE = '\\';
//	static constexpr auto WILDCARD_ESCAPE_ESCAPE = "\\\\";

	if (pattern.empty())
		return str.empty();

	if (str.empty()) {
		bool layer_accept = false;
		for (const auto& c : pattern) {
			if (c == WILDCARD_ANY) {
				continue;
			} else if (c == WILDCARD_LAYER && !layer_accept) {
				layer_accept = true;
				continue;
			} else {
				return false;
			}
		}
		return true;
	}

	const auto array = std::make_unique<bool[]>((str.size() + 1) * (pattern.size() + 1)); // Yep, vla...
	const auto index = [&](size_t i, size_t j) -> bool& {
		return array[(pattern.size() + 1) * i + j];
	};

	// clear
	for (size_t i = 0; i <= str.size(); ++i)
		for (size_t j = 0; j <= pattern.size(); ++j)
			index(i, j) = false;

	// accept any leading WILDCARD_ANY and at most one WILDCARD_LAYER as str start
	{
		bool layer_accept = false;
		for (size_t j = 1; j <= pattern.size(); ++j) {
			if (pattern[j - 1] == WILDCARD_LAYER && !layer_accept) {
				layer_accept = true;
				index(0, j) = true;
				continue;
			}
			if (pattern[j - 1] == WILDCARD_ANY) {
				index(0, j) = true;
				continue;
			}
			break;
		}
	}

	// base case
	index(0, 0) = true;

	// solve
	for (size_t i = 1; i <= str.size(); i++) {
		for (size_t j = 1; j <= pattern.size(); j++) {
			const auto sc = str[i - 1];
			const auto pc = pattern[j - 1];
			const auto up = index(i - 1, j);
			const auto left = index(i, j - 1);
			const auto up_left = index(i - 1, j - 1);

			if (pc == sc || pc == WILDCARD_SINGLE && sc != WILDCARD_SEPARATOR)
				index(i, j) = up_left;

			else if (pc == WILDCARD_LAYER)
				index(i, j) = left || up && sc != WILDCARD_SEPARATOR;

			else if (pc == WILDCARD_ANY)
				index(i, j) = left || up;
		}
	}

	return index(str.size(), pattern.size());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
