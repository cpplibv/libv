// Project: libv.utility, File: src/libv/utility/entity/entity_ptr.cpp

// hpp
#include <libv/utility/entity/entity_ptr.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct BaseContextBlockArenaMemoryChunkInfo {
	std::byte* memory_chunk_ptr; /// Owning pointer
	BaseContextBlockArena::FLEntry* first_fl_entry;
	std::size_t chunk_capacity;
};

BaseContextBlockArenaMemoryChunkInfo BaseContextBlockArena::allocateAndInitMemoryChunk(
		void* store_ptr,
		void* user_context,
		std::size_t block_size_and_alignment,
		std::size_t block_count,
		std::size_t object_size,
		std::byte* last_memory_chunk,
		BlockHeader* last_header,
		FLEntry* last_fl_entry
	) {

	BaseContextBlockArenaMemoryChunkInfo result;

	const auto block_size = block_size_and_alignment;
	const auto block_alignment = block_size_and_alignment;
	const auto block_capacity = block_size / object_size - libv::ceil_div(block_header_size, object_size);

	// Allocate memory
	const auto memory_size = block_size * block_count;
	result.memory_chunk_ptr = new(std::align_val_t(block_alignment)) std::byte[memory_size];
	// result.memory_chunk_ptr = std::aligned_alloc(block_size_and_alignment_, memory_size);

	result.chunk_capacity = block_capacity * block_count;

	//
	// Create Block Headers (Creating in forward order, so as_aligned used to promise the objects that are not there yet)

	for (auto block_index = 0uz; block_index < block_count - 1; ++block_index) {
		const auto next_header = as_aligned<BlockHeader>(result.memory_chunk_ptr + (block_index + 1) * block_size);
		createHeaderAt(result.memory_chunk_ptr + block_index * block_size, store_ptr, user_context, next_header);
	}
	createHeaderAt(result.memory_chunk_ptr + (block_count - 1) * block_size, store_ptr, user_context, last_header); // Set last entry as sentinel

	// Set first blocks header's next mem chunk
	as_aligned<BlockHeader>(result.memory_chunk_ptr)->next_memory_chunk_ptr_ = last_memory_chunk;

	//
	// Fill in the free list (Creating in forward order, so as_aligned used to promise the objects that are not there yet)

	auto prev_entry_ptr = result.memory_chunk_ptr + libv::ceil_div(block_header_size, object_size) * object_size;
	result.first_fl_entry = as_aligned<FLEntry>(prev_entry_ptr);

	for (auto block_index = 0uz; block_index < block_count; ++block_index) {
		const auto block_start_ptr = result.memory_chunk_ptr + block_index * block_size;

		auto entry_offset = libv::ceil_div(block_header_size, object_size) * object_size;
		for (; entry_offset + object_size < block_size; entry_offset += object_size) {
			auto entry_ptr = block_start_ptr + entry_offset;
			createFLEntryAt(prev_entry_ptr, as_aligned<FLEntry>(entry_ptr));
			prev_entry_ptr = entry_ptr;
		}
	}
	createFLEntryAt(prev_entry_ptr, last_fl_entry); // Set last entry as sentinel

	return result;
}

void BaseContextBlockArena::allocateMoreMemory() {
	const auto prev_free_list_head = free_list_head;
	const auto prev_first_memory_chunk = first_memory_chunk_ptr_;
	const auto prev_first_block_header = as_aligned<BlockHeader>(prev_first_memory_chunk);
	const auto store_ptr = prev_first_block_header->store_ptr;

	const auto block_capacity = block_size_and_alignment_ / object_size_ - libv::ceil_div(BaseContextBlockArena::block_header_size, object_size_);
	const auto new_chunk_count = libv::ceil_div(libv::max(capacity_ / 2, 1uz), block_capacity);

	auto m = allocateAndInitMemoryChunk(store_ptr, user_context, block_size_and_alignment_, new_chunk_count, object_size_, prev_first_memory_chunk, prev_first_block_header, prev_free_list_head);
	free_list_head = m.first_fl_entry;
	first_memory_chunk_ptr_ = m.memory_chunk_ptr;
	capacity_ += m.chunk_capacity;
}

