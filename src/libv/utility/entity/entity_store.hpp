// Project: libv.utility, File: src/libv/utility/entity/entity_ptr.hpp

#pragma once

#include <libv/utility/entity/entity_ptr_fwd.hpp>

#include <bit>
#include <cassert>
#include <mutex>
#include <utility>

#include <libv/utility/ceil_div.hpp>
#include <libv/utility/min_max.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Entity Store memory layout:
//
//		MemoryChunks are aligned to block size, therefore any pointer pointing inside a block
//		can be converted to a BlockHeader safely (and from there via store_ptr the Store can be accessed too).
//
//      +-Store-----------------------         <-----------------.
//      | FLEntry* free_list_head              >--------.        |
//      | void* user_context >--> UserContext           |        |
//      | std::byte* first_memory_chunk_ptr    >-----.  |        |
//      +-----------------------------               |  |        |
//                                                   |  |        |
//                                                   |  |        |
//      +-MemoryChunk---------------           <-----.  |        |
//      | +-Block-----------------                      |        |
//      | | +-BlockHeader-------               <--------+--.     |
//      | | | void* store_ptr                  >--------+--+-----|
//      | | | void* user_context >--> UserContext       |  |     |
//      | | | BlockHeader* next_header         >--------+--|     |
//      | | | std::byte* next_memory_chunk_ptr >--------+--+--.  |
//      | | +-------------------                        |  |  |  |
//      | | <Padding> alignof(Object)                   |  |  |  |
//      | |                                             |  |  |  |
//      | | --FLEntry or Object-                        |  |  |  |
//      | | --FLEntry or Object-                        |  |  |  |
//      | | --FLEntry or Object-                        |  |  |  |
//      | |                                             |  |  |  |
//      | | +-FLEntry-----------               <--.     |  |  |  |
//      | | | FLEntry* next                    >--+--.  |  |  |  |
//      | | +-------------------                  |  |  |  |  |  |
//      | | <Padding> alignof(Object)             |  |  |  |  |  |
//      | |                                       |  |  |  |  |  |
//      | | --FLEntry or Object-                  |  |  |  |  |  |
//      | | --FLEntry or Object-                  |  |  |  |  |  |
//      | |                                       |  |  |  |  |  |
//      | | +-FLEntry-----------               <--+--+--.  |  |  |
//      | | | FLEntry* next                    >--.  |     |  |  |
//      | | +-------------------                     |     |  |  |
//      | | <Padding> alignof(Object)                |     |  |  |
//      | |                                          |     |  |  |
//      | | +-FLEntry-----------               <-----.     |  |  |
//      | | | FLEntry* next                    >-----.     |  |  |
//      | | +-------------------                     |     |  |  |
//      | | <Padding> alignof(Object)                |     |  |  |
//      | |                                          |     |  |  |
//      | | +-Object------------                     |     |  |  |
//      | | | object bytes                           |     |  |  |
//      | | +-------------------                     .     |  |  |
//      | |                                          .     |  |  |
//      | | +-Object------------                     .     |  |  |
//      | | | object bytes                                 |  |  |
//      | | +-------------------                           |  |  |
//      | |                                                |  |  |
//      | | ...                                            |  |  |
//      | +-----------------------                         |  |  |
//      |                                                  |  |  |
//      | +-Block-----------------                         |  |  |
//      | | +-BlockHeader-------               <-----------.  |  |
//      | | | void* store_ptr                  >--------------+--.
//      | | | void* user_context >--> UserContext             |
//      | | | BlockHeader* next_header         >-----------.  |
//      | | | std::byte* next_memory_chunk_ptr >--> null   |  |    // Only the first BlockHeader's next_memory_chunk_ptr
//      | | +-------------------                           |  |    // point to the next MemoryChunk, the rest is null.
//      | | <Padding> alignof(Object)                      |  |
//      | |                                                |  |
//      | | --FLEntry or Object-                           |  |
//      | | --FLEntry or Object-                           |  |
//      | | --FLEntry or Object-                           |  |
//      | | --FLEntry or Object-                           |  |
//      | | --FLEntry or Object-                           |  |
//      | |                                                |  |
//      | | ...                                            |  |
//      | +-----------------------                         |  |
//      +---------------------------                       |  |
//                                                         |  |
//                                                         |  |
//      +-MemoryChunk---------------           <-----------+--.
//      | +-Block-----------------                         |
//      | | +-BlockHeader-------               <-----------.
//      | | | ...
//      | | +-------------------
//      | | ...
//      | +-----------------------
//      | ...
//      +---------------------------

// -------------------------------------------------------------------------------------------------

