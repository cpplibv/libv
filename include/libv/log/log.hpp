// File: Log.hpp, Created on 2014. december 1. 12:50, Author: Vader

#pragma once

// ext
#include <fmt/format.h>
#include <fmt/printf.h>
// libv
#include <libv/thread/number.hpp>
#include <libv/utility/code_position.hpp>
#include <libv/utility/enum.hpp>
// std
#include <ostream>
#include <sstream>
#include <string>
#include <vector>


// TODO P3: Log system should provide an interface to name threads. And it most likely will not be
//			thread_local but a map inside the log system, or better a "specific log message".
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

#define LIBV_LOG_BASE_TRACE(Module, ...) ::libv::call_log(::libv::log, LIBV_POC, ::libv::Trace, Module, __VA_ARGS__)
#define LIBV_LOG_BASE_DEBUG(Module, ...) ::libv::call_log(::libv::log, LIBV_POC, ::libv::Debug, Module, __VA_ARGS__)
#define LIBV_LOG_BASE_INFO( Module, ...) ::libv::call_log(::libv::log, LIBV_POC, ::libv::Info , Module, __VA_ARGS__)
#define LIBV_LOG_BASE_WARN( Module, ...) ::libv::call_log(::libv::log, LIBV_POC, ::libv::Warn , Module, __VA_ARGS__)
#define LIBV_LOG_BASE_ERROR(Module, ...) ::libv::call_log(::libv::log, LIBV_POC, ::libv::Error, Module, __VA_ARGS__)
#define LIBV_LOG_BASE_FATAL(Module, ...) ::libv::call_log(::libv::log, LIBV_POC, ::libv::Fatal, Module, __VA_ARGS__)

#define LIBV_LOG_LIBV_TRACE(...) LIBV_LOG_BASE_TRACE("libv", __VA_ARGS__)
#define LIBV_LOG_LIBV_DEBUG(...) LIBV_LOG_BASE_DEBUG("libv", __VA_ARGS__)
#define LIBV_LOG_LIBV_INFO( ...) LIBV_LOG_BASE_INFO( "libv", __VA_ARGS__)
#define LIBV_LOG_LIBV_WARN( ...) LIBV_LOG_BASE_WARN( "libv", __VA_ARGS__)
#define LIBV_LOG_LIBV_ERROR(...) LIBV_LOG_BASE_ERROR("libv", __VA_ARGS__)
#define LIBV_LOG_LIBV_FATAL(...) LIBV_LOG_BASE_FATAL("libv", __VA_ARGS__)

namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: enum, or strong type LIBV_STRONG_TYPEDEF(int, Severity);
using Severity = int;

constexpr Severity Trace{1};
constexpr Severity Debug{2};
constexpr Severity Info{3};
constexpr Severity Warn{4};
constexpr Severity Error{5};
constexpr Severity Fatal{6};

inline std::string toString(Severity value) {
	switch (value) {
	case ::libv::Trace: return "Trace";
	case ::libv::Debug: return "Debug";
	case ::libv::Info: return "Info ";
	case ::libv::Warn: return "Warn ";
	case ::libv::Error: return "Error";
	case ::libv::Fatal: return "Fatal";
	default: return "Undefined";
	}
}

inline std::string toColorString(Severity value) {
	switch (value) {
	case ::libv::Trace: return "\u001B[37mTrace\u001B[0m";
	case ::libv::Debug: return "\u001B[37mDebug\u001B[0m";
	case ::libv::Info: return  "\u001B[32mInfo \u001B[0m";
	case ::libv::Warn: return  "\u001B[33mWarn \u001B[0m";
	case ::libv::Error: return "\u001B[31mError\u001B[0m";
	case ::libv::Fatal: return "\u001B[35mFatal\u001B[0m";
	default: return "Undefined";
	}
}

// -------------------------------------------------------------------------------------------------

struct tag_format2 {};

