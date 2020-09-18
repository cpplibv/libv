// Project: libv.arg, File: src/libv/arg/parser.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
//#include <fmt/format.h>
// std
#include <charconv>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <unordered_map>
#include <vector>
//#include <variant>


namespace libv {
namespace arg {

// -------------------------------------------------------------------------------------------------

template <typename T>
class Builder;

// -------------------------------------------------------------------------------------------------

class OutcomeArgumentParse {
	std::optional<std::string> error_;
	std::optional<std::string> warning_;

public:
	inline operator bool() const {
		return !error_ && !warning_;
	}
	inline void error(std::string message) {
		error_ = std::move(message);
	}
	inline void warning(std::string message) {
		warning_ = std::move(message);
	}
};

class BaseArgument {
	template <typename T>
	friend class Builder;

private:
	std::vector<std::string> aliases_;
	std::string name_;
	std::string description_;

public:
	inline BaseArgument(std::vector<std::string> aliases) :
		aliases_(std::move(aliases)) { }

	inline const std::vector<std::string>& aliases() const noexcept {
		return aliases_;
	}

	inline const std::string& name() const noexcept {
		return name_;
	}

	inline const std::string& description() const noexcept {
		return description_;
	}

	virtual bool consume() = 0;
	virtual bool fulfilled() = 0;
	virtual void parse(std::string_view str, OutcomeArgumentParse& outcome) = 0;
	virtual void ostream(std::ostream& os) = 0;
	virtual ~BaseArgument() { };
};

namespace detail { // ------------------------------------------------------------------------------

template <typename T,
		typename = std::enable_if_t<std::is_integral_v<T>>>
		// This could be is_arithmetic_v once Clang/GCC implements from_chars to floating point types
constexpr inline void parser_function(T& value, std::string_view str, OutcomeArgumentParse& outcome) {
	std::from_chars_result result = std::from_chars<T>(str.begin(), str.end(), value);
	std::error_code ec = std::make_error_code(result.ec);

	// TODO P5: discard leading / trailing whitespace
	// TODO P5: verify if the whole str was consumed by from_chars
	// TODO P5: Only two errc possible: its out of range or its not a number
	//	if (result.ec == std::errc::result_out_of_range)
	//	else if (result.ec == std::errc::invalid_argument)
	//	else // unkown error code

	if (ec)
		outcome.error("Invalid argument: " + ec.message() + " starting from \"" + result.ptr + "\"");
}

// This could be removed once Clang/GCC implements from_chars to floating point types
inline void parser_function(float& value, std::string_view str, OutcomeArgumentParse& outcome) {
	size_t pos = 0;
	try {
		value = std::stof(std::string(str), &pos);
	} catch (const std::invalid_argument& e) {
		// no conversion could be performed
		outcome.error(std::string("Invalid argument: ") + e.what() + " starting at " + (str.begin() + pos));
	} catch (const std::out_of_range& e) {
		// the converted value would fall out of the range of the result type or if the underlying function set errno
		outcome.error(std::string("Out of range: ") + e.what() + " starting at " + (str.begin() + pos));
	}
}

// This could be removed once Clang/GCC implements from_chars to floating point types
inline void parser_function(double& value, std::string_view str, OutcomeArgumentParse& outcome) {
	size_t pos = 0;
	try {
		value = std::stod(std::string(str), &pos);
	} catch (const std::invalid_argument& e) {
		// no conversion could be performed
		outcome.error(std::string("Invalid argument: ") + e.what() + " starting at " + (str.begin() + pos));
	} catch (const std::out_of_range& e) {
		// the converted value would fall out of the range of the result type or if the underlying function set errno
		outcome.error(std::string("Out of range: ") + e.what() + " starting at " + (str.begin() + pos));
	}
}

inline void parser_function(std::string& value, std::string_view str, OutcomeArgumentParse& outcome) {
	(void) outcome;
	value = str;
}

} // namespace detail ------------------------------------------------------------------------------

// TODO P5: libv.arg: implicit value for typed flags, both default_value(T) and implicit_value(T)

class Flag : public BaseArgument {
	using value_type = bool;
	value_type value_ = false;

public:
	inline Flag(std::vector<std::string> aliases) :
		BaseArgument(std::move(aliases)) { }

	virtual bool consume() override {
		value_ = true;
		return false;
	}

	virtual bool fulfilled() override {
		return true;
	}

	virtual void parse(std::string_view str, OutcomeArgumentParse& outcome) override {
		(void) str;
		(void) outcome;
	}

	virtual void ostream(std::ostream& os) override {
		os << ": " << (value_ ? "true" : "false");
	}

