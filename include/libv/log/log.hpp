// File: Log.hpp, Created on 2014. december 1. 12:50, Author: Vader

#pragma once

// ext
#include <fmt/ostream.h>
#include <fmt/format.h>
// libv
#include <libv/thread/number.hpp>
#include <libv/utility/observer_ref.hpp>
#include <libv/utility/source_location.hpp>
// std
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>


// TODO P3: Log system should provide an interface to name threads. And it most likely will not be
//			thread_local but a map inside the log system, or better a "specific log message".
// TODO P4: Consider warn / fail / log unused arguments too, (or not, as this requires format parsing)
// TODO P4: Log system could get its own thread, later this can be useful for high stress debugs and
//			system analysis, or binary logging...
// TODO P4: {line_gap} assign the rest of the command line into this gap, this should also be supported in the messages in some form
//			Most likely will be integrated into the serialization and output-formatter system
// IDEA: Format string per route; most likely will be integrated into the serialization and output-formatter system
//		libv::log.route_below(libv::Info, std::cout, "{severity} {thread_id} {module}: {message} <{file}:{line}>\n");
//		libv::log.route(std::cout, "{severity} {module}: {message}\n");

// -------------------------------------------------------------------------------------------------
// Trace - Only when tracing the code and execution steps
// Debug - Information that can be is diagnostically helpful
// Info  - Generally useful information to log (service start/stop, configuration assumptions, etc). Info is always available but is not important under normal circumstances
// Warn  - Anything that could cause application oddities, but for which automatically recovering is possible (switching to backup server, retrying an operation, etc)
// Error - Any error which is fatal to the operation but not the service or application. Usually requires user intervention (missing file)
// Fatal - Any error that is forcing a shutdown of the service or application to prevent (further) data loss or corruption
// -------------------------------------------------------------------------------------------------

namespace libv {

// -------------------------------------------------------------------------------------------------

#ifndef WISH_SHORT_PATH_CUTOFF
#    define WISH_SHORT_PATH_CUTOFF 0
#endif

class Logger {
public:
	enum class Severity {
		Trace = 1,
		Debug = 2,
		Info = 3,
		Warn = 4,
		Error = 5,
		Fatal = 6,
	};

private:
	inline std::string_view toString(Severity value) {
		switch (value) {
		case Severity::Trace: return "Trace";
		case Severity::Debug: return "Debug";
		case Severity::Info: return "Info ";
		case Severity::Warn: return "Warn ";
		case Severity::Error: return "Error";
		case Severity::Fatal: return "Fatal";
		default: return "Undefined";
		}
	}

	inline std::string_view toColorString(Severity value) {
		switch (value) {
		case Severity::Trace: return "\u001B[37mTrace\u001B[0m";
		case Severity::Debug: return "\u001B[37mDebug\u001B[0m";
		case Severity::Info: return  "\u001B[32mInfo \u001B[0m";
		case Severity::Warn: return  "\u001B[33mWarn \u001B[0m";
		case Severity::Error: return "\u001B[31mError\u001B[0m";
		case Severity::Fatal: return "\u001B[35mFatal\u001B[0m";
		default: return "Undefined";
		}
	}

	struct MatchResult {
		bool matched;
		bool allow;
	};

	struct Rule {
		using MatcherFunction = bool (Rule::*)(Severity, const std::string_view) const;

	private:
		std::string module{""};
		Severity severity = Severity::Trace;
		bool allow;
		MatcherFunction matcher;

	private:
		bool isSubModul(const std::string_view submodule) const {
			const auto isPrefix = submodule.compare(0, module.size(), module) == 0;
			const auto isEqualLenght = submodule.size() == module.size();

			return isPrefix && (isEqualLenght
					|| (submodule.size() > module.size() && submodule[module.size()] == '.'));
		}

