// Project: libv.log, File: src/libv/log/log.hpp

#pragma once

// ext
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
// libv
#include <libv/mt/thread_number.hpp>
#include <libv/utility/memory/observer_ref.hpp>
#include <libv/utility/source_location.hpp>
// std
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>


// TODO P3: Log system should provide an interface to name threads. And it most likely will not be
//			thread_local but a map inside the log system, or better a "specific log message".
// TODO P4: Consider warn / fail / log unused arguments too, (or not, as this requires format parsing due to {1} indexing)
// TODO P4: Log system could get its own thread, later this can be useful for high stress debugs and
//			system analysis, or binary logging...
// TODO P4: {line_gap} assign the rest of the command line into this gap, this should also be supported in the messages in some form
//			Most likely will be integrated into the serialization and output-formatter system
// TODO P5: Separate flag for argument coloring
// TODO P5: Option for full line color (mostly for warning+ levels)
// TODO P5: Better generic color handling, remove hard-coded color strings
// TODO P5: Reorganize toString / toColorString / toColorShortString, generalize it
// TODO P5: Specify / refine requirements for warning / error / fatal log levels
//			warning - what happened, what caused it, how the recovery was done
//			error - what happened, what caused it, what should be done by the user
//			fatal - what happened, what caused it, what should be done by the user
// IDEA: Format string per route; most likely will be integrated into the serialization and output-formatter system
//		libv::log.route_below(libv::Info, std::cout, "{severity} {thread_id} {module}: {message} <{file}:{line}>\n");
//		libv::log.route(std::cout, "{severity} {module}: {message}\n");
// TODO P5: Improve var template instantiations by fmt::make_args_checked:
//		void vlog(const char* file, int line, fmt::string_view format,
//				fmt::format_args args) {
//			fmt::print("{}: {}: ", file, line);
//			fmt::vprint(format, args);
//		}
//
//		template <typename S, typename... Args>
//		void log(const char* file, int line, const S& format, Args&&... args) {
//			vlog(file, line, format,
//					fmt::make_args_checked<Args...>(format, args...));
//		}
//
// TODO P5: Design a unified log line structure
//		 Warn: This happened. It was unexpected because. This was done to recover. [Recommended user action.]

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
	[[nodiscard]] constexpr inline std::string_view toString(Severity value) noexcept {
		switch (value) {
		case Severity::Trace: return "Trace";
		case Severity::Debug: return "Debug";
		case Severity::Info: return "Info ";
		case Severity::Warn: return "Warn ";
		case Severity::Error: return "Error";
		case Severity::Fatal: return "Fatal";
		}
		return "Undefined";
	}

	[[nodiscard]] constexpr inline std::string_view toColorString(Severity value) noexcept {
		switch (value) {
		case Severity::Trace: return "\u001B[37mTrace\u001B[0m";
		case Severity::Debug: return "\u001B[37mDebug\u001B[0m";
		case Severity::Info: return  "\u001B[32mInfo \u001B[0m";
		case Severity::Warn: return  "\u001B[33mWarn \u001B[0m";
		case Severity::Error: return "\u001B[31mError\u001B[0m";
		case Severity::Fatal: return "\u001B[35mFatal\u001B[0m";
		}
		return "Undefined";
	}

	[[nodiscard]] constexpr inline std::string_view toColorShortString(Severity value) noexcept {
		switch (value) {
		case Severity::Trace: return "\u001B[37mT\u001B[0m";
		case Severity::Debug: return "\u001B[37mD\u001B[0m";
		case Severity::Info: return  "\u001B[32mI\u001B[0m";
		case Severity::Warn: return  "\u001B[33mW\u001B[0m";
		case Severity::Error: return "\u001B[31mE\u001B[0m";
		case Severity::Fatal: return "\u001B[35mF\u001B[0m";
		}
		return "?";
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
		[[nodiscard]] bool isSubModule(const std::string_view submodule) const {
			const auto isPrefix = submodule.compare(0, module.size(), module) == 0;
			const auto isEqualLenght = submodule.size() == module.size();

			return isPrefix && (isEqualLenght
					|| (submodule.size() > module.size() && submodule[module.size()] == '.'));
		}

	public:
		[[nodiscard]] bool matcher_any(Severity, const std::string_view) const {
			return true;
		}
		[[nodiscard]] bool matcher_module(Severity, const std::string_view module) const {
			return isSubModule(module);
		}
		[[nodiscard]] bool matcher_severity_equal(Severity severity_, const std::string_view) const {
			return severity_ == severity;
		}
		[[nodiscard]] bool matcher_module_severity_equal(Severity severity_, const std::string_view module) const {
			return isSubModule(module) && severity_ == severity;
		}
		[[nodiscard]] bool matcher_severity_above(Severity severity_, const std::string_view) const {
			return severity_ > severity;
		}
		[[nodiscard]] bool matcher_module_severity_above(Severity severity_, const std::string_view module) const {
			return isSubModule(module) && severity_ > severity;
		}
		[[nodiscard]] bool matcher_severity_below(Severity severity_, const std::string_view) const {
			return severity_ < severity;
		}
		[[nodiscard]] bool matcher_module_severity_below(Severity severity_, const std::string_view module) const {
			return isSubModule(module) && severity_ < severity;
		}

	public:
		[[nodiscard]] MatchResult match(Severity severity, const std::string_view module) const {
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
	bool levelShort_ = true;
	std::vector<Rule> rules;
	std::vector<libv::observer_ref<std::ostream>> outputs;
	std::vector<libv::observer_ref<Logger>> outputChains;
	std::string format = "{severity} {thread_id} {module}: {message}, {file}:{line}\n";

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

	bool isLevelShort() {
		return levelShort_;
	}

	void levelShort(bool levelShort) {
		this->levelShort_ = levelShort;
	}

private:
	constexpr auto color_args(const std::string_view format) {
		// Note: This function could be implemented in a true constexpr way.
		//			Implementation could also be improved.
		std::string result;
		std::size_t size = format.size();
		result.reserve(size + 32);

		std::size_t open = 0;
		std::size_t close = 0;
		for (std::size_t i = 0; i < size; i++) {
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
	template <typename OS, typename Args>
	inline void ostream_if_possible(OS& os, const Args& arg) {
		if constexpr (requires { os << arg; }) {
			os << " \"" << arg << "\",";
		} else {
			os << " ?,";
		}
	}

	template <typename... Args>
	/*NEVER_INLINE*/ std::string formatMessage(
			const std::string_view fmt,
			const Args&... args) {
		std::string message;
		try {
			message = fmt::format(fmt::runtime(isColored() ? color_args(fmt) : fmt), args...);
		} catch (const fmt::format_error& ex) {
			message = fmt::format("Failed to format log message: \"{}\" reason: \"{}\" arguments:", fmt, ex.what());
			std::ostringstream argument_ss;
			(ostream_if_possible(argument_ss, args), ...);
//			((argument_ss << " \"" << args << "\","), ...);
			message += argument_ss.str();
		}
		return message;
	}

	/*NEVER_INLINE*/ void recordMessage(const std::string_view module, const Severity severity,
			const source_location loc, const std::string& message) {

		// constexpr bool force_generic_path_in_log = false;
		//
		// [[maybe_unused]] static thread_local std::string generic_file_path_tmp;
		// if constexpr (force_generic_path_in_log) {
		// 	generic_file_path_tmp = loc.file_name() + WISH_SHORT_PATH_CUTOFF;
		// 	std::replace(generic_file_path_tmp.begin(), generic_file_path_tmp.end(), '\\', '/');
		// }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
		const auto entry = fmt::format(fmt::runtime(this->format),
				fmt::arg("message", message),
				fmt::arg("module", module),
				fmt::arg("severity",
						!colored ? toString(severity) :
						levelShort_ ? toColorShortString(severity) : toColorString(severity)),
				fmt::arg("line", loc.line()),
				fmt::arg("file",
						// force_generic_path_in_log ?
						// generic_file_path_tmp.c_str() :
						loc.file_name() + WISH_SHORT_PATH_CUTOFF),
				fmt::arg("function", loc.function_name()),
				fmt::arg("column", loc.column()),
				// fmt::arg("time", std::chrono::steady_clock::now()),
				fmt::arg("thread_id", thread_number())
		);
#pragma GCC diagnostic pop

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

		if (outputChains.empty() && outputs.empty())
			return;

		if (!notable(severity, module))
			return;

		auto message = formatMessage(fmt, args...);

		recordMessage(module, severity, loc, message);

		for (auto& output : outputChains)
			output->log(module, severity, loc, format, args...);
	}

public:
	void setFormat(std::string format) {
		this->format = std::move(format);
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
	LoggerModule(Logger& log, const std::string_view module) noexcept : log(log), module(module) { }

public:
	/// Level 1/6 log.
	/// Trace - Only when tracing the code and execution steps
	template <typename... Args>
	inline void trace(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Trace, fmt.location, fmt.value, args...);
	}

	/// Level 2/6 log.
	/// Debug - Information that can be is diagnostically helpful
	template <typename... Args>
	inline void debug(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Debug, fmt.location, fmt.value, args...);
	}

	/// Level 3/6 log.
	/// Info  - Generally useful information to log (service start/stop, configuration assumptions, etc). Info is always available but is not important under normal circumstances
	template <typename... Args>
	inline void info(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Info, fmt.location, fmt.value, args...);
	}

	/// Level 4/6 log.
	/// Warn  - Anything that could cause application oddities, but for which automatically recovering is possible (switching to backup server, retrying an operation, etc)
	template <typename... Args>
	inline void warn(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Warn, fmt.location, fmt.value, args...);
	}

	/// Level 5/6 log.
	/// Error - Any error which is fatal to the operation but not the service or application. Usually requires user intervention (missing file)
	template <typename... Args>
	inline void error(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Error, fmt.location, fmt.value, args...);
	}

	/// Level 6/6 log.
	/// Fatal - Any error that is forcing a shutdown of the service or application to prevent (further) data loss or corruption
	template <typename... Args>
	inline void fatal(const with_source_location<const std::string_view> fmt, const Args&... args) {
		log.log(module, Logger::Severity::Fatal, fmt.location, fmt.value, args...);
	}

