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
		auto lock = std::unique_lock(mutex);
		return size_;
	}

	[[nodiscard]] inline std::size_t capacity() const noexcept {
		auto lock = std::unique_lock(mutex);
		return capacity_;
	}

	[[nodiscard]] inline void* context_ptr() const noexcept {
		auto lock = std::unique_lock(mutex);
		return as_aligned<BlockHeader>(first_memory_chunk_ptr_)->context_ptr;
	}

private:
	void update_context_ptr(void* context_ptr);
	void destroy();

	inline BaseContextBlockArena() = default;

public:
	BaseContextBlockArena(
			void* context_ptr,
			std::size_t block_size_and_alignment,
			std::size_t block_count,
			std::size_t object_size);

	BaseContextBlockArena(const BaseContextBlockArena&) = delete;
	BaseContextBlockArena& operator=(const BaseContextBlockArena&) & = delete;
	BaseContextBlockArena(BaseContextBlockArena&& other) noexcept;
	BaseContextBlockArena& operator=(BaseContextBlockArena&& other) & noexcept;

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

protected:
	explicit constexpr inline entity_ptr(T* ptr) noexcept : ptr(ptr) {
		assert(ptr != nullptr && "Internal error: Entity_ptr cannot be null on direct construction");
		++entity_access::ref_count(*ptr);
	}

public:
	constexpr inline entity_ptr() noexcept = default;

	explicit(false) constexpr inline entity_ptr(std::nullptr_t) noexcept : ptr(nullptr) { }

	constexpr inline entity_ptr(const entity_ptr& other) noexcept : ptr(other.ptr) {
		if (ptr != nullptr)
			++entity_access::ref_count(*ptr);
	}

	constexpr inline entity_ptr& operator=(std::nullptr_t) & noexcept {
		reset();
		return *this;
	}

	constexpr inline entity_ptr& operator=(const entity_ptr& other) & noexcept {
		// NOTE: using this == &other instead of ptr == other.ptr to have all moved from state the same (and be reset)
		if (this == &other)
			return *this;

		reset();
		ptr = other.ptr;

		if (ptr != nullptr)
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

	explicit(false) constexpr inline entity_ptr(const primary_entity_ptr<T>& other) noexcept :
		entity_ptr(static_cast<const entity_ptr&>(other)) { }

	constexpr inline entity_ptr& operator=(const primary_entity_ptr<T>& other) & noexcept {
		return *this = static_cast<const entity_ptr&>(other);
	}

	explicit(false) constexpr inline entity_ptr(primary_entity_ptr<T>&& other) noexcept :
		ptr(other.get()) {
		other.kill();
		other.ptr = nullptr;
	}

	constexpr inline entity_ptr& operator=(primary_entity_ptr<T>&& other) & noexcept {
		reset();
		ptr = other.ptr;
		other.kill();
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
	[[nodiscard]] constexpr inline T* get() const noexcept {
		return ptr;
	}

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
struct primary_entity_ptr : public entity_ptr<T> {
	friend entity_store<T>;

private:
	explicit constexpr inline primary_entity_ptr(T* ptr) noexcept : entity_ptr<T>(ptr) {}

public:
	constexpr inline primary_entity_ptr() noexcept = default;

	constexpr inline primary_entity_ptr(const primary_entity_ptr&) noexcept = delete;
	constexpr inline primary_entity_ptr& operator=(const primary_entity_ptr&) & noexcept = delete;
	constexpr inline primary_entity_ptr(primary_entity_ptr&&) noexcept = default;
	constexpr inline primary_entity_ptr& operator=(primary_entity_ptr&& other) & noexcept {
		kill();
		static_cast<entity_ptr<T>&>(*this) = static_cast<entity_ptr<T>&&>(other);
		return *this;
	}

	explicit(false) constexpr inline primary_entity_ptr(std::nullptr_t) noexcept : entity_ptr<T>(nullptr) { }

	void reset() {
		kill();
		entity_ptr<T>::reset();
	}

	void kill() {
		if constexpr (requires { this->get()->kill(); } )
			if (this->get() != nullptr)
				this->get()->kill();
	}

	~primary_entity_ptr() {
		kill();
	}

public:
	[[nodiscard]] friend constexpr inline std::strong_ordering operator<=>(const primary_entity_ptr& lhs, const primary_entity_ptr& rhs) noexcept = default;
	[[nodiscard]] friend constexpr inline bool operator==(const primary_entity_ptr& lhs, const primary_entity_ptr& rhs) noexcept = default;

	[[nodiscard]] friend constexpr inline bool operator==(const primary_entity_ptr& p, std::nullptr_t) noexcept {
		return p.ptr == nullptr;
	}
	[[nodiscard]] friend constexpr inline bool operator==(std::nullptr_t, const primary_entity_ptr& p) noexcept {
		return nullptr == p.ptr;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const primary_entity_ptr& p, std::nullptr_t) noexcept {
		return p.ptr != nullptr;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(std::nullptr_t, const primary_entity_ptr& p) noexcept {
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
	[[nodiscard]] inline primary_entity_ptr<T> create(Args&&... args) {
		auto ptr = as_aligned<T>(BaseContextBlockArena::alloc());
		::new (ptr) T(std::forward<Args>(args)...);
		return primary_entity_ptr{ptr};
	}

	template <typename... Args>
	[[nodiscard]] inline entity_ptr<T> create_secondary(Args&&... args) {
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
