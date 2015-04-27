// File: Log.hpp, Created on 2014. december 1. 12:50, Author: Vader

#pragma once

#include <chrono>
#include <cstring>
#include <mutex>
#include <ostream>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include <vl/string.hpp>

#define __VFILENAME__ (std::strrchr(__FILE__, '/') + 1)

#define VLOG_SEVERITY_TRACE 1
#define VLOG_SEVERITY_DEBUG 2
#define VLOG_SEVERITY_INFO  3
#define VLOG_SEVERITY_NOTIF 4
#define VLOG_SEVERITY_WARN  5
#define VLOG_SEVERITY_ERROR 6
#define VLOG_SEVERITY_FATAL 7

#define VLOG_TRACE(LOG, FORMAT, ARGS...) (LOG).log(VLOG_SEVERITY_TRACE, vl::format((FORMAT), ##ARGS), __VFILENAME__, __FUNCTION__, __LINE__);
#define VLOG_DEBUG(LOG, FORMAT, ARGS...) (LOG).log(VLOG_SEVERITY_DEBUG, vl::format((FORMAT), ##ARGS), __VFILENAME__, __FUNCTION__, __LINE__);
#define VLOG_INFO( LOG, FORMAT, ARGS...) (LOG).log(VLOG_SEVERITY_INFO , vl::format((FORMAT), ##ARGS), __VFILENAME__, __FUNCTION__, __LINE__);
#define VLOG_NOTIF(LOG, FORMAT, ARGS...) (LOG).log(VLOG_SEVERITY_NOTIF, vl::format((FORMAT), ##ARGS), __VFILENAME__, __FUNCTION__, __LINE__);
#define VLOG_WARN( LOG, FORMAT, ARGS...) (LOG).log(VLOG_SEVERITY_WARN , vl::format((FORMAT), ##ARGS), __VFILENAME__, __FUNCTION__, __LINE__);
#define VLOG_ERROR(LOG, FORMAT, ARGS...) (LOG).log(VLOG_SEVERITY_ERROR, vl::format((FORMAT), ##ARGS), __VFILENAME__, __FUNCTION__, __LINE__);
#define VLOG_FATAL(LOG, FORMAT, ARGS...) (LOG).log(VLOG_SEVERITY_FATAL, vl::format((FORMAT), ##ARGS), __VFILENAME__, __FUNCTION__, __LINE__);

namespace vl {
class Log;
using Severity = int;

class LogRecord {
private:
	std::chrono::steady_clock::time_point time;
	//		Time time;
	const Severity severity;
	std::string message;
	const char* file;
	const char* function;
	const int line;
	std::vector<const Log*> logSource;
public:
	LogRecord(const Severity& severity, const std::string& message, const char* file, const char* function, const int line) :
		time(std::chrono::steady_clock::now()),
		severity(severity),
		message(message),
		file(file),
		function(function),
		line(line) { }
	void addModul(const Log& l) {
		logSource.emplace_back(&l);
	}
	friend std::ostream& operator<<(std::ostream& os, const LogRecord& r);
};

class Log {
public:
	static const Severity SEVERITY_TRACE = 1;
	static const Severity SEVERITY_DEBUG = 2;
	static const Severity SEVERITY_INFO = 3;
	static const Severity SEVERITY_NOTIF = 4;
	static const Severity SEVERITY_WARN = 5;
	static const Severity SEVERITY_ERROR = 6;
	static const Severity SEVERITY_FATAL = 7;
private:
	std::set<std::ostream*> ostreams;
	std::set<Log*> chainOutput;
	const std::string modul;
public:
	Log(const std::string& modul) : modul(modul) { }
	Log(const std::string& modul, Log& log) : modul(modul) {
		output(log);
	}
public:
	const std::string& getName() const {
		return modul;
	}
	void output(Log& log) {
		chainOutput.insert(&log);
	}
	void output(std::ostream& ostream) {
		ostreams.insert(&ostream);
	}
	void detachAllStream() {
		ostreams.clear();
	}
public:
	void broadcast(LogRecord& record) {
		record.addModul(*this);
		for (auto& item : chainOutput) {
			item->broadcast(record);
		}
		for (auto& item : ostreams) {
			(*item) << record;
		}
	}
	void log(const Severity severity, const std::string& message, const char* file, const char* function, const int line) {
		LogRecord record(severity, message, file, function, line);
		broadcast(record);
	}
};

inline Log& log() {
	static Log log("vl");
	return log;
}

inline std::ostream& operator<<(std::ostream& os, const LogRecord& r) {
	static std::mutex guard_m;
	std::unique_lock<std::mutex> lock(guard_m);

	//		os << "" << r.time.time_of_day() << " ";
	os << "s" << r.severity << " ";
	os << "t" << std::this_thread::get_id() << " ";
	for (int i = r.logSource.size() - 1; i >= 0; i--)
		os << "[" << r.logSource[i]->getName() << "]";
	if (r.severity >= VLOG_SEVERITY_NOTIF || r.severity == VLOG_SEVERITY_TRACE)
		os << " " << r.file << '@' << r.function << ':' << r.line << " -";
	os << ' ' << r.message << '\n' << std::flush;
	return os;
}

} //namespace vl

//std::string vuiGetSeverityString(VUISeverity value) {
//			switch (value) {
//				case VUI_SEVERITY_TRACE: return "Trace";
//				case VUI_SEVERITY_DEBUG: return "Debug";
//				case VUI_SEVERITY_INFO: return "Info ";
//				case VUI_SEVERITY_NOTIF: return "Notif";
//				case VUI_SEVERITY_WARN: return "Warn ";
//				case VUI_SEVERITY_ERROR: return "Error";
//				case VUI_SEVERITY_FATAL: return "Fatal";
//				case VUI_SEVERITY_UNDEF: default: return "Undef";
//			}
//		}