struct BaseContextBlockArenaMemoryChunkInfo;

class BaseContextBlockArena {
public:
	struct BlockHeader {
		void* store_ptr;
		void* user_context;
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

	void* user_context = nullptr;
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
	[[nodiscard]] static constexpr inline T* as_aligned(std::byte* ptr) noexcept;

private:
	[[nodiscard]] static BaseContextBlockArenaMemoryChunkInfo allocateAndInitMemoryChunk(
			void* store_ptr, void* user_context, std::size_t block_size_and_alignment, std::size_t block_count, std::size_t object_size,
			std::byte* last_memory_chunk, BlockHeader* last_header, FLEntry* last_fl_entry);
	static inline FLEntry* createFLEntryAt(std::byte* at, FLEntry* next);
	static inline std::byte* destroyFLEntryAt(FLEntry* at);
	static inline void createHeaderAt(std::byte* at, void* store_ptr, void* user_context, BlockHeader* next_header);

	void allocateMoreMemory();

	template <typename Fn>
	void foreachBlockHeader(Fn&& func);

public:
	[[nodiscard]] inline std::byte* alloc();
	inline void dealloc(std::byte* ptr) noexcept;

public:
	[[nodiscard]] inline std::size_t size() const noexcept;
	[[nodiscard]] inline std::size_t capacity() const noexcept;

private:
	void update_store_ptr(void* store_ptr);
	void update_user_context(void* user_context);
	void destroy();

	inline BaseContextBlockArena() = default;

public:
	BaseContextBlockArena(
			void* store_ptr,
			void* user_context,
			std::size_t block_size_and_alignment,
			std::size_t block_count,
			std::size_t object_size);

	BaseContextBlockArena(const BaseContextBlockArena&) = delete;
	BaseContextBlockArena& operator=(const BaseContextBlockArena&) & = delete;
	BaseContextBlockArena(BaseContextBlockArena&& other) noexcept;
	BaseContextBlockArena& operator=(BaseContextBlockArena&& other) & noexcept;

