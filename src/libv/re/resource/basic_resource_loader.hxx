// Project: libv.rev, File: src/libv/rev/resource/basic_resource.hpp

#pragma once

#include <tsl/robin_set.h>

#include <libv/algo/slice.hpp>
#include <libv/container/small_vector.hpp>
#include <libv/fsw/token.hpp>
#include <libv/fsw/watcher.hpp>
#include <libv/mt/ticket_machine.hpp>
#include <libv/re/log.hpp>
#include <libv/re/r.hpp>
#include <libv/re/resource/common_resource.hpp>
#include <libv/re/settings.hpp>
#include <libv/res/common_resource_event.hpp>
#include <libv/res/resource_path.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
#include <libv/utility/read_file.hpp>

#include <expected>
#include <mutex>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct WatchedFile {
	libv::fsw::Token token;

	explicit WatchedFile(libv::fsw::Token token) : token(std::move(token)) { }
	WatchedFile(const WatchedFile&) = delete;
	WatchedFile& operator=(const WatchedFile&) & = delete;
	WatchedFile(WatchedFile&&) noexcept = default;
	WatchedFile& operator=(WatchedFile&&) & noexcept = default;

	~WatchedFile() {
		r.fsw.unsubscribe(token);
	}
};

// -------------------------------------------------------------------------------------------------

enum class ThreadAffinity {
	GL,
	FS,
	CPU,
	Continue,
};

class ResourceLoadingFunction {
	libv::mt::ticket token;
	cppcoro::generator<ThreadAffinity> coroutine;
	cppcoro::generator<ThreadAffinity>::iterator it;

public:
	ResourceLoadingFunction(cppcoro::generator<ThreadAffinity>&& coroutine, libv::mt::ticket&& token) :
		token(std::move(token)),
		coroutine(std::move(coroutine)) {
		init(); // Prime the co-routine to check what is the first thread it would like to have
	}

	void schedule(ThreadAffinity ta) {
		switch (ta) {
		case ThreadAffinity::GL:
			r.queueTaskGL(std::move(*this));
			break;
		case ThreadAffinity::CPU:
			r.queueTaskCPU(std::move(*this));
			break;
		case ThreadAffinity::FS:
			r.queueTaskFS(std::move(*this));
			break;
		case ThreadAffinity::Continue:
			; // Noop
			break;
		}
	}

	void init() {
		it = coroutine.begin(); // Primes the co-routine
		if (it == coroutine.end())
			return;

		const ThreadAffinity ta = *it;
		schedule(ta);
	}

	void operator()() {
		while (true) {
			++it;
			if (it == coroutine.end())
				return;

			const ThreadAffinity ta = *it;
			if (ta == ThreadAffinity::Continue)
				continue;

			schedule(ta);
			break;
		}
	}
};

// -------------------------------------------------------------------------------------------------

