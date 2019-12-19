// File: file_watcher.hpp Author: Vader Created on 2017. július 8., 23:19

#pragma once

// std
#include <filesystem>
#include <functional>
#include <iosfwd>
#include <memory>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

class ImplFileWatcher;

// -------------------------------------------------------------------------------------------------

//enum class Action : uint32_t {
enum class Action {
	create = 1,
	remove = 2,
	modify = 3,
	rename = 4,
};

std::ostream& operator<<(std::ostream& os, const Action& event);

//class WatchToken {
struct WatchToken {
//	friend class ImplFileWatcher;
//	friend class FileWatcher;

	// <<< P4: hide / improve
	void* id;

	constexpr inline WatchToken(void* id) noexcept : id(id) { }
};

struct Event {
//	WatchToken token;
	Action action;
	std::filesystem::path path;
	std::filesystem::path old_path;

//	std::string dir; /// The directory
//	std::string filename; /// The filename that was accessed (not full path)
//	std::string old_path; /// The name of the file or directory moved

	friend std::ostream& operator<<(std::ostream& os, const Event& event);
};

class FileWatcher {
public:
	using callback_type = std::function<void(const Event&)>;
	using token_type = WatchToken;

private:
	std::unique_ptr<ImplFileWatcher> self;

public:
	FileWatcher();
	~FileWatcher();

public:
	token_type subscribe_file(std::filesystem::path path, callback_type callback);
	token_type subscribe_directory(std::filesystem::path path, callback_type callback);

//	void unsubscribe(std::filesystem::path path);
	void unsubscribe(token_type token);
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