public:
	/// Level 1/6 log.
	/// Trace - Only when tracing the code and execution steps
	template <typename Bool, typename... Args>
	inline void trace_if(const Bool& condition, const with_source_location<const std::string_view> fmt, const Args&... args) {
		if (condition)
			log.log(module, Logger::Severity::Trace, fmt.location, fmt.value, args...);
	}

	/// Level 2/6 log.
	/// Debug - Information that can be is diagnostically helpful
	template <typename Bool, typename... Args>
	inline void debug_if(const Bool& condition, const with_source_location<const std::string_view> fmt, const Args&... args) {
		if (condition)
			log.log(module, Logger::Severity::Debug, fmt.location, fmt.value, args...);
	}

	/// Level 3/6 log.
	/// Info  - Generally useful information to log (service start/stop, configuration assumptions, etc). Info is always available but is not important under normal circumstances
	template <typename Bool, typename... Args>
	inline void info_if(const Bool& condition, const with_source_location<const std::string_view> fmt, const Args&... args) {
		if (condition)
			log.log(module, Logger::Severity::Info, fmt.location, fmt.value, args...);
	}

	/// Level 4/6 log.
	/// Warn  - Anything that could cause application oddities, but for which automatically recovering is possible (switching to backup server, retrying an operation, etc)
	template <typename Bool, typename... Args>
	inline void warn_if(const Bool& condition, const with_source_location<const std::string_view> fmt, const Args&... args) {
		if (condition)
			log.log(module, Logger::Severity::Warn, fmt.location, fmt.value, args...);
	}

	/// Level 5/6 log.
	/// Error - Any error which is fatal to the operation but not the service or application. Usually requires user intervention (missing file)
	template <typename Bool, typename... Args>
	inline void error_if(const Bool& condition, const with_source_location<const std::string_view> fmt, const Args&... args) {
		if (condition)
			log.log(module, Logger::Severity::Error, fmt.location, fmt.value, args...);
	}

	/// Level 6/6 log.
	/// Fatal - Any error that is forcing a shutdown of the service or application to prevent (further) data loss or corruption
	template <typename Bool, typename... Args>
	inline void fatal_if(const Bool& condition, const with_source_location<const std::string_view> fmt, const Args&... args) {
		if (condition)
			log.log(module, Logger::Severity::Fatal, fmt.location, fmt.value, args...);
	}
};

// -------------------------------------------------------------------------------------------------

inline Logger logger_stream;
inline LoggerModule log{libv::logger_stream, "libv"};

// -------------------------------------------------------------------------------------------------

} // namespace libv
