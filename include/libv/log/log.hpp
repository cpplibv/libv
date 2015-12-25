// File: Log.hpp, Created on 2014. december 1. 12:50, Author: Vader

#pragma once

// ext
#include <cppformat/format.h>
// libv
#include <libv/utility.hpp>
// std
#include <cstring>
#include <ostream>
#include <string>
#include <vector>

//#include <mutex>

//TODO P1: catch exception of fmt::format on bad format
//TODO P3: Log system should provide an interface to name threads.
//			And / Or interact with worker thread name. AHHHAAA! Thread local string!
//TODO P4: Log system could get its own thread, later this can be useful for high stress debugs and
//			system analysis

// -------------------------------------------------------------------------------------------------
// Trace - Only when I would be "tracing" the code and trying to find one part of a func specifically
// Debug - Information that is diagnostically helpful to people more than just developers (IT, sysadmins, etc)
// Info - Generally useful information to log (service start/stop, configuration assumptions, etc). Info I want to always have available but usually dont care about under normal circumstances. This is my out-of-the-box config level
// Warn - Anything that can potentially cause application oddities, but for which I am automatically recoverring (such as switching from a primary to backup server, retrying an operation, missing secondary data, etc)
// Error - Any error which is fatal to the operation but not the service or application (cant open a required file, missing data, etc). These errors will force user (administrator, or direct user) intervention. These are usually reserved (in my apps) for incorrect connection strings, missing services, etc.
// Fatal - Any error that is forcing a shutdown of the service or application to prevent data loss (or further data loss). I reserve these only for the most heinous errors and situations where there is guaranteed to have been data corruption or loss.
// -------------------------------------------------------------------------------------------------


#define __VFILENAME__ (std::strrchr(__FILE__, '/') + 1)

#define LIBV_POC ::libv::CodePosition{__VFILENAME__, __FUNCTION__, __LINE__}

#define LIBV_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv", __VA_ARGS__);
#define LIBV_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv", __VA_ARGS__);
#define LIBV_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv", __VA_ARGS__);
#define LIBV_WARN( ...) ::libv::log(LIBV_POC, ::libv::Debug, "libv", __VA_ARGS__);
#define LIBV_ERROR(...) ::libv::log(LIBV_POC, ::libv::Warn , "libv", __VA_ARGS__);
#define LIBV_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv", __VA_ARGS__);

namespace libv {

// -------------------------------------------------------------------------------------------------

//LIBV_STRONG_TYPEDEF(int, Severity);
using Severity = int;

constexpr Severity Trace{100};
constexpr Severity Debug{200};
constexpr Severity Info{300};
constexpr Severity Warn{400};
constexpr Severity Error{500};
constexpr Severity Fatal{600};

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
	std::string format = "{severity} [{modul}] {message}\n";

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
					fmt::arg("modul", modul),
					fmt::arg("severity", severity),
					fmt::arg("line", poc.line),
					fmt::arg("file", poc.file),
					fmt::arg("func", poc.func) //...
					);
			for (auto& output : outputs) {
				*output << record;
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

} //namespace libv
