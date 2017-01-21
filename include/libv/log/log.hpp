// File: Log.hpp, Created on 2014. december 1. 12:50, Author: Vader

#pragma once

// ext
#include <fmt/format.h>
#include <fmt/printf.h>
// libv
#include <libv/utility.hpp>
// std
#include <cstring>
#include <ostream>
#include <string>
#include <thread>
#include <vector>

//#include <mutex>

// TODO P1: catch exception of fmt::format on bad format
// TODO P3: Log system should provide an interface to name threads.
//			And / Or interact with worker thread name. AHHHAAA! Thread local string!
// TODO P4: Log system could get its own thread, later this can be useful for high stress debugs and
//			system analysis

// -------------------------------------------------------------------------------------------------
// Trace - Only when tracing the code and execution steps
// Debug - Information that can be is diagnostically helpful
// Info  - Generally useful information to log (service start/stop, configuration assumptions, etc). Info is always available but is not important under normal circumstances
// Warn  - Anything that could cause application oddities, but for which automatically recovering is possible (switching to backup server, retrying an operation, etc)
// Error - Any error which is fatal to the operation but not the service or application. Usually requires user intervention (missing file)
// Fatal - Any error that is forcing a shutdown of the service or application to prevent (further) data loss or corruption
// -------------------------------------------------------------------------------------------------


#define __LIBV_SHORT_FILE__ (std::strrchr(__FILE__, '/') + 1)

#define LIBV_POC ::libv::CodePosition{__LIBV_SHORT_FILE__, __FUNCTION__, __LINE__}

#define LIBV_TRACE(Module, ...) ::libv::log(LIBV_POC, ::libv::Trace, Module, __VA_ARGS__);
#define LIBV_DEBUG(Module, ...) ::libv::log(LIBV_POC, ::libv::Debug, Module, __VA_ARGS__);
#define LIBV_INFO( Module, ...) ::libv::log(LIBV_POC, ::libv::Info , Module, __VA_ARGS__);
#define LIBV_WARN( Module, ...) ::libv::log(LIBV_POC, ::libv::Warn , Module, __VA_ARGS__);
#define LIBV_ERROR(Module, ...) ::libv::log(LIBV_POC, ::libv::Error, Module, __VA_ARGS__);
#define LIBV_FATAL(Module, ...) ::libv::log(LIBV_POC, ::libv::Fatal, Module, __VA_ARGS__);

#define LIBV_LOG_TRACE(...) LIBV_TRACE("libv", __VA_ARGS__);
#define LIBV_LOG_DEBUG(...) LIBV_DEBUG("libv", __VA_ARGS__);
#define LIBV_LOG_INFO( ...) LIBV_INFO( "libv", __VA_ARGS__);
#define LIBV_LOG_WARN( ...) LIBV_WARN( "libv", __VA_ARGS__);
#define LIBV_LOG_ERROR(...) LIBV_ERROR("libv", __VA_ARGS__);
#define LIBV_LOG_FATAL(...) LIBV_FATAL("libv", __VA_ARGS__);

namespace libv {

// -------------------------------------------------------------------------------------------------

//LIBV_STRONG_TYPEDEF(int, Severity);
using Severity = int;

constexpr Severity Trace{1};
constexpr Severity Debug{2};
constexpr Severity Info{3};
constexpr Severity Warn{4};
constexpr Severity Error{5};
constexpr Severity Fatal{6};

// -------------------------------------------------------------------------------------------------

struct CodePosition {
	const char* file;
	const char* func;
	const int line;
};

// -------------------------------------------------------------------------------------------------

struct Logger;

// -------------------------------------------------------------------------------------------------

class Logger {

	struct MatchResult {
		bool matched;
		bool allow;
	};