struct ResourceAccess {
	template <typename T>
	static inline void requestLoad(T& resource, LoadingMode mode) {
		resource.requestLoad(mode);
	}
	template <typename T>
	static inline void requestActivatingLoad(T& resource, LoadingMode mode) {
		resource.requestActivatingLoad(mode);
	}
	template <typename T>
	static inline void recordLoadFailure(T& resource) {
		resource.recordLoadFailure();
	}
	template <typename T>
	static inline void recordLoadSuccess(T& resource) {
		resource.recordLoadSuccess();
	}
	template <typename T>
	static inline void recordUnloadFromCache(T& resource) {
		resource.recordUnloadFromCache();
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename Key>
struct ResourceEquality {
	using is_transparent = void;
	using ptr = libv::intrusive2_ptr<T>;
public:
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE std::size_t operator()(const ptr& lhs, const ptr& rhs) noexcept {
		return *lhs == *rhs;
	}
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE std::size_t operator()(const ptr& lhs, const T& rhs) noexcept {
		return *lhs == rhs;
	}
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE std::size_t operator()(const T& lhs, const ptr& rhs) noexcept {
		return lhs == *rhs;
	}
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE std::size_t operator()(const ptr& lhs, const Key& rhs) noexcept {
		return *lhs == rhs;
	}
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE std::size_t operator()(const Key& lhs, const ptr& rhs) noexcept {
		return lhs == *rhs;
	}
};

template <typename T, typename Key>
struct ResourceHasher {
	using is_transparent = void;
public:
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE std::size_t operator()(const libv::intrusive2_ptr<T>& resource) noexcept {
		return resource->hash();
	}
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE std::size_t operator()(const T& resource) noexcept {
		return resource.hash();
	}
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE std::size_t operator()(const Key& key) noexcept {
		return key.hash();
	}
};

template <typename CRTP, typename Traits>
class BasicResourceLoader {
	using Resource = typename Traits::Resource;
	using ResourceID = typename Traits::ResourceID;
	using ResourceKey = typename Traits::ResourceKey;
	using Resource_ptr = libv::intrusive2_ptr<Resource>;
	using DelayedLoadGuard = typename Traits::DelayedLoadGuard;

public:
	std::mutex mutex;

	tsl::robin_set<Resource_ptr,
			ResourceHasher<Resource, ResourceKey>,
			ResourceEquality<Resource, ResourceKey>,
			std::allocator<Resource_ptr>,
			true // StoreHash=true as equality is expensive to calculate on most resource
			> cache;

	bool trackFiles = false;
	std::chrono::steady_clock::duration fswReloadDelay{0};
	tsl::robin_set<ResourceID> fswReloadInflight;
	std::unordered_map<ResourceID, std::vector<WatchedFile>> watchedFiles;

public:
	explicit BasicResourceLoader(const libv::res::ResourceLoaderSettings& settings) :
		trackFiles(settings.trackFiles),
		fswReloadDelay(settings.fswReloadDelay) { }

private:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE CRTP& self() noexcept {
		return static_cast<CRTP&>(*this);
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const CRTP& self() const noexcept {
		return static_cast<const CRTP&>(*this);
	}

public:
	std::expected<std::string, libv::res::FileLoadFailure> _fs_loadFile(
			std::vector<WatchedFile>* const watchTokens,
			const std::vector<libv::res::ResourcePathMapping>& resourceMappings,
			std::string_view fileIdentifier,
			ResourceID id);

	template <typename... Args>
	[[nodiscard]] Resource_ptr _cacheGet(std::unique_lock<std::mutex>& lock, bool requestLoad, bool& cacheHit, LoadingMode mode, const Resource_ptr& fallback, Args&&... args);
	[[nodiscard]] Resource_ptr _cacheExtract(const Resource& resource);

	template <typename... Args>
	[[nodiscard]] Resource_ptr load(LoadingMode mode, const Resource_ptr& fallback, Args&&... args);
	template <typename... Args>
	[[nodiscard]] DelayedLoadGuard load_delayed(LoadingMode mode, const Resource_ptr& fallback, Args&&... args);

	/// Called from Resource's requestLoad or recordLoadFailure/recordLoadSuccess
	void executeLoad(Resource_ptr&& resource, LoadingMode mode);
	/// Called from fsw events
	void requestReload(ResourceID id);
	/// Called from Resource_ptr dtor when only one reference is left (which is in the cache)
	void requestUnload(Resource* resource);
};

// -------------------------------------------------------------------------------------------------

template <typename CRTP, typename Traits>
std::expected<std::string, libv::res::FileLoadFailure> BasicResourceLoader<CRTP, Traits>::_fs_loadFile(
		std::vector<WatchedFile>* const watchTokens,
		const std::vector<libv::res::ResourcePathMapping>& resourceMappings,
		std::string_view fileIdentifier,
		ResourceID id) {

	libv::small_vector<libv::res::ResourceMappingError, 8> mappingErrors;
	libv::small_vector<libv::res::LoadFilePhysicalError, 8> physicalErrors;

	for (const auto& [resourcePrefix, virtualPrefix] : resourceMappings) {
		if (!fileIdentifier.starts_with(resourcePrefix))
			continue;

		// Apply mapping rules
		const auto virtualName = libv::slice_off_view(fileIdentifier, static_cast<int64_t>(resourcePrefix.size()));
		auto virtualFilepath = fmt::format("{}{}", virtualPrefix, virtualName);
		auto physicalFilepathResult = libv::res::resource_path_ec(virtualFilepath);
		if (!physicalFilepathResult) {
			mappingErrors.emplace_back(virtualFilepath, physicalFilepathResult.error());
			continue;
		}
		auto physicalFilepath = physicalFilepathResult.value();

		// FSW
		if (watchTokens != nullptr)
			// Subscribe to every file that was mapped even if they don't exist
			watchTokens->emplace_back(r.fsw.subscribe_file(physicalFilepath, [id, this](const libv::fsw::Event&) {
						requestReload(id);
					}));

		// Read file data
		auto resource_data = libv::read_file_ec(physicalFilepath);
		if (!resource_data.ec) {
			return std::move(resource_data.data);
		} else {
			physicalErrors.emplace_back(std::string(physicalFilepath), resource_data.ec);

			if (resource_data.ec == std::errc::no_such_file_or_directory)
				continue; // no_such_file_or_directory is not a hard error, move onto the next rule
			else
				break; // treat every other error as hard error
		}
	}

	// Compose and log detailed error message about the failures
	std::vector<libv::res::ResourceMappingUnmatched> unmatchedMappings;

	std::ostringstream errorMsg;
	for (const auto& [resourcePrefix, virtualPrefix] : resourceMappings)
		if (!fileIdentifier.starts_with(resourcePrefix)) {
			unmatchedMappings.emplace_back(resourcePrefix, virtualPrefix);
			errorMsg << "\t\tMapping '" << resourcePrefix << "' -> '" << virtualPrefix << "' - Unmatched prefix\n";
		}
	for (const auto& [virtualFilepath, ec] : mappingErrors)
		errorMsg << "\t\tVirtual '" << virtualFilepath << "' - " << libv::res::to_message(ec) << "\n";
	for (const auto& [physicalFilepath, ec] : physicalErrors)
		errorMsg << "\t\tPhysical '" << physicalFilepath << "' - " << ec.message() << " (" << ec << ")\n";

	log_re.error("Failed to load {} resource file: '{}'\n{}", Traits::name_resource, fileIdentifier, std::move(errorMsg).str());

	return std::unexpected(libv::res::FileLoadFailure{
			std::move(unmatchedMappings),
			std::vector<libv::res::ResourceMappingError>{mappingErrors.begin(), mappingErrors.end()},
			std::vector<libv::res::LoadFilePhysicalError>{physicalErrors.begin(), physicalErrors.end()}});
}

template <typename CRTP, typename Traits>
template <typename... Args>
[[nodiscard]] auto BasicResourceLoader<CRTP, Traits>::_cacheGet(std::unique_lock<std::mutex>& lock, bool requestLoad, bool& cacheHit, LoadingMode mode, const Resource_ptr& fallback, Args&&... args) -> Resource_ptr {
	const auto it = cache.find(ResourceKey{args...});
	cacheHit = it != cache.end();
	if (cacheHit)
		return *it;

	auto resource = Resource_ptr::make(std::forward<Args>(args)..., CachedTag{}, !requestLoad, fallback);
	cache.insert(resource);
	lock.unlock();
	if (requestLoad)
		ResourceAccess::requestLoad(*resource, mode); // Request async load
	return resource;
}

template <typename CRTP, typename Traits>
[[nodiscard]] auto BasicResourceLoader<CRTP, Traits>::_cacheExtract(const Resource& resource) -> Resource_ptr {
	auto it = cache.find(resource);
	assert(it != cache.end() && "Internal error: Resource is missing from the cache");

	// Not sure if stealing the resource with const_cast + move + erase is supported in the container
	// It is perfectly fine and well-defined if the followings hold true:
	//	- The 'declaration' or memory initialization of the element type is done with non-const T
	//	- Erase with an iterator does not observe the element erased or only does so for a single destructor call
	auto extracted = std::move(const_cast<Resource_ptr&>(*it));
	cache.erase(it);
	return extracted;
}

template <typename CRTP, typename Traits>
template <typename... Args>
[[nodiscard]] auto BasicResourceLoader<CRTP, Traits>::load(LoadingMode mode, const Resource_ptr& fallback, Args&&... args) -> Resource_ptr {
	auto lock = std::unique_lock(mutex);

	auto ptr = self()._loadBuiltin(args...);
	if (ptr)
		return ptr;

	bool cacheHit = false;
	ptr = _cacheGet(lock, true, cacheHit, mode, fallback, std::forward<Args>(args)...);
	return ptr;
}

template <typename CRTP, typename Traits>
template <typename... Args>
[[nodiscard]] auto BasicResourceLoader<CRTP, Traits>::load_delayed(LoadingMode mode, const Resource_ptr& fallback, Args&&... args) -> DelayedLoadGuard {
	auto lock = std::unique_lock(mutex);
	auto ptr = self()._loadBuiltin(args...);
	if (ptr)
		return DelayedLoadGuard{std::move(ptr), mode, false};

	bool cacheHit = false;
	ptr = _cacheGet(lock, false, cacheHit, mode, fallback, std::forward<Args>(args)...);

	return DelayedLoadGuard{std::move(ptr), mode, !cacheHit};
}

// Called from Resource_ptr dtor when only one reference is left (which is in the cache)
template <typename CRTP, typename Traits>
void BasicResourceLoader<CRTP, Traits>::executeLoad(Resource_ptr&& resource, LoadingMode mode) {
	(void) ResourceLoadingFunction(
			self().loadingRoutine(std::move(resource), mode),
			mode == LoadingMode::blocking ? r.blockNextFrame() : libv::mt::ticket{nullptr}
	);
}

// Called from fsw events
template <typename CRTP, typename Traits>
void BasicResourceLoader<CRTP, Traits>::requestReload(ResourceID id) {
	auto lock = std::unique_lock(mutex);

	const Resource_ptr* resource_opt = [&]() -> const Resource_ptr* {
		// 'Full table scan' is acceptable as reload performance is secondary
		for (const auto& resource : cache)
			if (resource->id() == id)
				return &resource;
		// It is possible that the resource was removed in the meantime, it is not an error
		return nullptr;
	}();
	if (!resource_opt)
		return;
	const Resource_ptr& resource = *resource_opt;

	if (fswReloadDelay.count() == 0) {
		lock.unlock();
		ResourceAccess::requestLoad(*resource, LoadingMode::async); // Request async reload
	} else {
		if (fswReloadInflight.contains(id))
			return; // Deduplicate reload
		fswReloadInflight.insert(id);

		r.queueTaskCPU([this, id, resource] {
			{
				auto lock2 = std::unique_lock(mutex);
				fswReloadInflight.erase(id);
			}
			ResourceAccess::requestLoad(*resource, LoadingMode::async); // Request async reload
		}, fswReloadDelay);
	}
}

// Called from Resource_ptr dtor when only one reference is left (which is in the cache)
template <typename CRTP, typename Traits>
void BasicResourceLoader<CRTP, Traits>::requestUnload(Resource* resource) {
	Resource_ptr lastInstance = nullptr;
	{
		auto lock = std::unique_lock(mutex);
		if (resource->ref_count() > 1)
			// Resource got reacquired while this call was in flight, not an issue, just return
			return;

		watchedFiles.erase(resource->id());
		lastInstance = _cacheExtract(*resource);
		ResourceAccess::recordUnloadFromCache(*lastInstance);
	}
	self().broadcastResourceUnload(std::move(lastInstance));
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
