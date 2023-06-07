// Project: libv.arg, File: src/libv/arg/parser.hpp

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
	explicit(false) inline operator bool() const {
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
	bool found_ = false;
	bool hidden_ = false;

public:
	explicit inline BaseArgument(std::vector<std::string> aliases) :
		aliases_(std::move(aliases)) { }

	[[nodiscard]] inline const std::vector<std::string>& aliases() const noexcept {
		return aliases_;
	}

	[[nodiscard]] inline const std::string& name() const noexcept {
		return name_;
	}

	[[nodiscard]] inline const std::string& description() const noexcept {
		return description_;
	}

	[[nodiscard]] constexpr inline bool found() const noexcept {
		return found_;
	}

	inline void mark_as_found() noexcept {
		found_ = true;
	}

	/// Hides the argument from usage and from print unless it is specified
	constexpr inline void hidden(bool value) noexcept {
		hidden_ = value;
	}

	[[nodiscard]] constexpr inline bool hidden() const noexcept {
		return hidden_;
	}

	virtual bool consume() = 0;
	virtual bool fulfilled() = 0;
	virtual void parse(std::string_view str, OutcomeArgumentParse& outcome) = 0;
	virtual void ostream(std::ostream& os) = 0;
	virtual ~BaseArgument() { };
};