template <typename Format>
constexpr auto color_args(Format&& format) {
	// Note: This function could be implemented in a true constexpr way is the call makes it possible, and yes I know how ugly this is.
	std::string result;
	const auto size = [&] {
		if constexpr (std::is_same_v<std::decay_t<Format>, const char*>)
			return std::strlen(format);
		else
			return std::size(format);
	}();
	result.reserve(size + 32);

	size_t open = 0;
	size_t close = 0;
	for (size_t i = 0; i < size; ++i) {
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

template <typename Log, typename Module, typename Format, typename... Args>
constexpr inline void call_log(Log&& log, CodePosition poc, Severity severity, Module&& module, Format&& format, Args&&... args) {
	log(poc, severity, std::forward<Module>(module), log.isColored() ? color_args(std::forward<Format>(format)) : std::forward<Format>(format), std::forward<Args>(args)...);
}

// -------------------------------------------------------------------------------------------------

class Logger {

	struct MatchResult {
		bool matched;
		bool allow;
	};

	struct Rule {
		using MatcherFunction = bool (Rule::*)(Severity, const std::string&) const;
	private:
		std::string module{""};
		Severity severity = Trace;
		bool allow;
		MatcherFunction matcher;
	private:
		bool isSubModul(const std::string& submodule) const {
			const auto isPrefix = submodule.compare(0, module.size(), module) == 0;
			const auto isEqualLenght = submodule.size() == module.size();

			return isPrefix && (isEqualLenght
					|| (submodule.size() > module.size() && submodule[module.size()] == '.'));
		}
	public:
		bool matcher_any(Severity, const std::string&) const {
			return true;
		}
		bool matcher_module(Severity, const std::string& module) const {
			return isSubModul(module);
		}
		bool matcher_severity_equal(Severity severity, const std::string&) const {
			return severity == this->severity;
		}
		bool matcher_module_severity_equal(Severity severity, const std::string& module) const {
			return isSubModul(module) && severity == this->severity;
		}
		bool matcher_severity_above(Severity severity, const std::string&) const {
			return severity > this->severity;
		}
		bool matcher_module_severity_above(Severity severity, const std::string& module) const {
			return isSubModul(module) && severity > this->severity;
		}
		bool matcher_severity_below(Severity severity, const std::string&) const {
			return severity < this->severity;
		}
		bool matcher_module_severity_below(Severity severity, const std::string& module) const {
			return isSubModul(module) && severity < this->severity;
		}
	public:
		MatchResult match(Severity severity, const std::string& module) const {
			return {(this->*matcher)(severity, module), allow};
		}
	public:
		Rule(const std::string& module,
				Severity severity,
				bool allow,
				MatcherFunction matcher) :
			module(module), severity(severity), allow(allow), matcher(matcher) { }
	};
private:
	bool colored = true;
	std::vector<Rule> rules;
	std::vector<std::ostream*> outputs;
	//std::vector<Logger*> outputs;
	std::string format = "{severity} {thread_id} {module}: {message} <{file}:{line}>\n";

private:
	bool notable(Severity severity, const std::string& module) {
		for (const auto& rule : rules) {
			const auto matchResult = rule.match(severity, module);
			if (matchResult.matched)
				return matchResult.allow;
		}
		return true;
	}

public:
	Logger& allow() {
		rules.emplace_back("", Trace, true, &Rule::matcher_any);
		return *this;
	}
	Logger& allow(const std::string& module) {
		rules.emplace_back(module, Trace, true, &Rule::matcher_module);
		return *this;
	}
	Logger& allow(Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_equal);
		return *this;
	}
	Logger& allow(const std::string& module, Severity severity) {
		rules.emplace_back(module, severity, true, &Rule::matcher_module_severity_equal);
		return *this;
	}
	Logger& allow_above(Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_above);
		return *this;
	}
	Logger& allow_above(const std::string& module, Severity severity) {
		rules.emplace_back(module, severity, true, &Rule::matcher_module_severity_above);
		return *this;
	}
	Logger& allow_below(Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_below);
		return *this;
	}
	Logger& allow_below(const std::string& module, Severity severity) {
		rules.emplace_back(module, severity, true, &Rule::matcher_module_severity_below);
		return *this;
	}
	Logger& deny() {
		rules.emplace_back("", Trace, false, &Rule::matcher_any);
		return *this;
	}
	Logger& deny(const std::string& module) {
		rules.emplace_back(module, Trace, false, &Rule::matcher_module);
		return *this;
	}
	Logger& deny(Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_equal);
		return *this;
	}
	Logger& deny(const std::string& module, Severity severity) {
		rules.emplace_back(module, severity, false, &Rule::matcher_module_severity_equal);
		return *this;
	}
	Logger& deny_above(Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_above);
		return *this;
	}
	Logger& deny_above(const std::string& module, Severity severity) {
		rules.emplace_back(module, severity, false, &Rule::matcher_module_severity_above);
		return *this;
	}
	Logger& deny_below(Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_below);
		return *this;
	}
	Logger& deny_below(const std::string& module, Severity severity) {
		rules.emplace_back(module, severity, false, &Rule::matcher_module_severity_below);
		return *this;
	}

public:
	bool isColored() {
		return colored;
	}

public:
	// TODO P2: Never inline
	template <typename... Args>
	void log(CodePosition poc, Severity severity, const std::string& module, const std::string& format, Args&&... args) {
		if (notable(severity, module)) {
			std::string message;
			try {
				message = fmt::format(format, std::forward<Args>(args)...);
			} catch (const fmt::FormatError& ex) {
				message = fmt::format("Failed to format log message: \"{}\" reason: \"{}\" arguments:", format, ex.what());
				std::ostringstream argument_ss;
				((argument_ss << " \"" << std::forward<Args>(args) << "\","), ...);
				message += argument_ss.str();
			}

			const auto record = fmt::format(this->format,
					fmt::arg("message", message),
					fmt::arg("module", module),
					fmt::arg("severity", colored ? toColorString(severity) : toString(severity)),
					fmt::arg("line", poc.line),
					fmt::arg("file", poc.file),
					fmt::arg("func", poc.func),
					fmt::arg("thread_id", thread_number())
					);
			for (auto& output : outputs) {
				*output << record << std::flush;
			}
		}
	}
	template <typename... Args>
	void operator()(CodePosition poc, Severity severity, const std::string& module, const std::string& format, Args&&... args) {
		this->log(poc, severity, module, format, std::forward<Args>(args)...);
	}
	friend std::ostream& operator<<(std::ostream& os, ::libv::Logger& l);
};

// -------------------------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, ::libv::Logger& l) {
	l.outputs.emplace_back(&os);
	return os;
}

// -------------------------------------------------------------------------------------------------

inline Logger log;

// -------------------------------------------------------------------------------------------------

} // namespace libv
