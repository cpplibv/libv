// Project: libv.fsw, File: src/libv/fsw/watcher.lpp, Author: Császár Mátyás [Vader]

#pragma once

// hpp
#include <libv/fsw/watcher.hpp>
// ext
#include <efsw/efsw.hpp>
// std
//#include <filesystem>
#include <functional>
#include <mutex>
#include <optional>
#include <unordered_map>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

struct CallbackDir {
	bool uses_relative_path;
	// NOTE: Pointer stability is used by watch tokens, better solutions are possible
	std::unique_ptr<Watcher::callback_type> callback;

	CallbackDir(bool uses_relative_path, std::unique_ptr<Watcher::callback_type>&& callback) :
		uses_relative_path(uses_relative_path),
		callback(std::move(callback)) { }
};

using CallbackFile = CallbackDir;

struct DirectoryCluster {
	efsw::WatchID watcher_used = 0;

	std::vector<CallbackDir> callbacks;
	std::unordered_map<std::string, std::vector<CallbackFile>> file_callbacks;
};

struct WatcherEntry {
	efsw::WatchID watchID;
	uint32_t ref_count;

	std::filesystem::path path;
};

// -------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
class ImplWatcher : public efsw::FileWatchListener {
#pragma GCC diagnostic pop
public:
	std::mutex mutex;

	std::vector<WatcherEntry> watcher_entries;
	std::unordered_map<std::string, DirectoryCluster> directories;

	std::optional<efsw::FileWatcher> efsw_watcher;

private:
	/// @param watchid The watch id for the directory
	/// @param dir The directory
	/// @param filename The filename that was accessed (not full path)
	/// @param action Action that was performed
	/// @param oldFilename The name of the file or directory moved
	virtual void handleFileAction(efsw::WatchID id, const std::string& dir, const std::string& filename, efsw::Action action, std::string old_path) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