namespace detail { // ------------------------------------------------------------------------------

template <typename T>
		requires std::is_arithmetic_v<T>
constexpr inline void parser_function(T& value, std::string_view str, OutcomeArgumentParse& outcome) {
	std::from_chars_result result = std::from_chars(str.data(), str.data() + str.size(), value);
	std::error_code ec = std::make_error_code(result.ec);

	// TODO P4: Use libv.utility parse_number
	//       -: discard leading / trailing whitespace
	//       -: verify if the whole str was consumed by from_chars
	//       -: Only two errc possible: its out of range or its not a number
	//	if (result.ec == std::errc::result_out_of_range)
	//	else if (result.ec == std::errc::invalid_argument)
	//	else // unknown error code

	if (ec)
		outcome.error("Invalid argument: " + ec.message() + " starting from \"" + result.ptr + "\"");
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
	explicit inline Flag(std::vector<std::string> aliases) :
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

	[[nodiscard]] constexpr inline const value_type& value() const noexcept {
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
	explicit inline Rest(std::vector<std::string> aliases) :
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
		for (std::size_t i = 1; i < value_.size(); ++i)
			os << ", " << value_[i];
		os << ']';
	}

	[[nodiscard]] constexpr inline const value_type& value() const noexcept {
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
	explicit inline Optional(std::vector<std::string> aliases) :
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

	[[nodiscard]] constexpr inline const value_type& value() const noexcept {
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
	explicit inline Require(std::vector<std::string> aliases) :
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

	[[nodiscard]] constexpr inline const value_type& value() const noexcept {
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
	explicit Argument(std::shared_ptr<ArgumentT>&& argument) : argument(std::move(argument)) { }

	[[nodiscard]] constexpr inline const auto& value() const noexcept {
		return argument->value();
	}

	template <typename T>
	constexpr inline auto& operator=(T&& value) noexcept {
		argument->value(std::forward<T>(value));
		return *this;
	}

	constexpr inline auto& hidden(bool value) noexcept {
		argument->hidden(value);
		return *this;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename ArgumentT>
class Builder {
	std::shared_ptr<ArgumentT> argument;

public:
	explicit Builder(std::shared_ptr<ArgumentT>&& argument) : argument(std::move(argument)) { }

	auto operator()(std::string name, std::string description = "") {
		argument->name_ = std::move(name);
		argument->description_ = std::move(description);
		return Argument<ArgumentT>{std::move(argument)};
	}
};

// -------------------------------------------------------------------------------------------------

class Parser {
	std::string program_name;
	std::string program_description;
	std::string executable_path_;

	std::unordered_map<std::string, std::shared_ptr<BaseArgument>> identifiers;
	std::vector<std::shared_ptr<BaseArgument>> arguments;

	std::vector<std::shared_ptr<BaseArgument>> requires_;
	std::vector<std::shared_ptr<BaseArgument>> positionals;
//	std::shared_ptr<BaseArgument> arg_exe;
	std::vector<std::pair<int, std::string>> unused;
	bool allow_unused_ = false;

private:
	template <typename T, typename... Aliases>
	constexpr void verifyArgumentType() {
		static_assert(!std::is_same_v<T, std::string_view>, "Argument type cannot be std::string_view to avoid a dangling view, use std::string instead");
		static_assert((std::is_convertible_v<Aliases, std::string> && ...), "Name types has to be convertible to string");
	}

public:
	explicit Parser(std::string program_name = "", std::string program_description = "") :
		program_name(std::move(program_name)),
		program_description(std::move(program_description)) {
	}

public:
//	Argument<std::string> exe() {
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
		allow_unused_ = false;
	}

	inline void allow_unused() noexcept {
		allow_unused_ = true;
	}

public:
	bool standard_validate(const int argc, const char* const* argv, std::ostream& os, int width = 120) {
		auto success = parse(argc, argv);

		if (!success)
			os << error_message(width) << usage(width);

		return success;
	}

public:
	bool parse(const int argc, const char* const* argv) {
		std::size_t positional_index = 0;

		if (argc > 0)
			executable_path_ = argv[0];

		for (int i = 1; i < argc; ++i) {
			BaseArgument* selected = nullptr;

			// Phase: Search
			const auto it = identifiers.find(argv[i]);
			if (it != identifiers.end()) {
				// Identifier found
				selected = &*it->second;
			} else {
				// Identifier not found, check if there is positional arguments
				bool positional_found = false;
				while (positional_index != positionals.size()) {
					selected = &*positionals[positional_index];
					++positional_index;
					if (selected->found()) {
						// This positional argument was already processed by its identifier, skip
						continue;
					} else {
						// Positional arguments found
						positional_found = true;
						--i; // Roll back i to consume argument as value in the next phase
						break;
					}
				}
				if (!positional_found) {
					// Argument is not expected therefore marked as unused
					unused.emplace_back(i, argv[i]);
					continue;
				}
			}

			// Phase: Consume
			selected->mark_as_found();
			while (selected->consume()) {
				++i;
				if (i >= argc) {
					// TODO P5: libv.arg: report error, but not if rest is active or multiple with already fulfilled count
					return false;
				}
				OutcomeArgumentParse outcome;
				selected->parse(argv[i], outcome);
			}
		}

		for (const auto& require : requires_)
			if (!require->fulfilled())
				return false;

		if (!allow_unused_ && !unused.empty())
			return false;

		return true;
	}

	std::ostream& error_message(std::ostream& os, int width = 120) const {
		(void) width; // TODO P5: libv.arg: use width

		os << "Failed to parse parameters:\n";

		if (!allow_unused_)
			for (const auto& [i, v] : unused)
				os << "  Argument \"" << v << "\" at index " << i << " was not used\n";

		for (const auto& require : requires_)
			if (!require->fulfilled())
				os << "  Required parameter \"" << require->name() << "\" was not set\n";

		return os;
	}

	std::string error_message(int width = 120) const {
		std::ostringstream os;
		error_message(os, width);
		return std::move(os).str();
	}

	std::ostream& report(std::ostream& os, int width = 120) const {
		(void) width; // TODO P5: libv.arg: use width

		if (!executable_path_.empty())
			os << "Executable path: " << executable_path_ << "\n";

		if (!arguments.empty())
			os << "Parsed arguments:\n";
		else if (unused.empty())
			os << "No argument were parsed";
		else
			os << "No argument were parsed\n";

		for (const auto& argument : arguments) {
			if (argument->hidden() && !argument->found())
				continue;

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
		return std::move(os).str();
	}

	std::ostream& usage(std::ostream& os, int width = 120) const {
		(void) width; // TODO P5: libv.arg: use width

		os << "Usage:\n";

		for (const auto& argument : arguments) {
			if (argument->hidden() && !argument->found())
				continue;

			os << "  ";

			const auto& aliases = argument->aliases();
			if (!aliases.empty())
				os << "  " << aliases[0];
			for (std::size_t i = 1; i < aliases.size(); ++i)
				os << ", " << aliases[i];

			os << "  " << argument->name();
			if (argument->hidden())
				os << " [hidden]";
			os << ": " << argument->description() << '\n';
		}
		return os;
	}

	std::string usage(int width = 120) const {
		std::ostringstream os;
		usage(os, width);
		return std::move(os).str();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace arg
} // namespace libv