	constexpr inline const value_type& value() const noexcept {
		return value_;
	}

	template <typename V>
	constexpr inline void value(V&& v) {
		value_ = std::forward<V>(v);
	}
};

class Rest : public BaseArgument {
	using value_type = std::vector<std::string>;
	value_type value_;

public:
	inline Rest(std::vector<std::string> aliases) :
		BaseArgument(std::move(aliases)) { }

	virtual bool consume() override {
		return true;
	}

	virtual bool fulfilled() override {
		return true;
	}

	virtual void parse(std::string_view str, OutcomeArgumentParse& outcome) override {
		(void) outcome;

		value_.emplace_back(str);
	}

	virtual void ostream(std::ostream& os) override {
		os << ": [";
		if (!value_.empty())
			os << value_.front();
		for (size_t i = 1; i < value_.size(); ++i)
			os << ", " << value_[i];
		os << ']';
	}

	constexpr inline const value_type& value() const noexcept {
		return value_;
	}

	template <typename V>
	constexpr inline void value(V&& v) {
		value_ = std::forward<V>(v);
	}
};

template <typename T>
class Optional : public BaseArgument {
	using value_type = std::optional<T>;
	value_type value_;
	bool consume_ = true;

public:
	inline Optional(std::vector<std::string> aliases) :
		BaseArgument(std::move(aliases)) { }

	virtual bool consume() override {
		return consume_;
	}

	virtual bool fulfilled() override {
		return true;
	}

	virtual void parse(std::string_view str, OutcomeArgumentParse& outcome) override {
		consume_ = false;
		value_.emplace();
		detail::parser_function(*value_, str, outcome);
		if (!outcome)
			value_.reset();
	}

	virtual void ostream(std::ostream& os) override {
		if (value_.has_value())
			os << ": " << *value_;
		else
			os << " is not set";
	}

	constexpr inline const value_type& value() const noexcept {
		return value_;
	}

	template <typename V>
	constexpr inline void value(V&& v) {
		value_.emplace(std::forward<V>(v));
	}
};

template <typename T>
class Require : public BaseArgument {
	using value_type = T;
	value_type value_;
	bool consume_ = true;
	bool fulfilled_ = false;

public:
	inline Require(std::vector<std::string> aliases) :
		BaseArgument(std::move(aliases)) { }

	virtual bool consume() override {
		return consume_;
	}

	virtual bool fulfilled() override {
		return fulfilled_;
	}

	virtual void parse(std::string_view str, OutcomeArgumentParse& outcome) override {
		consume_ = false;
		detail::parser_function(value_, str, outcome);
		fulfilled_ = outcome;
	}

	virtual void ostream(std::ostream& os) override {
		if (fulfilled_)
			os << ": " << value_;
		else
			os << " is not set";
	}

	constexpr inline const value_type& value() const noexcept {
		return value_;
	}