	struct Rule {
		using MatcherFunction = bool (Rule::*)(Severity, const std::string&) const;
	private:
		std::string modul{""};
		Severity severity = Trace;
		bool allow;
		MatcherFunction matcher;
	private:
		bool isSubModul(const std::string& submodul) const {
			const auto isPrefix = submodul.compare(0, modul.size(), modul) == 0;
			const auto isEqualLenght = submodul.size() == modul.size();

			return isPrefix && (isEqualLenght
					|| (submodul.size() > modul.size() && submodul[modul.size()] == '.'));
		}
	public:
		bool matcher_any(Severity, const std::string&) const {
			return true;
		}
		bool matcher_modul(Severity, const std::string& modul) const {
			return isSubModul(modul);
		}
		bool matcher_severity_equal(Severity severity, const std::string&) const {
			return severity == this->severity;
		}
		bool matcher_modul_severity_equal(Severity severity, const std::string& modul) const {
			return isSubModul(modul) && severity == this->severity;
		}
		bool matcher_severity_above(Severity severity, const std::string&) const {
			return severity > this->severity;
		}
		bool matcher_modul_severity_above(Severity severity, const std::string& modul) const {
			return isSubModul(modul) && severity > this->severity;
		}
		bool matcher_severity_below(Severity severity, const std::string&) const {
			return severity < this->severity;
		}
		bool matcher_modul_severity_below(Severity severity, const std::string& modul) const {
			return isSubModul(modul) && severity < this->severity;
		}
	public:
		MatchResult match(Severity severity, const std::string& modul) const {
			return
			{
				(this->*matcher)(severity, modul), allow
			};
		}
	public:
		Rule(const std::string& modul,
				Severity severity,
				bool allow,
				MatcherFunction matcher) :
			modul(modul), severity(severity), allow(allow), matcher(matcher) { }
	};
private:
	std::vector<Rule> rules;
	std::vector<std::ostream*> outputs;
	//std::vector<Logger*> outputs;
//	std::string format = "{thread} {severity} [{module}] {message}\n";
	std::string format = "{thread} {severity} [{module}] {message} <{file}:{line}>\n";

private:
	bool notable(Severity severity, const std::string& modul) {
		for (const auto& rule : rules) {
			const auto matchResult = rule.match(severity, modul);
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
	Logger& allow(const std::string& modul) {
		rules.emplace_back(modul, Trace, true, &Rule::matcher_modul);
		return *this;
	}
	Logger& allow(Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_equal);
		return *this;
	}
	Logger& allow(const std::string& modul, Severity severity) {
		rules.emplace_back(modul, severity, true, &Rule::matcher_modul_severity_equal);
		return *this;
	}
	Logger& allow_above(Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_above);
		return *this;
	}
	Logger& allow_above(const std::string& modul, Severity severity) {
		rules.emplace_back(modul, severity, true, &Rule::matcher_modul_severity_above);
		return *this;
	}
	Logger& allow_below(Severity severity) {
		rules.emplace_back("", severity, true, &Rule::matcher_severity_below);
		return *this;
	}
	Logger& allow_below(const std::string& modul, Severity severity) {
		rules.emplace_back(modul, severity, true, &Rule::matcher_modul_severity_below);
		return *this;
	}
	Logger& deny() {
		rules.emplace_back("", Trace, false, &Rule::matcher_any);
		return *this;
	}
	Logger& deny(const std::string& modul) {
		rules.emplace_back(modul, Trace, false, &Rule::matcher_modul);
		return *this;
	}
	Logger& deny(Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_equal);
		return *this;
	}
	Logger& deny(const std::string& modul, Severity severity) {
		rules.emplace_back(modul, severity, false, &Rule::matcher_modul_severity_equal);
		return *this;
	}
	Logger& deny_above(Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_above);
		return *this;
	}
	Logger& deny_above(const std::string& modul, Severity severity) {
		rules.emplace_back(modul, severity, false, &Rule::matcher_modul_severity_above);
		return *this;
	}
	Logger& deny_below(Severity severity) {
		rules.emplace_back("", severity, false, &Rule::matcher_severity_below);
		return *this;
	}
	Logger& deny_below(const std::string& modul, Severity severity) {
		rules.emplace_back(modul, severity, false, &Rule::matcher_modul_severity_below);
		return *this;
	}

public:
	template<typename... Args>
	void log(CodePosition poc, Severity severity, const std::string& modul, const std::string& format, Args&&... args) {
		if (notable(severity, modul)) {
			const auto message = fmt::sprintf(format, std::forward<Args>(args)...);
			const auto record = fmt::format(this->format,
					fmt::arg("message", message),
					fmt::arg("module", modul),
					fmt::arg("severity", severity),
					fmt::arg("line", poc.line),
					fmt::arg("file", poc.file),
					fmt::arg("func", poc.func),
					fmt::arg("thread", get_this_thread_id())
					//...
					);
			for (auto& output : outputs) {
				*output << record << std::flush;
			}
		}
	}
	template<typename... Args>
	void operator()(CodePosition poc, Severity severity, const std::string& modul, const std::string& format, Args&&... args) {
		this->log(poc, severity, modul, format, std::forward<Args>(args)...);
	}
	friend std::ostream& operator<<(std::ostream& os, ::libv::Logger& l);
};

// -------------------------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, ::libv::Logger& l) {
	l.outputs.emplace_back(&os);
	return os;
}

// -------------------------------------------------------------------------------------------------

extern Logger log;

// -------------------------------------------------------------------------------------------------
inline std::string toString(Severity value) {
	switch (value) {
	case ::libv::Trace: return "Trace";
	case ::libv::Debug: return "Debug";
	case ::libv::Info: return "Info";
	case ::libv::Warn: return "Warn";
	case ::libv::Error: return "Error";
	case ::libv::Fatal: return "Fatal";
	default: return "Undef";
	}
}

} // namespace libv
