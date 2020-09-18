// Project: libv.thread, File: src/libv/thread/single_instance.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <memory>
#include <mutex>


// -------------------------------------------------------------------------------------------------

namespace libv {
namespace thread {
namespace detail {

template <typename Guard>
struct SingleInstanceProxy {
	Guard& instance;

	SingleInstanceProxy(Guard& instance) :
		instance(instance) {}

	~SingleInstanceProxy() {
		std::lock_guard lock(instance.mutex);
		if (--instance.ref_count == 0)
			instance.variable.reset();
	}
};

} // namespace detail ------------------------------------------------------------------------------

/// This utility guarantees that only a single instance is alive which was handed out by the same object.
/// Constructors and destructors are running in a mutually exclusive manner. This is not a singleton implementation,
/// multiple SingleInstance instance can hand out multiple instances of the same type.
/// @example `SingleInstance<std::string> var; std::shared_ptr<std::string> instance = var.get();`
template <typename T>
class SingleInstance {
	template <typename Guard>
	friend class detail::SingleInstanceProxy;

private:
	std::mutex mutex;
	std::unique_ptr<T> variable;
	size_t ref_count;

public:
	std::shared_ptr<T> get() {
		std::lock_guard lock(mutex);

		if (ref_count == 0)
			variable = std::make_unique<T>();

		++ref_count;

		return std::shared_ptr<T>(std::make_shared<detail::SingleInstanceProxy<SingleInstance<T>>>(*this), variable.get());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace thread
} // namespace libv
