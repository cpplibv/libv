// Project: libv.fsw, File: src/libv/fsw/watcher.hpp, Author: Császár Mátyás [Vader]

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

class FileWatcher {
public:
	using callback_type = std::function<void(const Event&)>;
	using token_type = WatchToken;

private:
	std::unique_ptr<class ImplFileWatcher> self;

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
