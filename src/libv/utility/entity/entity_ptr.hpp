// Project: libv.utility, File: src/libv/utility/entity/entity_ptr.hpp

#pragma once

// fwd
#include <libv/utility/entity/entity_ptr_fwd.hpp>
// std
#include <bit>
#include <cassert>
#include <mutex>
#include <new>
#include <utility>
// libv
#include <libv/utility/int_ceil_div.hpp>
#include <libv/utility/min_max.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

//template <typename T>
//concept cx_entity = requires(T& var) {
//	{ var.ref_count } -> std::integral;
//};

// -------------------------------------------------------------------------------------------------

class BaseContextBlockArenaMemoryChunk;

class BaseContextBlockArena {
public:
	struct BlockHeader {
		void* context_ptr;

		BlockHeader* next_header;

		std::byte* next_memory_chunk_ptr_ = nullptr; /// Owning pointer (Only the first block header in a memory chunk contains the next memory chunk)
	};

	struct FLEntry { /// Free List Entry
		FLEntry* next;
	};

private:
	using Mutex = std::mutex;

	[[no_unique_address]] mutable Mutex mutex;

	FLEntry* free_list_head = nullptr;

	std::byte* first_memory_chunk_ptr_ = nullptr; /// Owning pointer
	std::size_t block_size_and_alignment_ = 0;
	std::size_t object_size_ = 0;

	std::size_t size_ = 0;
	std::size_t capacity_ = 0;

protected:
	static constexpr std::size_t block_header_size = sizeof(BlockHeader);
	static constexpr std::size_t free_list_entry_size = sizeof(FLEntry);

protected:
	template <typename T>
	[[nodiscard]] static constexpr inline T* as_aligned(std::byte* ptr) noexcept {
		return reinterpret_cast<T*>(static_cast<void*>(ptr));
	}

private:
	[[nodiscard]] static BaseContextBlockArenaMemoryChunk allocateAndInitMemoryChunk(
			void* context_ptr, std::size_t block_size_and_alignment, std::size_t block_count, std::size_t object_size,
			std::byte* last_memory_chunk, BlockHeader* last_header, FLEntry* last_fl_entry);

	static inline FLEntry* createFLEntryAt(std::byte* at, FLEntry* next) {
		return ::new(at) FLEntry{next};
	}

	static inline std::byte* destroyFLEntryAt(FLEntry* at) {
		at->~FLEntry();
		return reinterpret_cast<std::byte*>(at);
	}

	static inline void createHeaderAt(std::byte* at, void* context_ptr, BlockHeader* next_header) {
		::new(at) BlockHeader{context_ptr, next_header};
	}

	void allocateMoreMemory();

public:
	[[nodiscard]] inline std::byte* alloc() { // Could be noexcept now, but in future growth would remove it
		auto lock = std::unique_lock(mutex);
		auto ptr = free_list_head;

		if (ptr == nullptr) {
			allocateMoreMemory();
			ptr = free_list_head;
		}

		free_list_head = free_list_head->next;
		++size_;

		return destroyFLEntryAt(ptr);
	}

	inline void dealloc(std::byte* ptr) noexcept {
		auto lock = std::unique_lock(mutex);
		assert(ptr != nullptr);
		--size_;
		free_list_head = createFLEntryAt(ptr, free_list_head);
	}

public:
	[[nodiscard]] inline std::size_t size() const noexcept {
		std::size_t result;
		auto lock = std::unique_lock(mutex);
		result = size_;
		return result;
	}

	[[nodiscard]] inline std::size_t capacity() const noexcept {
		std::size_t result;
		auto lock = std::unique_lock(mutex);
		result = capacity_;
		return result;
	}

	[[nodiscard]] inline void* context_ptr() const noexcept {
		void* result;
		auto lock = std::unique_lock(mutex);
		result = as_aligned<BlockHeader>(first_memory_chunk_ptr_)->context_ptr;
		return result;
	}

private:
	void destroy();

