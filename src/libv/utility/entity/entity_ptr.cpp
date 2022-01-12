// Project: libv.utility, File: src/libv/utility/entity/entity_ptr.cpp

// hpp
#include <libv/utility/entity/entity_ptr.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct BaseContextBlockArenaMemoryChunk {
	std::byte* memory_chunk_ptr; /// Owning pointer
	BaseContextBlockArena::FLEntry* first_fl_entry;
	std::size_t chunk_capacity;
};

BaseContextBlockArenaMemoryChunk BaseContextBlockArena::allocateAndInitMemoryChunk(
		void* context_ptr,
		std::size_t block_size_and_alignment,
		std::size_t block_count,
		std::size_t object_size,
		std::byte* last_memory_chunk,
		BlockHeader* last_header,
		FLEntry* last_fl_entry
	) {

	BaseContextBlockArenaMemoryChunk result;

	const auto block_size = block_size_and_alignment;
	const auto block_alignment = block_size_and_alignment;
	const auto block_capacity = block_size / object_size - libv::int_ceil_div(block_header_size, object_size);

	// Allocate memory
	const auto memory_size = block_size * block_count;
	result.memory_chunk_ptr = new(std::align_val_t(block_alignment)) std::byte[memory_size];
	// result.memory_chunk_ptr = std::aligned_alloc(block_size_and_alignment_, memory_size);

	result.chunk_capacity = block_capacity * block_count;

	//
	// Create Block Headers (Creating in forward order, so as_aligned used to promise the objects that are not there yet)

	for (auto block_index = 0uz; block_index < block_count - 1; ++block_index) {
		const auto next_header = as_aligned<BlockHeader>(result.memory_chunk_ptr + (block_index + 1) * block_size);
		createHeaderAt(result.memory_chunk_ptr + block_index * block_size, context_ptr, next_header);
	}
	createHeaderAt(result.memory_chunk_ptr + (block_count - 1) * block_size, context_ptr, last_header); // Set last entry as sentinel

	// Set first blocks header's next mem chunk
	as_aligned<BlockHeader>(result.memory_chunk_ptr)->next_memory_chunk_ptr_ = last_memory_chunk;

	//
	// Fill in the free list (Creating in forward order, so as_aligned used to promise the objects that are not there yet)

	auto prev_entry_ptr = result.memory_chunk_ptr + libv::int_ceil_div(block_header_size, object_size) * object_size;
	result.first_fl_entry = as_aligned<FLEntry>(prev_entry_ptr);

	for (auto block_index = 0uz; block_index < block_count; ++block_index) {
		const auto block_start_ptr = result.memory_chunk_ptr + block_index * block_size;

		auto entry_offset = libv::int_ceil_div(block_header_size, object_size) * object_size;
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
	const auto context_ptr = prev_first_block_header->context_ptr;

	const auto block_capacity = block_size_and_alignment_ / object_size_ - libv::int_ceil_div(BaseContextBlockArena::block_header_size, object_size_);
	const auto new_chunk_count = libv::int_ceil_div(libv::max(capacity_ / 2, 1uz), block_capacity);

	auto m = allocateAndInitMemoryChunk(context_ptr, block_size_and_alignment_, new_chunk_count, object_size_, prev_first_memory_chunk, prev_first_block_header, prev_free_list_head);
	free_list_head = m.first_fl_entry;
	first_memory_chunk_ptr_ = m.memory_chunk_ptr;
	capacity_ += m.chunk_capacity;
}

BaseContextBlockArena::BaseContextBlockArena(
		void* context_ptr,
		std::size_t block_size_and_alignment,
		std::size_t block_count,
		std::size_t object_size) :
		BaseContextBlockArena() { // Calling delegated default ctor to get a dtor call in case of an exception

	block_size_and_alignment_ = block_size_and_alignment;
	object_size_ = object_size;

	auto m = allocateAndInitMemoryChunk(context_ptr, block_size_and_alignment, block_count, object_size, nullptr, nullptr, nullptr);
	free_list_head = m.first_fl_entry;
	first_memory_chunk_ptr_ = m.memory_chunk_ptr;
	capacity_ = m.chunk_capacity;
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
		::operator delete(memory_chunk_ptr, std::align_val_t(block_size_and_alignment_));
		// std::free(memory_ptr_); use with std::aligned_alloc

		memory_chunk_ptr = next_chunk;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
