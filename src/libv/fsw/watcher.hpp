// Project: libv.fsw, File: src/libv/fsw/watcher.hpp

#pragma once

// std
#include <filesystem>
#include <functional>
#include <memory>
// pro
#include <libv/fsw/action.hpp>
#include <libv/fsw/event.hpp>
#include <libv/fsw/token.hpp>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

class Watcher {
public:
	using callback_type = std::function<void(const Event&)>;
	using token_type = WatchToken;

private:
	std::unique_ptr<class ImplWatcher> self;

public:
	Watcher();
	~Watcher();

public:
	// TODO P5: libv.fsw: If the watched directory removed (then recreated) the watching stops
	token_type subscribe_directory(std::filesystem::path path, callback_type callback);
//	token_type subscribe_directory_run(std::filesystem::path path, callback_type callback)
	token_type subscribe_file(std::filesystem::path path, callback_type callback);
//	token_type subscribe_file_run(std::filesystem::path path, callback_type callback);

	// TODO P5: libv.fsw: Add a type (file or directory) independent way of subscribe to a (potentially not existing) file system entity
//	void subscribe(std::filesystem::path path, callback_type callback);

//	void unsubscribe(std::filesystem::path path);
	void unsubscribe(token_type token);
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
