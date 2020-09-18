// Project: libv.glr, File: src/libv/glr/uniform_buffer.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/gl/gl_fwd.hpp>
#include <libv/utility/observer_ptr.hpp>
// std
#include <memory>
#include <vector>
// pro
#include <libv/gl/uniform_buffer_object.hpp>
// TODO P2: remove this include layout_std140 if possible from here
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_block_layout.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class DestroyQueues;
class RemoteUniformBuffer;
struct Remote;

struct UniformDataBlock {
	size_t offset;
	size_t size;

	inline bool operator<(UniformDataBlock rhs) const noexcept {
		return offset < rhs.offset;
	}

	inline bool operator<(size_t index_) const noexcept {
		return offset < index_;
	}
};

class RemoteUniformBuffer {
public:
	bool dirty = false;
	bool created = false;

	//uint32_t ref_count = 0;

	libv::gl::UniformBuffer buffer;
	libv::gl::BufferUsage usage = libv::gl::BufferUsage::StaticDraw;

	std::vector<std::byte> data;
	boost::container::flat_set<UniformDataBlock, std::less<>> frees;
	static constexpr size_t BLOCK_ALIGNMENT = 256;
	// TODO P5: load BLOCK_ALIGNMENT run-time

	libv::observer_ptr<DestroyQueues> remote = nullptr;

private:
	void create(libv::gl::GL& gl, Remote& remote_) noexcept;
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	void bind(libv::gl::GL& gl, Remote& remote_, UniformBlockBindingLocation binding, const UniformDataBlock& block) noexcept;

	RemoteUniformBuffer(libv::gl::BufferUsage usage) noexcept : usage(usage) { }
	~RemoteUniformBuffer() noexcept;

	RemoteUniformBuffer(const RemoteUniformBuffer&) = delete;
	RemoteUniformBuffer& operator=(const RemoteUniformBuffer&) = delete;
	RemoteUniformBuffer(RemoteUniformBuffer&&) = delete;
	RemoteUniformBuffer& operator=(RemoteUniformBuffer&&) = delete;

public:
	UniformDataBlock allocate(size_t size) noexcept;
	void deallocate(UniformDataBlock block) noexcept;
};

// -------------------------------------------------------------------------------------------------

class UniformBlockUniqueView_std140 {
private:
	template <typename T>
	struct Proxy {
		libv::observer_ref<std::byte> ptr;

		void operator=(const T& value) && {
			write_std140(ptr, value);
		}
	};

public:
	UniformBlockBindingLocation binding;
	UniformDataBlock block;
	std::shared_ptr<RemoteUniformBuffer> remote;

public:
	UniformBlockUniqueView_std140(
			UniformBlockBindingLocation binding,
			UniformDataBlock block,
			const std::shared_ptr<RemoteUniformBuffer>& remote) :
		binding(binding),
		block(block),
		remote(remote) { }

	UniformBlockUniqueView_std140(const UniformBlockUniqueView_std140&) = delete;
	UniformBlockUniqueView_std140& operator=(const UniformBlockUniqueView_std140&) = delete;

	UniformBlockUniqueView_std140(UniformBlockUniqueView_std140&& orig) :
		binding(orig.binding),
		block(orig.block),
		remote(std::move(orig.remote)) {
		orig.remote.reset();
	};

	UniformBlockUniqueView_std140& operator=(UniformBlockUniqueView_std140&& orig) {
		binding = orig.binding;
		block = orig.block;
		remote = std::move(orig.remote);
		orig.remote.reset();
		return *this;
	};

public:
	template <typename T>
	Proxy<T> operator[](const libv::glr::Uniform_t<T>& location) {
		remote->dirty = true;
		const auto ptr = libv::make_observer_ref(remote->data.data() + block.offset + location.location);
		return Proxy<T>{ptr};
	}

	~UniformBlockUniqueView_std140() {
		if (remote)
			remote->deallocate(block);
	}
};

// -------------------------------------------------------------------------------------------------

class UniformBuffer {
	friend class AttorneyUniformBufferRemote;

private:
	std::shared_ptr<RemoteUniformBuffer> remote;

public:
	UniformBuffer(libv::gl::BufferUsage usage) :
		remote(std::make_shared<RemoteUniformBuffer>(usage)) { }
	UniformBuffer(const UniformBuffer& orig) = default;
	UniformBuffer(UniformBuffer&& orig) = default;

public:
	template <typename T>
	[[nodiscard]] UniformBlockUniqueView_std140 block_unique(const UniformBlockLayout<T>& layout) {
		remote->dirty = true;
		return UniformBlockUniqueView_std140{layout.binding, remote->allocate(layout.size), remote};
	}

	void reserve(size_t byte_size) {
		remote->data.reserve(byte_size);
	}
};

// =================================================================================================

class UniformBlockStreamView_std140 {
private:
	template <typename T>
	struct Proxy {
		libv::observer_ref<std::byte> ptr;

		inline void operator=(const T& value) && {
			write_std140(ptr, value);
		}
	};

public:
	UniformBlockBindingLocation binding;
	UniformDataBlock block;
	std::shared_ptr<RemoteUniformBuffer> remote;

public:
	UniformBlockStreamView_std140(
			UniformBlockBindingLocation binding,
			UniformDataBlock block,
			const std::shared_ptr<RemoteUniformBuffer>& remote) :
		binding(binding),
		block(block),
		remote(remote) { }

	UniformBlockStreamView_std140(const UniformBlockStreamView_std140& other) = default;
	UniformBlockStreamView_std140(UniformBlockStreamView_std140&& other) = default;
	UniformBlockStreamView_std140& operator=(const UniformBlockStreamView_std140& other) & = default;
	UniformBlockStreamView_std140& operator=(UniformBlockStreamView_std140&& other) & = default;

public:
	template <typename T>
	Proxy<T> operator[](const libv::glr::Uniform_t<T>& location) {
		remote->dirty = true;
		const auto ptr = libv::make_observer_ref(remote->data.data() + block.offset + location.location);
		return Proxy<T>{ptr};
	}

	~UniformBlockStreamView_std140() {
		// The uniform memory is deallocated by a stream clear function
	}
};

// -------------------------------------------------------------------------------------------------

class UniformBufferStream {
	friend class AttorneyUniformBufferRemote;

private:
	std::shared_ptr<RemoteUniformBuffer> remote;

public:
	UniformBufferStream(libv::gl::BufferUsage usage = libv::gl::BufferUsage::StreamDraw) :
		remote(std::make_shared<RemoteUniformBuffer>(usage)) { }

	UniformBufferStream(const UniformBufferStream& orig) = default;
	UniformBufferStream(UniformBufferStream&& orig) = default;

public:
	template <typename T>
	[[nodiscard]] UniformBlockStreamView_std140 block_stream(const UniformBlockLayout<T>& layout) {
		remote->dirty = true;
		return UniformBlockStreamView_std140{layout.binding, remote->allocate(layout.size), remote};
	}

	const auto& tmp() {
		return remote->data;
	}

	void clear() {
		remote->data.clear();
		remote->frees.clear();
	}
};

// -------------------------------------------------------------------------------------------------

struct AttorneyUniformBufferRemote {
	friend class Queue;
	static inline RemoteUniformBuffer& remote(const UniformBuffer& object) {
		return *object.remote;
	}
	static inline RemoteUniformBuffer& remote(const UniformBufferStream& object) {
		return *object.remote;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
