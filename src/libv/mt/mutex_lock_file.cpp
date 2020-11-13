// Project: libv, File: app/update/make_patch.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/mt/mutex_lock_file.hpp>
// ext
#include <boost/interprocess/sync/file_lock.hpp>
// std
#include <filesystem>
#include <fstream>
#include <memory>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

struct impl_mutex_lock_file {
	std::ofstream file;
	boost::interprocess::file_lock lock;
	std::filesystem::path filepath;

	explicit impl_mutex_lock_file(std::filesystem::path filepath) :
		file(filepath, std::ios::binary | std::ios::out),
		lock(filepath.string().c_str()),
		filepath(std::move(filepath)) {
	}
};

mutex_lock_file::mutex_lock_file(const std::filesystem::path& filepath) :
		self(std::make_unique<impl_mutex_lock_file>(filepath)) {}

mutex_lock_file::~mutex_lock_file() {
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	// NOTE: On windows deleting a file will fail if it is already opened by another process
	//          so we can safely attempt to remove it every time.

	self->file.close();

	std::error_code ignore_ec; // Failure can be ignored, lockfile most likely open by another process
	std::filesystem::remove(self->filepath, ignore_ec);

	#else
	// NOTE: On unix deleting a file will would succeed even if it is opened by another process
	//          so we have to leave the lockfile there. Not a clean solution, but an acceptable compromise for now.

	#endif
}

void mutex_lock_file::lock() {
	self->lock.lock();
}

void mutex_lock_file::lock_shared() {
	self->lock.lock_sharable();
}

bool mutex_lock_file::try_lock() {
	return self->lock.try_lock();
}

bool mutex_lock_file::try_lock_shared() {
	return self->lock.try_lock_sharable();
}

void mutex_lock_file::unlock() {
	self->lock.unlock();
}

void mutex_lock_file::unlock_shared() {
	self->lock.unlock_sharable();
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
