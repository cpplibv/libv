// Project: libv.glr, File: src/libv/glr/remote.cpp

// hpp
#include <libv/glr/remote.hpp>
// libv
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/gl.hpp>
// std
#include <optional>
#include <thread>
// pro
#include <libv/glr/destroy_queue.hpp>
#include <libv/glr/queue.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class ImplRemote {
public:
	std::optional<libv::gl::GL> gl;
	std::vector<Queue> queues;
	DestroyQueues destroyQueues;
	std::thread::id contextThreadID;
	bool initalized = false;
};

// -------------------------------------------------------------------------------------------------

Remote::Remote() : self(std::make_unique<ImplRemote>()) {}

Remote::~Remote() {
	if (self->initalized) {
		LIBV_GL_DEBUG_ASSERT(std::this_thread::get_id() == self->contextThreadID);
		destroy();
	}
}

Queue Remote::queue() {
	return Queue{*this};
}

void Remote::queue(Queue&& queue) {
	self->queues.emplace_back(std::move(queue));
}

DestroyQueues& Remote::destroyQueues() noexcept {
	return self->destroyQueues;
}

void Remote::clear() noexcept {
	self->queues.clear();
	self->destroyQueues.clear(*self->gl);
}

void Remote::enableTrace() noexcept {
	self->gl->enableTrace();
}

void Remote::enableDebug() noexcept {
	self->gl->enableDebug();
}

void Remote::readPixels(libv::vec2i pos, libv::vec2i size, libv::gl::ReadFormat format, libv::gl::DataType type, void* data) noexcept {
	self->gl->readPixels(pos, size, format, type, data);
}

libv::GL& Remote::gl() noexcept {
	LIBV_GL_DEBUG_ASSERT(std::this_thread::get_id() == self->contextThreadID);
	LIBV_GL_DEBUG_ASSERT(self->initalized);
	return *self->gl;
}

// -------------------------------------------------------------------------------------------------

void Remote::create() {
	LIBV_GL_DEBUG_ASSERT(!self->initalized);
	self->gl.emplace();
	self->contextThreadID = std::this_thread::get_id();
	self->initalized = true;
}

void Remote::destroy() {
	LIBV_GL_DEBUG_ASSERT(std::this_thread::get_id() == self->contextThreadID);
	LIBV_GL_DEBUG_ASSERT(self->initalized);
	clear();
	self->gl->clear();
	self->initalized = false;
}

void Remote::execute() {
	for (auto& queue : self->queues)
		queue.execute(*self->gl, *this);
	clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
