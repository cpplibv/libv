// Project: libv, File: app/update/make_patch.cpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <memory>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

// TODO P5: This should file and mutex_lock_file should be in an inter process and not multi thread library. Create another lib

// -------------------------------------------------------------------------------------------------

/// Note: For each file, use a single file_lock object per process.
/// Note: Use the same thread to lock and unlock a file.
class mutex_lock_file {
	std::unique_ptr<class impl_mutex_lock_file> self;

public:
	///	Opens a file lock. If the file does not exist creates it.
	/// Throws: interprocess_exception if the file creation or the file access fails.
	explicit mutex_lock_file(const std::filesystem::path& filepath);

	mutex_lock_file() = delete;
	mutex_lock_file(const mutex_lock_file&) = delete;
	mutex_lock_file& operator=(const mutex_lock_file&) & = delete;

	mutex_lock_file(mutex_lock_file&&) noexcept = default;
	mutex_lock_file& operator=(mutex_lock_file&&) & noexcept = default;

	///	Closes and attempts to deletes the lock file. Does not throw.
	~mutex_lock_file() noexcept;

public:
	///	Effects: The calling thread tries to obtain exclusive ownership of the mutex,
	///		and if another thread has exclusive, or sharable ownership of
	///		the mutex, it waits until it can obtain the ownership.
	///	Throws: interprocess_exception on error.
	void lock();

	///	Effects: The calling thread tries to obtain sharable ownership of the mutex,
	///		and if another thread has exclusive ownership of the mutex, waits until
	///		it can obtain the ownership.
	///	Throws: interprocess_exception on error.
	void lock_shared();

	///	Effects: The calling thread tries to acquire exclusive ownership of the mutex
	///		without waiting. If no other thread has exclusive, or sharable
	///		ownership of the mutex this succeeds.
	///	Returns: If it can acquire exclusive ownership immediately returns true.
	///		If it has to wait, returns false.
	///	Throws: interprocess_exception on error.
	bool try_lock();

	///	Effects: The calling thread tries to acquire sharable ownership of the mutex
	///		without waiting. If no other thread has exclusive ownership of the
	///		mutex this succeeds.
	///	Returns: If it can acquire sharable ownership immediately returns true. If it
	///		has to wait, returns false.
	///	Throws: interprocess_exception on error.
	bool try_lock_shared();

	///	Precondition: The thread must have exclusive ownership of the mutex.
	///	Effects: The calling thread releases the exclusive ownership of the mutex.
	///	Throws: An exception derived from interprocess_exception on error.
	void unlock();

	///	Precondition: The thread must have sharable ownership of the mutex.
	///	Effects: The calling thread releases the sharable ownership of the mutex.
	///	Throws: An exception derived from interprocess_exception on error.
	void unlock_shared();
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
