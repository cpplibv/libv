// File: Trackable.hpp, Created on 2014. május 28. 18:45, Author: Vader

#pragma once

#include <map>
#include <cassert>

#include "thread_policy.hpp"

namespace libv {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <typename ThreadPolicy>
struct ConnectionPoint {
	bool alive = true;
	ThreadPolicy threadPolicy;
};

struct TrackableConnectionBase {
	virtual bool readLock() = 0;
	virtual void readUnlock() = 0;
	virtual bool writeLock() = 0;
	virtual void writeUnlock() = 0;
};

struct TrackableConnectionNoTrackable final : TrackableConnectionBase {
	virtual bool readLock() override {
		return true;
	}
	virtual void readUnlock() override { }
	virtual bool writeLock() override {
		return true;
	}
	virtual void writeUnlock() override { }
};

template <typename TP>
struct TrackableConnectionTrackable final : TrackableConnectionBase {
	std::shared_ptr<ConnectionPoint<TP>> sp;
	TrackableConnectionTrackable(std::shared_ptr<ConnectionPoint<TP>> sp) : sp(std::move(sp)) { }
	virtual bool readLock() override {
		sp->threadPolicy.readLock();
		return sp->alive;
	}
	virtual void readUnlock() override {
		sp->threadPolicy.readUnlock();
	}
	virtual bool writeLock() override {
		sp->threadPolicy.writeLock();
		return sp->alive;
	}
	virtual void writeUnlock() override {
		sp->threadPolicy.writeUnlock();
	}
};

struct TrackableConnectionWeak final : TrackableConnectionBase {
	std::shared_ptr<void> tr_sp;
	std::weak_ptr<void> tr_wp;
	TrackableConnectionWeak(const std::weak_ptr<void>& tr_wp) :
		tr_wp(tr_wp) { }

	virtual bool readLock() override {
		tr_sp = tr_wp.lock();
		return !tr_wp.expired();
	}
	virtual void readUnlock() override {
		tr_sp.reset();
	}
	virtual bool writeLock() override {
		tr_sp = tr_wp.lock();
		return !tr_wp.expired();
	}
	virtual void writeUnlock() override {
		tr_sp.reset();
	}
};
} //namespace detail

// -------------------------------------------------------------------------------------------------

struct TrackableAccessor;

struct TrackableBase {
};

template <typename ThreadPolicy>
class TrackableThread : TrackableBase {
	friend class TrackableAccessor;
	std::shared_ptr<detail::ConnectionPoint<ThreadPolicy>> trackingPoint;
protected:
	void disconnect() {
		auto lock = make_write_lock_guard(trackingPoint->threadPolicy);
		trackingPoint->alive = false;
	}
public:
	TrackableThread() {
		trackingPoint = std::make_shared<detail::ConnectionPoint<ThreadPolicy>>();
	}
	virtual ~TrackableThread() {
		assert((ThreadPolicy::ALLOW_DISCONNECT_ON_DTOR || !trackingPoint->alive) &&
				"Object reached Trackable base class destructor with live tracking point but the "
				"used ThreadPolicy forbids this. In the most derived Object's destructor (before "
				"any signal reached callback method could be invalid) use Trackable's disconnect() "
				"member function or if hidden use TrackableAccessor::disconnect(Trackable&) static "
				"member function to ensure thread safety.");
		if (ThreadPolicy::ALLOW_DISCONNECT_ON_DTOR)
			disconnect();
	}
};

using Trackable = TrackableThread<SingleThread>;

struct TrackableAccessor {
	template <typename ThreadPolicy>
	static inline std::shared_ptr<detail::ConnectionPoint<ThreadPolicy>> getTrackingPoint(TrackableThread<ThreadPolicy>& t) {
		return t.trackingPoint;
	}
	template <typename ThreadPolicy>
	static inline void disconnect(TrackableThread<ThreadPolicy>& t) {
		return t.disconnect();
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace libv
