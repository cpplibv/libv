// Project: libv.algo, File: src/libv/algo/wildcard.cpp

// hpp
#include <libv/algo/wildcard.hpp>
// ext
#include <boost/container/small_vector.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

bool match_wildcard(const std::string_view str, const std::string_view pattern) noexcept {
	static constexpr auto WILDCARD_SINGLE = '?';
	static constexpr auto WILDCARD_ANY = '*';
//	static constexpr auto WILDCARD_ESCAPE = '\\';
//	static constexpr auto WILDCARD_ESCAPE_ESCAPE = "\\\\";

	boost::container::small_vector<bool, 1024> table;
	table.resize((str.size() + 1) * (pattern.size() + 1), false);
	const auto index = [&](size_t i, size_t j) -> bool& {
		return table[(pattern.size() + 1) * i + j];
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

// -------------------------------------------------------------------------------------------------

struct ImplWildcardGlobMatcher {
	static constexpr std::string_view WILDCARD_ESCAPE = "\\";

	static constexpr std::string_view WILDCARD_LAYER_ANY = "**";
	static constexpr std::string_view WILDCARD_LAYER = "*";
	static constexpr std::string_view WILDCARD_SINGLE = "?";
	//	static constexpr std::string_view WILDCARD_RANGE = "[abc]";
	//	static constexpr std::string_view WILDCARD_RANGE = "[a-z]";
	//	static constexpr std::string_view WILDCARD_NEGATED_RANGE = "[!abc]";
	//	static constexpr std::string_view WILDCARD_NEGATED_RANGE = "[!a-z]";
	//	static constexpr std::string_view WILDCARD_ALTERNATE = "(a.txt|b.jpg)";

	/// Separator is not allowed in WILDCARD_SINGLE or WILDCARD_LAYER (but it is allowed in WILDCARD_LAYER_ANY)
	static constexpr std::string_view WILDCARD_SEPARATOR = "/";

	enum class TokenType {
		literal,   // abc
		single,    // ?
		layer,     // *
		layer_any, // **
	};

	struct Token {
		TokenType type;
		size_t begin = 0;
		size_t size = 0;
	};

public:
	boost::container::small_vector<Token, 10> pattern;
	boost::container::small_vector<std::string_view::value_type, 256> pattern_literals;

public:
	mutable boost::container::small_vector<bool, 1024> table;

public:
	void preprocess(const std::string_view pattern_str) {
		pattern.clear();
		pattern_literals.clear();

		// Preprocess pattern
		for (size_t i = 0; i < pattern_str.size();) {
			if (pattern_str.substr(i).starts_with(WILDCARD_ESCAPE) && pattern_str.size() - i != WILDCARD_ESCAPE.size()) {
				i += WILDCARD_ESCAPE.size();

				if (!pattern.empty() && pattern.back().type == TokenType::literal)
					pattern.back().size++;
				else
					pattern.push_back(Token{TokenType::literal, pattern_literals.size(), 1});

				pattern_literals.push_back(pattern_str[i]);
				i++;

			} else if (pattern_str.substr(i).starts_with(WILDCARD_LAYER_ANY)) {
				i += WILDCARD_LAYER_ANY.size();
				if (!pattern.empty() && pattern.back().type == TokenType::layer)
					pattern.back().type = TokenType::layer_any;

				else if (pattern.empty() || pattern.back().type != TokenType::layer_any)
					pattern.push_back(Token{TokenType::layer_any});

			} else if (pattern_str.substr(i).starts_with(WILDCARD_LAYER)) {
				i += WILDCARD_LAYER.size();
				if (pattern.empty() || (pattern.back().type != TokenType::layer && pattern.back().type != TokenType::layer_any))
					pattern.push_back(Token{TokenType::layer});

			} else if (pattern_str.substr(i).starts_with(WILDCARD_SINGLE)) {
				i += WILDCARD_SINGLE.size();
				if (!pattern.empty() && pattern.back().type == TokenType::single)
					pattern.back().size++;
				else
					pattern.push_back(Token{TokenType::single, 0, 1});

			} else {
				if (!pattern.empty() && pattern.back().type == TokenType::literal)
					pattern.back().size++;
				else
					pattern.push_back(Token{TokenType::literal, pattern_literals.size(), 1});

				pattern_literals.push_back(pattern_str[i]);
				i++;
			}
		}
	}

	inline bool& index(size_t i, size_t j) const noexcept {
		return table[(pattern.size() + 1) * i + j];
	}

	bool match(const std::string_view str) const {
		table.resize((str.size() + 1) * (pattern.size() + 1), false);

		// accept any leading WILDCARD_ANY and at most one WILDCARD_LAYER as str start
		{
			bool layer_accept = false;
			for (size_t j = 1; j <= pattern.size(); ++j) {
				if (pattern[j - 1].type == TokenType::layer && !layer_accept) {
					layer_accept = true;
					index(0, j) = true;
					continue;
				}
				if (pattern[j - 1].type == TokenType::layer_any) {
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
				if (index(i, j)) // Optimization: Already calculated
					continue;

				const auto& token = pattern[j - 1];
				const auto str_literal = str.substr(i - 1);

				const bool up = index(i - 1, j);
				const bool left = index(i, j - 1);
				const bool up_left = index(i - 1, j - 1);

				if (token.type == TokenType::literal) {
					const bool accept = str_literal.starts_with(std::string_view(pattern_literals.data() + token.begin, token.size));
					if (accept && up_left)
						// Optimization: Prefill the every matched character
						for (size_t s = 0; s < token.size && i + s <= str.size() && index(i + s, j) == false; ++s)
							index(i + s, j) = true;

				} else if (token.type == TokenType::single) {
					const bool accept = str_literal.size() >= token.size && str_literal.substr(0, token.size).find_first_of(WILDCARD_SEPARATOR) == str_literal.npos;
					if (accept && up_left)
						// Optimization: Prefill the every matched character
						for (size_t s = 0; s < token.size && i + s <= str.size() && index(i + s, j) == false; ++s)
							index(i + s, j) = true;

				} else if (token.type == TokenType::layer_any) {
					index(i, j) |= left || up;

				} else if (token.type == TokenType::layer) {
					index(i, j) |= left || (up && !str_literal.starts_with(WILDCARD_SEPARATOR));
				}
			}
		}

		return index(str.size(), pattern.size());
	}
};

// -------------------------------------------------------------------------------------------------

WildcardGlobMatcher::WildcardGlobMatcher(const std::string_view pattern_str) :
	self(std::make_unique<ImplWildcardGlobMatcher>()) {
	self->preprocess(pattern_str);
}

void WildcardGlobMatcher::pattern(const std::string_view pattern_str) {
	self->preprocess(pattern_str);
}

bool WildcardGlobMatcher::match(const std::string_view str) const {
	return self->match(str);
}

WildcardGlobMatcher::~WildcardGlobMatcher() {
	// For the sake of forward declared unique_ptr
}

// -------------------------------------------------------------------------------------------------

bool match_wildcard_glob(const std::string_view str, const std::string_view pattern_str) noexcept {
	ImplWildcardGlobMatcher matcher;
	matcher.preprocess(pattern_str);
	return matcher.match(str);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