	inline BaseContextBlockArena() = default;

public:
	BaseContextBlockArena(
			void* context_ptr,
			std::size_t block_size_and_alignment,
			std::size_t block_count,
			std::size_t object_size);

	constexpr inline BaseContextBlockArena(const BaseContextBlockArena&) = delete;

	constexpr inline BaseContextBlockArena& operator=(const BaseContextBlockArena&) & = delete;

	inline BaseContextBlockArena(BaseContextBlockArena&& other) noexcept :
			free_list_head(other.free_list_head),
			first_memory_chunk_ptr_(other.first_memory_chunk_ptr_),
			block_size_and_alignment_(other.block_size_and_alignment_),
			object_size_(other.object_size_),
			size_(other.size_),
			capacity_(other.capacity_) {
		other.free_list_head = nullptr;
		other.first_memory_chunk_ptr_ = nullptr;
		other.block_size_and_alignment_ = 0;
		other.object_size_ = 0;
		other.size_ = 0;
		other.capacity_ = 0;
	}

	inline BaseContextBlockArena& operator=(BaseContextBlockArena&& other) & noexcept {
		if (&other == this)
			return *this;

		destroy();

		free_list_head = other.free_list_head;
		first_memory_chunk_ptr_ = other.first_memory_chunk_ptr_;
		block_size_and_alignment_ = other.block_size_and_alignment_;
		object_size_ = other.object_size_;
		size_ = other.size_;
		capacity_ = other.capacity_;

		other.free_list_head = nullptr;
		other.first_memory_chunk_ptr_ = nullptr;
		other.block_size_and_alignment_ = 0;
		other.object_size_ = 0;
		other.size_ = 0;
		other.capacity_ = 0;

		return *this;
	}

	~BaseContextBlockArena() {
		destroy();
	}
};

// =================================================================================================