	inline ~BaseContextBlockArena();
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class entity_store : private BaseContextBlockArena {
	friend entity_ptr<T>;

public:
	/// Block size is at least 256 entry or 64 KB
	static constexpr std::size_t entry_size = sizeof(T);
	static constexpr std::size_t block_size = libv::max(std::bit_ceil(entry_size * 256 + BaseContextBlockArena::block_header_size), 64uz * 1024uz);
	static constexpr std::size_t block_alignment = block_size;
	static constexpr std::size_t block_capacity = block_size / entry_size - libv::ceil_div(BaseContextBlockArena::block_header_size, entry_size);

public:
	explicit inline entity_store(std::size_t requested_capacity, void* user_context = nullptr);

	constexpr inline entity_store(const entity_store&) = delete;
	constexpr inline entity_store& operator=(const entity_store&) & = delete;
	constexpr inline entity_store(entity_store&& other) noexcept = default;
	constexpr inline entity_store& operator=(entity_store&& other) & noexcept = default;

public:
	[[nodiscard]] static constexpr inline entity_store* store_from_pointer(T* ptr) noexcept;
	[[nodiscard]] static constexpr inline entity_store* store_from_pointer(const entity_ptr<T>& ptr) noexcept;
	[[nodiscard]] static constexpr inline void* context_from_pointer(T* ptr) noexcept;
	[[nodiscard]] static constexpr inline void* context_from_pointer(const entity_ptr<T>& ptr) noexcept;

public:
	/// Does not run constructor, only provides raw memory.
	/// The returned pointer must be freed with entity_store.deallocate()
	[[nodiscard]] inline T* allocate();
	/// Does not run destructor, only releases the raw memory.
	/// The pointer must be a pointer allocated by the same entity_store.
	/// The exact entity_store used to allocate the memory can be accessed by
	/// entity_store::store_from_pointer(pointer) too (even from an already destructed object pointer);
	inline void deallocate(T* ptr) noexcept;

public:
	template <typename... Args>
	[[nodiscard]] inline primary_entity_ptr<T> create(Args&&... args);
	template <typename... Args>
	[[nodiscard]] inline entity_ptr<T> create_secondary(Args&&... args);

private:
	inline void destroy(T* ptr) noexcept;

public:
	[[nodiscard]] inline std::size_t size() const noexcept;
	[[nodiscard]] inline std::size_t capacity() const noexcept;
};

// =================================================================================================

template <typename T>
constexpr inline T* BaseContextBlockArena::as_aligned(std::byte* ptr) noexcept {
	return static_cast<T*>(static_cast<void*>(ptr));
}

inline BaseContextBlockArena::FLEntry* BaseContextBlockArena::createFLEntryAt(std::byte* at, FLEntry* next) {
	return ::new(at) FLEntry{next};
}

inline std::byte* BaseContextBlockArena::destroyFLEntryAt(FLEntry* at) {
	at->~FLEntry();
	return reinterpret_cast<std::byte*>(at);
}

inline void BaseContextBlockArena::createHeaderAt(std::byte* at, void* store_ptr, void* user_context, BlockHeader* next_header) {
	::new(at) BlockHeader{store_ptr, user_context, next_header};
}

inline std::byte* BaseContextBlockArena::alloc() { // Could be noexcept now, but in future growth would remove it
	auto lock = std::unique_lock(mutex);
	auto ptr = free_list_head;

	if (ptr == nullptr) [[unlikely]] {
		allocateMoreMemory();
		ptr = free_list_head;
	}

	free_list_head = free_list_head->next;
	++size_;

	return destroyFLEntryAt(ptr);
}

inline void BaseContextBlockArena::dealloc(std::byte* ptr) noexcept {
	auto lock = std::unique_lock(mutex);
	assert(ptr != nullptr);
	--size_;
	free_list_head = createFLEntryAt(ptr, free_list_head);
}

inline std::size_t BaseContextBlockArena::size() const noexcept {
	auto lock = std::unique_lock(mutex);
	return size_;
}

inline std::size_t BaseContextBlockArena::capacity() const noexcept {
	auto lock = std::unique_lock(mutex);
	return capacity_;
}

inline BaseContextBlockArena::~BaseContextBlockArena() {
	destroy();
}

// =================================================================================================

template <typename T>
constexpr inline entity_store<T>* entity_store<T>::store_from_pointer(T* ptr) noexcept {
	if (ptr == nullptr)
		return nullptr;

	auto* blockHeader = std::bit_cast<BlockHeader*>(std::bit_cast<uintptr_t>(ptr) & ~(block_alignment - 1));
	return reinterpret_cast<entity_store*>(blockHeader->store_ptr);
}

template <typename T>
constexpr inline entity_store<T>* entity_store<T>::store_from_pointer(const entity_ptr<T>& ptr) noexcept {
	return store_from_pointer(ptr.get());
}

template <typename T>
constexpr inline void* entity_store<T>::context_from_pointer(T* ptr) noexcept {
	if (ptr == nullptr)
		return nullptr;

	auto* blockHeader = std::bit_cast<BlockHeader*>(std::bit_cast<uintptr_t>(ptr) & ~(block_alignment - 1));
	return blockHeader->user_context;
}

template <typename T>
constexpr inline void* entity_store<T>::context_from_pointer(const entity_ptr<T>& ptr) noexcept {
	return context_from_pointer(ptr.get());
}

template <typename T>
inline entity_store<T>::entity_store(std::size_t requested_capacity, void* user_context) :
	BaseContextBlockArena(
			this,
			user_context,
			block_alignment,
			libv::ceil_div(requested_capacity, block_capacity), // block_count
			entry_size) {
}

template <typename T>
inline T* entity_store<T>::allocate() {
	// Check is placed here to allow fwd declared type usage in store
	static_assert(sizeof(T) >= BaseContextBlockArena::free_list_entry_size,
			"sizeof(T) must be at least that size of a pointer");

	auto* ptr = as_aligned<T>(BaseContextBlockArena::alloc());
	return ptr;
}

template <typename T>
inline void entity_store<T>::deallocate(T* ptr) noexcept {
	BaseContextBlockArena::dealloc(reinterpret_cast<std::byte*>(ptr));
}

template <typename T>
template <typename... Args>
inline primary_entity_ptr<T> entity_store<T>::create(Args&&... args) {
	auto* ptr = allocate();
	::new (ptr) T(std::forward<Args>(args)...);
	return primary_entity_ptr{ptr};
}

template <typename T>
template <typename... Args>
inline entity_ptr<T> entity_store<T>::create_secondary(Args&&... args) {
	auto* ptr = allocate();
	::new (ptr) T(std::forward<Args>(args)...);
	return entity_ptr{ptr};
}

template <typename T>
inline void entity_store<T>::destroy(T* ptr) noexcept {
	ptr->~T();
	deallocate(ptr);
}

template <typename T>
inline std::size_t entity_store<T>::size() const noexcept {
	return BaseContextBlockArena::size();
}

template <typename T>
inline std::size_t entity_store<T>::capacity() const noexcept {
	return BaseContextBlockArena::capacity();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
