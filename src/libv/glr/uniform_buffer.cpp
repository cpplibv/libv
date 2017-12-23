// File:   uniform_buffer.cpp Author: Vader Created on 2019. január 25., 5:34

// hpp
#include <libv/glr/uniform_buffer.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/uniform_buffer.hpp>
#include <libv/utility/align.hpp>
// pro
#include <libv/glr/remote.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

void RemoteUniformBuffer::create(libv::gl::GL& gl, Remote& remote_) noexcept {
	gl(buffer).create();

	remote = libv::make_observer(remote_);
	created = true;
}

void RemoteUniformBuffer::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (!created)
		create(gl, remote_);

	gl(buffer).bind();
	gl(buffer).data(data, usage);
	dirty = false;
}

void RemoteUniformBuffer::bind(libv::gl::GL& gl, Remote& remote_, UniformBlockBindingLocation binding, const UniformDataBlock& block) noexcept {
	if (dirty)
		update(gl, remote_);

	gl(buffer).bindRange(binding, block.offset, block.size);
}

RemoteUniformBuffer::~RemoteUniformBuffer() noexcept {
	if (remote)
		remote->gc(buffer);

	assert(frees.empty() ||
			(frees.size() == 1 && frees.begin()->offset == 0 && frees.begin()->size == data.size()));
}

UniformDataBlock RemoteUniformBuffer::allocate(size_t size) noexcept {
	size = libv::align_of_2(size, BLOCK_ALIGNMENT);

	UniformDataBlock result;
	result.size = size;

	for (auto it = frees.begin(); it != frees.end(); it++) {

		// Perfect fit
		if (it->size == size) {
			result.offset = it->offset;
			frees.erase(it);
			return result;

		// Over fit -> Slice free segment
		} else if (it->size > size) {
			result.offset = it->offset;
			it->offset += size;
			it->size -= size;
			return result;
		}
	}

	// No free segment -> Expand memory
	result.offset = data.size();
	data.resize(data.size() + size);
	return result;
}

void RemoteUniformBuffer::deallocate(UniformDataBlock block) noexcept {
	// 0 -----|-----|*****|-----|-----  A and  B
	// 1 -----|-----|*****|#####|-----  A and !B     - free
	// 2 -----|#####|*****|-----|----- !A and  B     * block
	// 3 -----|#####|*****|#####|----- !A and !B     # used
	//
	// A = prev.index + prev.size == block.index
	// B = block.index + block.size == next.index

	// as block.index cannot be a part of the free list, lower_bound always yields the next or the end
	const auto it = frees.lower_bound(block.offset);

	const auto has_prev = it != frees.begin();
	const auto has_next = it != frees.end();

	const auto prev = has_prev ? std::prev(it) : it;
	const auto next = it;

	const auto merge_prev = has_prev && prev->offset + prev->size == block.offset;
	const auto merge_next = has_next && block.offset + block.size == next->offset;

	if (merge_prev && merge_next) { // 0
		prev->size += next->size + block.size;
		frees.erase(next);

	} else if (merge_prev && !merge_next) { // 1
		prev->size += block.size;

	} else if (!merge_prev && merge_next) { // 2
		next->offset = block.offset;
		next->size += block.size;

	} else { // 3
		frees.insert(block);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
