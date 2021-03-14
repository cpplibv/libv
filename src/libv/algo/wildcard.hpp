// Project: libv.algo, File: src/libv/algo/wildcard.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <memory>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Supported pattern control character sequences:
/// '?' - Matches a single character
/// '*' - Matches zero or more character
bool match_wildcard(const std::string_view str, const std::string_view pattern) noexcept;

/// Supported pattern control character sequences:
/// '/' - The layer separator
/// '\\' - Pattern escape prefix
/// '?' - Matches a single character except the layer separator
/// '*' - Matches zero or more character except the layer separator
/// '**' - Matches zero or more character including the layer separator
bool match_wildcard_glob(const std::string_view str, const std::string_view pattern_str) noexcept;

// -------------------------------------------------------------------------------------------------

class WildcardGlobMatcher {
private:
	std::unique_ptr<class ImplWildcardGlobMatcher> self;

public:
	/// Pattern is stored internally, the lifetime of pattern string_view can expire after the constructor call
	explicit WildcardGlobMatcher(const std::string_view pattern_str);
	inline WildcardGlobMatcher() noexcept = default;
	inline WildcardGlobMatcher(WildcardGlobMatcher&&) noexcept = default;
	inline WildcardGlobMatcher& operator=(WildcardGlobMatcher&&) & noexcept = default;
	~WildcardGlobMatcher();

	/// Change pattern. Pattern is stored internally, the lifetime of pattern string_view can expire after the call
	void pattern(const std::string_view pattern_str);

	/// Returns if the given string is matched by the pattern
	[[nodiscard]] bool match(const std::string_view str) const;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
