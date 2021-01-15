// Project: libv.mt, File: src/libv/mt/async_value.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cassert>
#include <memory>
#include <mutex>
#include <stop_token>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

template <typename T>
class async_value {
private:
	struct payload_t {
		std::optional<T> value;
		std::stop_source stop_source;
	};

private:
	mutable std::mutex mutex;
	std::shared_ptr<payload_t> result = std::make_shared<payload_t>();

public:
	inline async_value() noexcept = default;
	inline async_value(const async_value& orig) noexcept {
		std::scoped_lock lock_orig(orig.mutex);
		result = orig.result;
	};
	inline async_value(async_value&& orig) noexcept {
		std::scoped_lock lock_orig(orig.mutex);
		result = std::move(orig.result);
	};
	inline async_value& operator=(const async_value& orig) & noexcept {
		std::scoped_lock lock_orig(orig.mutex);
		std::scoped_lock lock(mutex);
		result = orig.result;
		return *this;
	};
	inline async_value& operator=(async_value&& orig) & noexcept {
		std::scoped_lock lock_orig(orig.mutex);
		std::scoped_lock lock(mutex);
		result = std::move(orig.result);
		return *this;
	};

public:
	/// Supports std::stop_token as the first argument for the loader function
	/// If the cancellation occurs before the loader function call, the call is skipped entirely
	template <typename Executor, typename LoadFunc, typename... Args>
	void load_async(Executor& executor, LoadFunc&& func, Args&&... args) {
		std::shared_ptr<payload_t> local_result;

		{
			std::scoped_lock lock(mutex);
			if (result)
				result->stop_source.request_stop();
			result = std::make_shared<payload_t>();
			local_result = result;
		}

		executor.execute_async([
				result_sp = std::move(local_result),
				f = std::forward<LoadFunc>(func),
				...a = std::forward<Args>(args)] () mutable {
			// By value capture for the function object and for the arguments

			if (result_sp->stop_source.stop_requested())
				return;

			if constexpr (std::is_invocable_v<decltype(std::move(f)), std::stop_token, decltype(std::move(a))...>)
				// Moving 'f' to give the loader function call operator the freedom to be rvalue qualified
				// Moving 'a...' as the local by value copies are going out of scope anyways
				// NOTE: This blocks lvalue references as parameters, but those uses are error prone and logically incorrect
				result_sp->value = std::move(f)(result_sp->stop_source.get_token(), std::move(a)...);
			else
				result_sp->value = std::move(f)(std::move(a)...);
		});
	}

	[[nodiscard]] bool loading() const noexcept {
		std::scoped_lock lock(mutex);
		return result && !result->value.has_value();
	}

	[[nodiscard]] bool has_value() const noexcept {
		std::scoped_lock lock(mutex);
		return result && result->value.has_value();
	}

	[[nodiscard]] const T& value() const noexcept {
		std::scoped_lock lock(mutex);
		assert(result && result->value.has_value());
		return result->value.value();
	}

	void cancel() noexcept {
		std::scoped_lock lock(mutex);
		if (result)
			result->stop_source.request_stop();
		result.reset();
	}

	~async_value() {
//		if (result && result.use_count() <= 2)
//  		// !!! Race condition here, observing use count is incorrect
//      !!! lifetime semantic violations
		result->stop_source.request_stop();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
