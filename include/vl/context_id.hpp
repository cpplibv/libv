// File:   context_id.hpp

#pragma once

namespace vl {

enum class ThreadID {
	IO,
	GL,
	NET,
	ANY
};

struct TaskFlag_t {
	bool sync;
	constexpr TaskFlag_t(bool sync) : sync(sync) {}
};

struct SyncFlag_t : TaskFlag_t {
	constexpr SyncFlag_t() : TaskFlag_t(true) {}
};

struct AsyncFlag_t : TaskFlag_t {
	constexpr AsyncFlag_t() : TaskFlag_t(false) {}
};

namespace TaskFlag {

constexpr SyncFlag_t SYNC;
constexpr AsyncFlag_t ASYNC;

} //namespace ThreadFlag

} //namespace vl