// Project: libv.process, File: src/libv/process/lock_file.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <memory>
#include <mutex>


namespace libv {
namespace process {

// -------------------------------------------------------------------------------------------------

/// Note: For each file, use a single file_lock object per process.
/// Note: Use the same thread to lock and unlock a file.
class lock_file {
	std::unique_ptr<class impl_lock_file> self;

public:
	///	Opens a file lock.
	/// If the file does not exist creates it.
	/// Does not acquires the lock.
	/// Throws: interprocess_exception if the file creation or the file access fails.
	explicit lock_file(const std::filesystem::path& filepath);

	lock_file() = delete;
	lock_file(const lock_file&) = delete;
	lock_file& operator=(const lock_file&) & = delete;

	lock_file(lock_file&&) noexcept;
	lock_file& operator=(lock_file&&) & noexcept;

	///	Closes (and attempts to delete) the lock file. Does not throw.
	/// Does not releases the lock.
	~lock_file() noexcept;

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

class lock_file_guard {
	lock_file mutex;
	bool owns;

public:
	lock_file_guard() = delete;
	lock_file_guard(const lock_file_guard&) = delete;
	lock_file_guard& operator=(const lock_file_guard&) & = delete;

	inline explicit lock_file_guard(const std::filesystem::path& filepath) :
		mutex(filepath),
		owns(true) {
		lock();
	}

	inline lock_file_guard(const std::filesystem::path& filepath, std::defer_lock_t) :
		mutex(filepath),
		owns(false) {
	}

	inline lock_file_guard(lock_file_guard&& orig) noexcept :
		mutex(std::move(orig.mutex)),
		owns(orig.owns)	{
		orig.owns = false;
	}

	inline lock_file_guard& operator=(lock_file_guard&& orig) & noexcept {
		if (owns_lock())
			unlock();

		mutex = std::move(orig.mutex);
		owns = orig.owns;
		orig.owns = false;

		return *this;
	}

	inline ~lock_file_guard() noexcept {
		if (owns_lock())
			unlock();
	}

public:
	inline void lock() {
		mutex.lock();
	}

	[[nodiscard]] inline bool try_lock() {
		return mutex.try_lock();
	}

	inline void unlock() {
		mutex.unlock();
	}

public:
	[[nodiscard]] inline bool owns_lock() const noexcept {
		return owns;
	}

	[[nodiscard]] explicit inline operator bool() const noexcept {
		return owns_lock();
	}

	[[nodiscard]] inline bool operator!() const noexcept {
		return !owns_lock();
	}
};

// -------------------------------------------------------------------------------------------------

class lock_file_shared_guard {
	lock_file mutex;
	bool owns;

public:
	lock_file_shared_guard() = delete;
	lock_file_shared_guard(const lock_file_shared_guard&) = delete;
	lock_file_shared_guard& operator=(const lock_file_shared_guard&) & = delete;

	inline explicit lock_file_shared_guard(const std::filesystem::path& filepath) :
		mutex(filepath),
		owns(true) {
		lock();
	}

	inline lock_file_shared_guard(const std::filesystem::path& filepath, std::defer_lock_t) :
		mutex(filepath),
		owns(false) {
	}

	inline lock_file_shared_guard(lock_file_shared_guard&& orig) noexcept :
		mutex(std::move(orig.mutex)),
		owns(orig.owns)	{
		orig.owns = false;
	}

	inline lock_file_shared_guard& operator=(lock_file_shared_guard&& orig) & noexcept {
		if (owns_lock())
			unlock();

		mutex = std::move(orig.mutex);
		owns = orig.owns;
		orig.owns = false;

		return *this;
	}

	inline ~lock_file_shared_guard() noexcept {
		if (owns_lock())
			unlock();
	}

public:
	inline void lock() {
		mutex.lock_shared();
		owns = true;
	}

	[[nodiscard]] inline bool try_lock() {
		owns = mutex.try_lock_shared();
		return owns;
	}

	inline void unlock() {
		mutex.unlock_shared();
		owns = false;
	}

public:
	[[nodiscard]] inline bool owns_lock() const noexcept {
		return owns;
	}

	[[nodiscard]] explicit inline operator bool() const noexcept {
		return owns_lock();
	}

	[[nodiscard]] inline bool operator!() const noexcept {
		return !owns_lock();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace process
} // namespace libv
