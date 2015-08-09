// File:   context_id.hpp

#pragma once

namespace vl {

// -------------------------------------------------------------------------------------------------

enum class ContextID {
	ANY,
	GL,
	IO,
	NET
};

// -------------------------------------------------------------------------------------------------

struct TaskFlag_t {
	bool sync;
	constexpr TaskFlag_t(bool sync) : sync(sync) { }
};

struct SyncFlag_t : TaskFlag_t {
	constexpr SyncFlag_t() : TaskFlag_t(true) { }
};

struct AsyncFlag_t : TaskFlag_t {
	constexpr AsyncFlag_t() : TaskFlag_t(false) { }
};

// -------------------------------------------------------------------------------------------------

struct ContextAffinity {
	static inline ContextAffinity IO(size_t priority) {
		return ContextAffinity(ContextID::IO, priority);
	}
	static inline ContextAffinity GL(size_t priority) {
		return ContextAffinity(ContextID::GL, priority);
	}
	static inline ContextAffinity NET(size_t priority) {
		return ContextAffinity(ContextID::NET, priority);
	}
	static inline ContextAffinity ANY(size_t priority) {
		return ContextAffinity(ContextID::ANY, priority);
	}

	ContextID context;
	size_t priority;
	ContextAffinity(ContextID context, size_t priority) :
		context(context), priority(priority) { }
};

// -------------------------------------------------------------------------------------------------

namespace TaskFlag {

constexpr SyncFlag_t SYNC;
constexpr AsyncFlag_t ASYNC;

} //namespace ThreadFlag

// -------------------------------------------------------------------------------------------------

} //namespace vl