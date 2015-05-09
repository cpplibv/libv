// File: Trackable.hpp, Created on 2014. május 28. 18:45, Author: Vader

#pragma once

#include <map>
#include "thread_policy.hpp"

namespace vl {

struct TrackableBase {
	virtual void connect(TrackableBase* ptr, bool reflect) = 0;
	virtual void disconnect(TrackableBase* ptr, bool reflect) = 0;
	virtual ~TrackableBase() { }
};

template <typename ThreadPolicy = SingleThread>
class Trackable : public TrackableBase {
private:
	std::map<TrackableBase*, int> connections;
	ThreadPolicy threadPolicy;
private:
	inline void connect(TrackableBase* ptr, bool reflect = true) {
		std::lock_guard<ThreadPolicy> thread_guard(threadPolicy);
		++connections[ptr];

		if (reflect)
			ptr->connect(this, false);
	}
	inline void disconnect(TrackableBase* ptr, bool reflect = true) {
		std::lock_guard<ThreadPolicy> thread_guard(threadPolicy);
		if (!--connections[ptr])
			connections.erase(ptr);

		if (reflect)
			ptr->disconnect(this, false);
	}
protected:
	inline void disconnectAll() {
		std::lock_guard<ThreadPolicy> thread_guard(threadPolicy);
		while (!connections.empty())
			this->disconnect(connections.begin()->first);
	}
	virtual ~Trackable() {
		disconnectAll();
	}
};

} //namespace vl

//namespace vl {
//
//	struct TrackingConnection;
//
//	struct TrackableBase {
//		virtual void connect(TrackableBase* ptr) = 0;
//		virtual void disconnect(TrackableBase* ptr) = 0;
//		virtual void connect(TrackingConnection* ptr) = 0;
//		virtual void disconnect(TrackingConnection* ptr) = 0;
//		virtual ~TrackableBase() { }
//	};
//
//	template<typename ThreadPolicy = SingleThread>
//	struct TrackingConnection {
//		unsigned int con;
//		TrackableBase* endpoint_a;
//		TrackableBase* endpoint_b;
//		ThreadPolicy threadPolicy;
//		TrackingConnection(TrackableBase* endpoint_a, TrackableBase* endpoint_b) :
//			endpoint_a(endpoint_a),
//			endpoint_b(endpoint_b) {
//			endpoint_a->connect(this);
//			endpoint_b->connect(this);
//		}
//		inline void lock() {
//			threadPolicy.lock();
//		}
//		inline void unlock() {
//			threadPolicy.unlock();
//		}
//	};
//
//	template<typename ThreadPolicy = SingleThread>
//	class Trackable : public TrackableBase {
//	private:
////		std::map<TrackableBase*, int> connections;
//		std::set<TrackingConnection> connections;
//	private:
//		inline void connect(TrackableBase* trackable) {
//			TrackingConnection(this, trackable);
//		}
//		inline void disconnect(TrackableBase* ptr, bool reflect = true) {
//			if (!--connections[ptr])
//				connections.erase(ptr);
//
//			if (reflect)
//				ptr->disconnect(this, false);
//		}
//	protected:
//		inline void disconnectAll() {
//			std::lock_guard<ThreadPolicy> thread_guard(threadPolicy);
//			while (!connections.empty())
//				this->disconnect(connections.begin()->first);
//		}
//		~Trackable() {
//			disconnectAll();
//		}
//	};
//} //namespace vl