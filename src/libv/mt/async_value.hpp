// Project: libv.mt, File: src/libv/mt/async_value.hpp

#pragma once

// std
#include <cassert>
#include <memory>
#include <mutex>
#include <optional>
#include <stop_token>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

/// async_value is basically a future with cancellation support
/// The object and lifetime semantics are matching with a shared_ptr
/// Creation of a new object is done with load_async
/// @thread shared state is thread safe, but the async_value object itself is unsafe
template <typename T>
class async_value {
private:
	struct payload_t {
		mutable std::mutex mutex;
		size_t async_value_ref_count = 1; // ref count used to cancel async task (shared/weak ptr has no viable event for it)
		std::optional<T> value;
		std::stop_source stop_source;

		void increment_ref_count() {
			assert(async_value_ref_count != 0);
			++async_value_ref_count;
		}

		void decrement_ref_count() {
			std::scoped_lock lock(mutex);
			if (--async_value_ref_count == 0)
				stop_source.request_stop();
		}
	};

	template <bool N, typename Value, typename Executor, typename LoadFunc, typename... Args>
	friend async_value<Value> aux_load_async(Executor& executor, LoadFunc&& func, Args&&... args);

private:
	std::shared_ptr<payload_t> result;

public:
	constexpr inline async_value() noexcept = default;
	inline async_value(const async_value& orig) noexcept :
		result(orig.result) {
		if (result)
			result->increment_ref_count();
	};
	inline async_value(async_value&& orig) noexcept = default;
	inline async_value& operator=(const async_value& orig) & noexcept {
		if (orig.result)
			orig.result->increment_ref_count();
		if (result)
			result->decrement_ref_count();
		result = orig.result;
		return *this;
	};
	inline async_value& operator=(async_value&& orig) & noexcept = default;

	inline ~async_value() {
		if (result)
			result->decrement_ref_count();
	}

public:
	[[nodiscard]] bool loading() const noexcept {
		assert(result);
		std::scoped_lock lock(result->mutex);
		return !result->value.has_value() && !result->stop_source.stop_requested();
	}

	[[nodiscard]] bool has_value() const noexcept {
		assert(result);
		std::scoped_lock lock(result->mutex);
		return result->value.has_value();
	}

	[[nodiscard]] const T& value() const noexcept
			requires (!std::is_void_v<T>) {
		assert(result && result->value.has_value());
		std::scoped_lock lock(result->mutex);
		return result->value.value();
	}

	void cancel() noexcept {
		std::scoped_lock lock(result->mutex);
		assert(result);
		result->stop_source.request_stop();
	}
};

// -------------------------------------------------------------------------------------------------

struct deduce_type_tag {};

template <bool use_stop_token, typename Value, typename Executor, typename LoadFunc, typename... Args>
async_value<Value> aux_load_async(Executor& executor, LoadFunc&& func, Args&&... args) {
	using payload_t = async_value<Value>::payload_t;

	async_value<Value> av;
	av.result = std::make_shared<payload_t>();

	executor.execute_async([
			result_sp = av.result,
			f = std::forward<LoadFunc>(func),
			...a = std::forward<Args>(args)]  mutable {
		// By value capture for the function object and for the arguments

		if (result_sp->stop_source.stop_requested())
			return;

		if constexpr (use_stop_token)
			// Moving 'f' to give the loader function call operator the freedom to be rvalue qualified
			// Moving 'a...' as the local by value copies are going out of scope anyways
			// NOTE: This blocks lvalue references as parameters, but those uses are error prone and logically incorrect
			result_sp->value = std::move(f)(result_sp->stop_source.get_token(), std::move(a)...);
		else
			result_sp->value = std::move(f)(std::move(a)...);
	});

	return av;
}

/// Creates a new async_value object associated with the loading task result
/// Supports std::stop_token as the first argument for the loader function
/// If the cancellation occurs before the loader function call, the call is skipped entirely
template <typename T = deduce_type_tag, typename Executor, typename LoadFunc, typename... Args>
inline auto load_async(Executor& executor, LoadFunc&& func, Args&&... args) {
	// NOTE: For deduction move is used due to the final invocation will use move
	static constexpr bool use_stop_token = std::is_invocable_v<decltype(std::move(func)), std::stop_token, decltype(std::move(args))...>;
	static constexpr bool deduce_type = std::is_same_v<T, deduce_type_tag>;

	if constexpr (use_stop_token) {
		using value_type = std::conditional_t<deduce_type, std::invoke_result_t<decltype(std::move(func)), std::stop_token, decltype(std::move(args))...>, T>;
		return aux_load_async<use_stop_token, value_type>(executor, std::forward<LoadFunc>(func), std::forward<Args>(args)...);
	} else {
		using value_type = std::conditional_t<deduce_type, std::invoke_result_t<decltype(std::move(func)), decltype(std::move(args))...>, T>;
		return aux_load_async<use_stop_token, value_type>(executor, std::forward<LoadFunc>(func), std::forward<Args>(args)...);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