	public:
		bool matcher_any(Severity, const std::string_view) const {
			return true;
		}
		bool matcher_module(Severity, const std::string_view module) const {
			return isSubModul(module);
		}
		bool matcher_severity_equal(Severity severity, const std::string_view) const {
			return severity == this->severity;
		}
		bool matcher_module_severity_equal(Severity severity, const std::string_view module) const {
			return isSubModul(module) && severity == this->severity;
		}
		bool matcher_severity_above(Severity severity, const std::string_view) const {
			return severity > this->severity;
		}
		bool matcher_module_severity_above(Severity severity, const std::string_view module) const {
			return isSubModul(module) && severity > this->severity;
		}
		bool matcher_severity_below(Severity severity, const std::string_view) const {
			return severity < this->severity;
		}
		bool matcher_module_severity_below(Severity severity, const std::string_view module) const {
			return isSubModul(module) && severity < this->severity;
		}

	public:
		MatchResult match(Severity severity, const std::string_view module) const {
			return {(this->*matcher)(severity, module), allow};
		}

	public:
		Rule(const std::string_view module,
				Severity severity,
				bool allow,
				MatcherFunction matcher) :
			module(module), severity(severity), allow(allow), matcher(matcher) { }
	};

private:
	bool colored = true;
	std::vector<Rule> rules;
	std::vector<libv::observer_ref<std::ostream>> outputs;
	std::vector<libv::observer_ref<Logger>> outputChains;
	std::string format = "{severity} {thread_id} {module}: {message}  {file}:{line}\n";

private:
	bool notable(Severity severity, const std::string_view module) {
		for (const auto& rule : rules) {
			const auto matchResult = rule.match(severity, module);
			if (matchResult.matched)
				return matchResult.allow;
		}
		return true;
	}

public:
	Logger& allow() {
		rules.emplace_back("", Severity::Trace, true, &Rule::matcher_any);
		return *this;
	}
	Logger& allow(const std::string_view module) {
		rules.emplace_back(module, Severity::Trace, true, &Rule::matcher_module);
		return *this;
	}
	Logger& allow(const Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_equal);
		return *this;
	}
	Logger& allow(const std::string_view module, const Severity severity) {
		rules.emplace_back(module, severity, true, &Rule::matcher_module_severity_equal);
		return *this;
	}
	Logger& allow_above(const Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_above);
		return *this;
	}
	Logger& allow_above(const std::string_view module, const Severity severity) {
		rules.emplace_back(module, severity, true, &Rule::matcher_module_severity_above);
		return *this;
	}
	Logger& allow_below(const Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_below);
		return *this;
	}
	Logger& allow_below(const std::string_view module, const Severity severity) {
		rules.emplace_back(module, severity, true, &Rule::matcher_module_severity_below);
		return *this;
	}
	Logger& deny() {
		rules.emplace_back("", Severity::Trace, false, &Rule::matcher_any);
		return *this;
	}
	Logger& deny(const std::string_view module) {
		rules.emplace_back(module, Severity::Trace, false, &Rule::matcher_module);
		return *this;
	}
	Logger& deny(const Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_equal);
		return *this;
	}
	Logger& deny(const std::string_view module, const Severity severity) {
		rules.emplace_back(module, severity, false, &Rule::matcher_module_severity_equal);
		return *this;
	}
	Logger& deny_above(const Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_above);
		return *this;
	}
	Logger& deny_above(const std::string_view module, const Severity severity) {
		rules.emplace_back(module, severity, false, &Rule::matcher_module_severity_above);
		return *this;
	}
	Logger& deny_below(const Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_below);
		return *this;
	}
	Logger& deny_below(const std::string_view module, const Severity severity) {
		rules.emplace_back(module, severity, false, &Rule::matcher_module_severity_below);
		return *this;
	}

public:
	bool isColored() {
		return colored;
	}

	void color(bool colored) {
		this->colored = colored;
	}

