// File: file_watcher.hpp Author: Vader Created on 2017. július 8., 23:19

#pragma once

// ext
#include <efsw/efsw.hpp>
// std
#include <functional>
#include <map>
#include <mutex>


namespace libv {

// -------------------------------------------------------------------------------------------------

class FileWatcher : public efsw::FileWatchListener {
	efsw::FileWatcher watcher;
	std::map<efsw::WatchID, std::function<void()>> directories;
	std::mutex mutex;

	/// @param watchid The watch id for the directory
	/// @param dir The directory
	/// @param filename The filename that was accessed (not full path)
	/// @param action Action that was performed
	/// @param oldFilename The name of the file or directory moved
	virtual void handleFileAction(efsw::WatchID id, const std::string&, const std::string&, efsw::Action, std::string) override {
		std::unique_lock lock(mutex);
		directories[id]();
	}

public:
	FileWatcher() {
	}

	void watch_directory(const std::string& path, std::function<void()> callback) {
		std::unique_lock lock(mutex);
		directories.emplace(watcher.addWatch(path, this, true), std::move(callback));
		watcher.watch();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