	template <typename V>
	constexpr inline void value(V&& v) {
		value_ = std::forward<V>(v);
		fulfilled_ = true;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename ArgumentT>
class Argument {
	friend class Parser;

//	using value_type = typename T::value_type;
	std::shared_ptr<ArgumentT> argument;

public:
	Argument(std::shared_ptr<ArgumentT>&& argument) : argument(std::move(argument)) { }

	constexpr inline const auto& value() const noexcept {
		return argument->value();
	}

	template <typename T>
	constexpr inline auto& operator=(T&& value) noexcept {
		argument->value(std::forward<T>(value));
		return *this;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename ArgumentT>
class Builder {
	std::shared_ptr<ArgumentT> argument;

public:
	Builder(std::shared_ptr<ArgumentT>&& argument) : argument(std::move(argument)) { }

	Argument<ArgumentT> operator()(std::string name, std::string description = "") {
		argument->name_ = std::move(name);
		argument->description_ = std::move(description);
		return {std::move(argument)};
	}
};

// -------------------------------------------------------------------------------------------------

class Parser {
	std::unordered_map<std::string, std::shared_ptr<BaseArgument>> identifiers;
	std::vector<std::shared_ptr<BaseArgument>> arguments;
	std::vector<std::shared_ptr<BaseArgument>> requires_;
	std::vector<std::shared_ptr<BaseArgument>> positionals;
//	std::shared_ptr<BaseArgument> arg_exe;
	std::vector<std::pair<int, std::string>> unused;
	bool require_no_unused_ = false;

private:
	template <typename T, typename... Aliases>
	constexpr void verifyArgumentType() {
		static_assert(!std::is_same_v<T, std::string_view>, "Argument type cannot be std::string_view to avoid a dangling view, use std::string instead");
		static_assert((std::is_convertible_v<Aliases, std::string> && ...), "Name types has to be convertible to string");
	}

public:
//	Builder<Require<std::string>> exe() {
//		if (!arg_exe)
//			arg_exe = std::make_shared<Require<std::string>>("exe");
//
//		return Argument<Require<std::string>>{arg_exe};
//	}

	template <typename... Aliases>
	Builder<Flag> flag(Aliases&&... aliases) {
		verifyArgumentType<bool, Aliases...>();

		auto argument = std::make_shared<Flag>(std::vector<std::string>{std::string(aliases)...});
		arguments.emplace_back(argument);
		(identifiers.emplace(std::forward<Aliases>(aliases), argument), ...);

		return Builder<Flag>{std::move(argument)};
	}

	template <typename T, typename... Aliases>
	Builder<Optional<T>> optional(Aliases&&... aliases) {
		verifyArgumentType<T, Aliases...>();

		auto argument = std::make_shared<Optional<T>>(std::vector<std::string>{std::string(aliases)...});
		arguments.emplace_back(argument);
		(identifiers.emplace(std::forward<Aliases>(aliases), argument), ...);

		return Builder<Optional<T>>{std::move(argument)};
	}

	template <typename T, typename... Aliases>
	Builder<Require<T>> require(Aliases&&... aliases) {
		verifyArgumentType<T, Aliases...>();

		auto argument = std::make_shared<Require<T>>(std::vector<std::string>{std::string(aliases)...});
		arguments.emplace_back(argument);
		requires_.emplace_back(argument);
		(identifiers.emplace(std::forward<Aliases>(aliases), argument), ...);

		return Builder<Require<T>>{std::move(argument)};
	}

	template <typename... Aliases>
	Builder<Rest> rest(Aliases&&... aliases) {
		verifyArgumentType<std::string, Aliases...>();

		auto argument = std::make_shared<Rest>(std::vector<std::string>{std::string(aliases)...});
		arguments.emplace_back(argument);
		(identifiers.emplace(std::forward<Aliases>(aliases), argument), ...);

		return Builder<Rest>{std::move(argument)};
	}

public:
	template <typename... Arguments>
	void positional(const Arguments&... arguments) {
		(positionals.emplace_back(arguments.argument), ...);
	}

public:
	inline void require_no_unused() noexcept {
		require_no_unused_ = true;
	}

public:
	bool parse(const int argc, const char* const* argv) {
		for (int i = 1; i < argc; ++i) {
			BaseArgument* selected;

			const auto it = identifiers.find(argv[i]);
			if (it != identifiers.end()) {
				selected = &*it->second;
			} else if (!positionals.empty()) {
				selected = &*positionals.front();
				positionals.erase(positionals.begin());
				i--; // Roll back i to consume argument
				// NOTE: positional is not the sole owner of argument, shared pointer stability is used
				// TODO P5: libv.arg: make the ownership rules nicer
			} else {
				unused.emplace_back(i, argv[i]);
				continue;
			}

			while (selected->consume()) {
				i++;
				if (i >= argc) {
					// report error, but not if rest is active or multiple with already fulfilled count
					return false;
				}
				OutcomeArgumentParse outcome;
				selected->parse(argv[i], outcome);
			}
		}

		if (require_no_unused_ && !unused.empty())
			return false;

		for (const auto& require : requires_)
			if (!require->fulfilled())
				return false;

		return true;
	}

	std::ostream& report(std::ostream& os, int width = 120) const {
		(void) width; // TODO P5: libv.arg: use width

		os << "Parsed arguments:\n";
		for (const auto& argument : arguments) {
			os << "  " << argument->name();
			argument->ostream(os);
			os << '\n';
		}

		if (!unused.empty())
			os << "Unused arguments:\n";
		for (const auto& argument : unused)
			os << "  " << argument.first << ": \"" << argument.second << "\"\n";

		return os;
	}

	std::string report(int width = 120) const {
		std::ostringstream os;
		report(os, width);
		return std::move(os.str());
	}

	std::ostream& usage(std::ostream& os, int width = 120) const {
		(void) width; // TODO P5: libv.arg: use width

		for (const auto& argument : arguments) {
			os << "  " << argument->name();

			const auto& aliases = argument->aliases();
			if (!aliases.empty())
				os << "  " << aliases[0];
			for (size_t i = 1; i < aliases.size(); ++i)
				os << ", " << aliases[i];
			os << "  " << argument->description() << '\n';
		}
		return os;
	}

	std::string usage(int width = 120) const {
		std::ostringstream os;
		usage(os, width);
		return std::move(os.str());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace arg
} // namespace libv