private:
	template <typename = void>
	constexpr auto color_args(const std::string_view format) {
		// Note: This function could be implemented in a true constexpr way.
		//			Implementation could also be improved.
		std::string result;
		size_t size = format.size();
		result.reserve(size + 32);

		size_t open = 0;
		size_t close = 0;
		for (size_t i = 0; i < size; i++) {
			if (format[i] == '{') {
				if (i + 1 < size && format[i + 1] == '{') {
					result += "{{";
					++i;
				} else {
					result += "\u001B[36m{";
					++open;
				}
			} else if (format[i] == '}') {
				if (open == close && i + 1 < size && format[i + 1] == '}') {
					result += "}}";
					++i;
				} else {
					result += "}\u001B[0m";
					++close;
				}
			} else {
				result.push_back(format[i]);
			}
		}
		if (open != close)
			result += "\u001B[0m";
		return result;
	}

private:
	template <typename... Args>
	/*NEVER_INLINE*/ std::string formatMessage(
			const std::string_view fmt,
			const Args&... args) {
		std::string message;
		try {
			message = fmt::format(isColored() ? color_args(fmt) : fmt, args...);
		} catch (const fmt::format_error& ex) {
			message = fmt::format("Failed to format log message: \"{}\" reason: \"{}\" arguments:", fmt, ex.what());
			std::ostringstream argument_ss;
			((argument_ss << " \"" << args << "\","), ...);
			message += argument_ss.str();
		}
		return message;
	}

	/*NEVER_INLINE*/ void recordMessage(const std::string_view module, const Severity severity,
			const source_location loc, const std::string& message) {

		const auto entry = fmt::format(this->format,
				fmt::arg("message", message),
				fmt::arg("module", module),
				fmt::arg("severity", colored ? toColorString(severity) : toString(severity)),
				fmt::arg("line", loc.line()),
				fmt::arg("file", loc.file_name() + WISH_SHORT_PATH_CUTOFF),
				fmt::arg("function", loc.function_name()),
				fmt::arg("column", loc.column()),
				// fmt::arg("time", std::chrono::steady_clock::now()),
				fmt::arg("thread_id", thread_number())
		);

		for (auto& output : outputs)
			*output << entry << std::flush;
	}

public:
	template <typename... Args>
	/*NEVER_INLINE*/ void log(
			const std::string_view module,
			const Severity severity,
			const source_location loc,
			const std::string_view fmt,
			const Args&... args) {

		if (!notable(severity, module))
			return;

		auto message = formatMessage(fmt, args...);

		recordMessage(module, severity, loc, message);

		for (auto& output : outputChains)
			output->log(module, severity, loc, format, args...);
	}

public:
	friend inline std::ostream& operator<<(std::ostream& os, Logger& l) {
		l.outputs.emplace_back(&os);
		return os;
	}

	friend inline Logger& operator<<(Logger& os, Logger& l) {
		l.outputChains.emplace_back(&os);
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

struct LoggerModule {
private:
	Logger& log;
	const std::string_view module;

public:
	LoggerModule(Logger& log, const std::string_view module) : log(log), module(module) { }

public:
	/// Trace - Only when tracing the code and execution steps
	template <typename... Args>
    inline void trace(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Trace, fmt.location, fmt.value, args...);
	}

	/// Debug - Information that can be is diagnostically helpful
	template <typename... Args>
    inline void debug(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Debug, fmt.location, fmt.value, args...);
	}

	/// Info  - Generally useful information to log (service start/stop, configuration assumptions, etc). Info is always available but is not important under normal circumstances
	template <typename... Args>
    inline void info(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Info, fmt.location, fmt.value, args...);
	}

	/// Warn  - Anything that could cause application oddities, but for which automatically recovering is possible (switching to backup server, retrying an operation, etc)
	template <typename... Args>
    inline void warn(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Warn, fmt.location, fmt.value, args...);
	}

	/// Error - Any error which is fatal to the operation but not the service or application. Usually requires user intervention (missing file)
	template <typename... Args>
    inline void error(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Error, fmt.location, fmt.value, args...);
	}

	/// Fatal - Any error that is forcing a shutdown of the service or application to prevent (further) data loss or corruption
	template <typename... Args>
    inline void fatal(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Fatal, fmt.location, fmt.value, args...);
	}
};

// -------------------------------------------------------------------------------------------------

inline Logger logger;
inline LoggerModule log{libv::logger, "libv"};

// -------------------------------------------------------------------------------------------------

} // namespace libv