struct entity_access {
	template <typename T>
	[[nodiscard]] static constexpr inline auto& ref_count(T& entity) noexcept {
		return entity.ref_count;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct entity_ptr {
	friend entity_store<T>;

private:
	T* ptr = nullptr;

public:
	constexpr inline void reset() noexcept {
		if (ptr == nullptr)
			return;

		const auto tmp = ptr;
		ptr = nullptr;

		if (--entity_access::ref_count(*tmp) != 0)
			return;

		auto ctx = entity_store<T>::context_from_pointer(tmp);
		// if (ctx == nullptr) // if ptr is not null, there isn't real reason to worry about null context
		//		return;

		ctx->destroy(tmp);
	}

private:
	explicit constexpr inline entity_ptr(T* ptr) noexcept : ptr(ptr) {
		++entity_access::ref_count(*ptr);
	}

public:
	constexpr inline entity_ptr() noexcept = default;

	constexpr inline entity_ptr(const std::nullptr_t) noexcept : ptr(nullptr) { }

	constexpr inline entity_ptr(const entity_ptr& other) noexcept : ptr(other.ptr) {
		if (other.ptr != nullptr)
			++entity_access::ref_count(*ptr);
	}

	constexpr inline entity_ptr& operator=(const entity_ptr& other) & noexcept {
		// NOTE: using this == &other instead of ptr == other.ptr to have all moved from state the same (and be reset)
		if (this == &other)
			return *this;

		reset();
		ptr = other.ptr;
		if (other.ptr != nullptr)
			++entity_access::ref_count(*ptr);
		return *this;
	}

	constexpr inline entity_ptr(entity_ptr&& other) noexcept : ptr(other.ptr) {
		other.ptr = nullptr;
	}

	constexpr inline entity_ptr& operator=(entity_ptr&& other) & noexcept {
		if (this == &other)
			return *this;

		reset();
		ptr = other.ptr;
		other.ptr = nullptr;
		return *this;
	}

	~entity_ptr() {
		reset();
	}

public:
	[[nodiscard]] constexpr inline T& operator*() const noexcept {
		return assert(ptr != nullptr), *ptr;
	}
	[[nodiscard]] constexpr inline T* operator->() const noexcept {
		return assert(ptr != nullptr), ptr;
	}
	[[nodiscard]] explicit constexpr inline operator bool() const noexcept {
		return ptr != nullptr;
	}
//	[[nodiscard]] explicit(false) constexpr inline operator T*() const noexcept {
//		return ptr;
//	}

public:
	/// Abandon the stored pointer without (dereferencing it and) decrementing its ref_count
	[[nodiscard]] constexpr inline T* abandon() noexcept {
		auto tmp = ptr;
		ptr = nullptr;
		return tmp;
	}

	/// Adopt a pointer without (dereferencing it and) incrementing its ref_count
	constexpr inline void adopt(T* ptr_) noexcept {
		reset();
		ptr = ptr_;
	}

public:
	[[nodiscard]] friend constexpr inline std::strong_ordering operator<=>(const entity_ptr& lhs, const entity_ptr& rhs) noexcept = default;
	[[nodiscard]] friend constexpr inline bool operator==(const entity_ptr& lhs, const entity_ptr& rhs) noexcept = default;

	[[nodiscard]] friend constexpr inline bool operator==(const entity_ptr& p, std::nullptr_t) noexcept {
		return p.ptr == nullptr;
	}
	[[nodiscard]] friend constexpr inline bool operator==(std::nullptr_t, const entity_ptr& p) noexcept {
		return nullptr == p.ptr;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const entity_ptr& p, std::nullptr_t) noexcept {
		return p.ptr != nullptr;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(std::nullptr_t, const entity_ptr& p) noexcept {
		return nullptr != p.ptr;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct entity_store : private BaseContextBlockArena {
	friend entity_ptr<T>;

private:
	/// Block size is at least 256 entry or 64 KB
	static constexpr std::size_t entry_size = sizeof(T);
	static constexpr std::size_t block_size = libv::max(std::bit_ceil(entry_size * 256 + BaseContextBlockArena::block_header_size), 64uz * 1024uz);
	static constexpr std::size_t block_alignment = block_size;
	static constexpr std::size_t block_capacity = block_size / entry_size - libv::int_ceil_div(BaseContextBlockArena::block_header_size, entry_size);

public:
	[[nodiscard]] static constexpr inline entity_store* context_from_pointer(T* ptr) noexcept {
		if (ptr == nullptr)
			return nullptr;

		auto* head = as_aligned<BlockHeader>(std::bit_cast<std::byte*>(std::bit_cast<std::size_t>(ptr) & ~(block_alignment - 1)));
		return reinterpret_cast<entity_store*>(head->context_ptr);
	}

public:
	explicit inline entity_store(std::size_t requested_capacity) :
		BaseContextBlockArena(
				this,
				block_alignment,
				libv::int_ceil_div(requested_capacity, block_capacity), // block_count
				entry_size) {
	}

	constexpr inline entity_store(const entity_store&) = delete;
	constexpr inline entity_store& operator=(const entity_store&) & = delete;
	constexpr inline entity_store(entity_store&& other) noexcept = default;
	constexpr inline entity_store& operator=(entity_store&& other) & noexcept = default;

//	entity_store* context_ptr() const {
//		return reinterpret_cast<entity_store*>(BaseContextBlockArena::context_ptr());
//	}

public:
	template <typename... Args>
	[[nodiscard]] inline entity_ptr<T> create(Args&&... args) {
		auto ptr = as_aligned<T>(BaseContextBlockArena::alloc());
		::new (ptr) T(std::forward<Args>(args)...);
		return entity_ptr{ptr};
	}

private:
	void destroy(T* ptr) {
		ptr->~T();
		BaseContextBlockArena::dealloc(reinterpret_cast<std::byte*>(ptr));
	}

public:
	[[nodiscard]] inline std::size_t size() const noexcept {
		return BaseContextBlockArena::size();
	}

	[[nodiscard]] inline std::size_t capacity() const noexcept {
		return BaseContextBlockArena::capacity();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
