// Project: libv.mt, File: src/libv/mt/single_instance.hpp

#pragma once

// std
#include <memory>
#include <mutex>
#include <optional>


// -------------------------------------------------------------------------------------------------

namespace libv {
namespace mt {
namespace detail {

template <typename Guard>
struct SingleInstanceProxy {
	Guard& instance;

	constexpr inline explicit SingleInstanceProxy(Guard& instance) noexcept :
		instance(instance) {}

	constexpr inline ~SingleInstanceProxy() {
		std::lock_guard lock(instance.mutex);
		if (--instance.ref_count == 0)
			instance.variable.reset();
	}
};

} // namespace detail ------------------------------------------------------------------------------

/// This utility guarantees that only a single instance is alive which was handed out by the same object.
/// Constructors and Destructors are running in a mutually exclusive manner. This is not a singleton implementation,
/// multiple SingleInstance instance can hand out multiple instances of the same type.
/// @example `SingleInstance<std::string> var; std::shared_ptr<std::string> instance = var.get();`
template <typename T>
class SingleInstance {
	template <typename Guard>
	friend class detail::SingleInstanceProxy;

private:
	std::mutex mutex;
	std::optional<T> variable; // Instead of optional: libv::storage<T> variable; aka alignas(T) std::byte storage[sizeof(T)]
	std::size_t ref_count = 0;
	// NOTE: Not implemented with weak_ptr because the destructors are also guarded with the same mutex

public:
	[[nodiscard]] std::shared_ptr<T> get() {
		std::lock_guard lock(mutex);

		if (ref_count == 0)
			variable.emplace();

		++ref_count;

		return std::shared_ptr<T>(std::make_shared<detail::SingleInstanceProxy<SingleInstance<T>>>(*this), &variable.value());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
