// Project: libv.fsw, File: src/libv/fsw/watcher.hpp

#pragma once

#include <libv/fsw/fwd.hpp>
#include <libv/fsw/action.hpp>
#include <libv/fsw/event.hpp>
#include <libv/fsw/token.hpp>

#include <filesystem>
#include <functional>
#include <memory>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

class ImplWatcher;

class Watcher {
public:
	using callback_type = std::function<void(const Event&)>;
	using token_type = Token;
	using safe_token_type = SafeToken;

private:
	std::unique_ptr<ImplWatcher> self;

public:
	Watcher();
	~Watcher();

public:
	// TODO P5: libv.fsw: If the watched directory removed (then recreated) the watching stops
	token_type subscribe_directory(std::filesystem::path path, callback_type callback);
	safe_token_type subscribe_directory_safe(std::filesystem::path path, callback_type callback);
	// token_type subscribe_directory_run(std::filesystem::path path, callback_type callback)
	// token_type subscribe_directory_run_safe(std::filesystem::path path, callback_type callback)
	token_type subscribe_file(std::filesystem::path path, callback_type callback);
	safe_token_type subscribe_file_safe(std::filesystem::path path, callback_type callback);
	// token_type subscribe_file_run(std::filesystem::path path, callback_type callback);
	// token_type subscribe_file_run_safe(std::filesystem::path path, callback_type callback);

	// TODO P5: libv.fsw: Add a type (file or directory) independent way of subscribe to a (potentially not existing)
	// 		file system entity
	// void subscribe(std::filesystem::path path, callback_type callback);

	/// No callback will ever be called after their token was unsubscribed
	void unsubscribe(token_type token);

	// void unsubscribe(std::filesystem::path path);
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
