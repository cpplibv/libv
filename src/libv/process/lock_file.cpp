// Project: libv.process, File: src/libv/process/lock_file.cpp

// hpp
#include <libv/process/lock_file.hpp>
// ext
#include <boost/interprocess/sync/file_lock.hpp>
// std
#include <filesystem>
#include <fstream>
#include <memory>


namespace libv {
namespace process {

// -------------------------------------------------------------------------------------------------

struct impl_lock_file {
	std::ofstream file;
	boost::interprocess::file_lock mutex;
	std::filesystem::path filepath;

	explicit impl_lock_file(std::filesystem::path filepath) :
		file(filepath, std::ios::binary | std::ios::out),
		mutex(filepath.string().c_str()),
		filepath(std::move(filepath)) {
	}
};

lock_file::lock_file(const std::filesystem::path& filepath) :
	self(std::make_unique<impl_lock_file>(filepath)) {}

lock_file::lock_file(lock_file&&) noexcept = default;

lock_file& lock_file::operator=(lock_file&&) & noexcept = default;

lock_file::~lock_file() {
#ifdef _WIN32
	// NOTE: On windows deleting a file will fail if it is already opened by another process
	//          so we can safely attempt to remove it every time.

	self->file.close();

	std::error_code ignore_ec; // Failure can be ignored, lockfile most likely open by another process
	std::filesystem::remove(self->filepath, ignore_ec);

#else
	// NOTE: On unix deleting a file would succeed even if it is opened by another process
	//          so we have to leave the lockfile there. Not a clean solution, but an acceptable compromise for now.

#endif
}

void lock_file::lock() {
	self->mutex.lock();
}

void lock_file::lock_shared() {
	self->mutex.lock_sharable();
}

bool lock_file::try_lock() {
	return self->mutex.try_lock();
}

bool lock_file::try_lock_shared() {
	return self->mutex.try_lock_sharable();
}

void lock_file::unlock() {
	self->mutex.unlock();
}

void lock_file::unlock_shared() {
	self->mutex.unlock_sharable();
}

// -------------------------------------------------------------------------------------------------

} // namespace process
} // namespace libv