template<typename Fn>
void BaseContextBlockArena::foreachBlockHeader(Fn&& func) {
	auto memory_chunk_ptr = first_memory_chunk_ptr_;

	while (memory_chunk_ptr != nullptr) {
		const auto first_block_header = as_aligned<BlockHeader>(memory_chunk_ptr);
		const auto next_chunk = first_block_header->next_memory_chunk_ptr_;

		auto block_it = first_block_header;
		while (block_it != nullptr) {
			auto next = block_it->next_header;
			func(*block_it);
			block_it = next;
		}

		memory_chunk_ptr = next_chunk;
	}
}

void BaseContextBlockArena::update_store_ptr(void* store_ptr) {
	foreachBlockHeader([&](BlockHeader& block_header) {
		block_header.store_ptr = store_ptr;
	});
}

void BaseContextBlockArena::update_user_context(void* user_context) {
	foreachBlockHeader([&](BlockHeader& block_header) {
		block_header.user_context = user_context;
	});
}

void BaseContextBlockArena::destroy() {
	auto memory_chunk_ptr = first_memory_chunk_ptr_;

	while (memory_chunk_ptr != nullptr) {
		const auto first_block_header = as_aligned<BlockHeader>(memory_chunk_ptr);
		const auto next_chunk = first_block_header->next_memory_chunk_ptr_;

		// Destroy Block Headers
		auto block_it = first_block_header;
		while (block_it != nullptr) {
			auto next = block_it->next_header;
			block_it->~BlockHeader();
			block_it = next;
		}

		// Deallocate memory chunk
		::operator delete[](memory_chunk_ptr, std::align_val_t(block_size_and_alignment_));
		// std::free(memory_ptr_); use with std::aligned_alloc

		memory_chunk_ptr = next_chunk;
	}
}

BaseContextBlockArena::BaseContextBlockArena(
		void* store_ptr,
		void* user_context,
		std::size_t block_size_and_alignment,
		std::size_t block_count,
		std::size_t object_size) :
		BaseContextBlockArena() { // Calling delegated default ctor to get a dtor call in case of an exception

	block_size_and_alignment_ = block_size_and_alignment;
	object_size_ = object_size;

	auto m = allocateAndInitMemoryChunk(store_ptr, user_context, block_size_and_alignment, block_count, object_size, nullptr, nullptr, nullptr);
	free_list_head = m.first_fl_entry;
	first_memory_chunk_ptr_ = m.memory_chunk_ptr;
	capacity_ = m.chunk_capacity;
}

BaseContextBlockArena::BaseContextBlockArena(BaseContextBlockArena&& other) noexcept :
		free_list_head(other.free_list_head),
		user_context(other.user_context),
		first_memory_chunk_ptr_(other.first_memory_chunk_ptr_),
		block_size_and_alignment_(other.block_size_and_alignment_),
		object_size_(other.object_size_),
		size_(other.size_),
		capacity_(other.capacity_) {
	other.free_list_head = nullptr;
	other.user_context = nullptr;
	other.first_memory_chunk_ptr_ = nullptr;
	other.block_size_and_alignment_ = 0;
	other.object_size_ = 0;
	other.size_ = 0;
	other.capacity_ = 0;

	update_store_ptr(this);
}

BaseContextBlockArena& BaseContextBlockArena::operator=(BaseContextBlockArena&& other) & noexcept {
	if (&other == this)
		return *this;

	destroy();

	free_list_head = other.free_list_head;
	user_context = other.user_context;
	first_memory_chunk_ptr_ = other.first_memory_chunk_ptr_;
	block_size_and_alignment_ = other.block_size_and_alignment_;
	object_size_ = other.object_size_;
	size_ = other.size_;
	capacity_ = other.capacity_;

	other.free_list_head = nullptr;
	other.user_context = nullptr;
	other.first_memory_chunk_ptr_ = nullptr;
	other.block_size_and_alignment_ = 0;
	other.object_size_ = 0;
	other.size_ = 0;
	other.capacity_ = 0;

	update_store_ptr(this);

	return *this;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
