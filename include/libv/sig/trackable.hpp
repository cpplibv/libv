// File: Trackable.hpp, Created on 2014. május 28. 18:45, Author: Vader

#pragma once

#include <map>
//#include <boost/container/small_vector.hpp>

//#ifndef LIBV_SIG_TRACKABLE_SMALL_VECTOR_SIZE
//#    define LIBV_SIG_TRACKABLE_SMALL_VECTOR_SIZE 4
//#endif

namespace libv {

// -------------------------------------------------------------------------------------------------

struct TrackableBase {
	virtual void connect(TrackableBase* ptr, bool reflect) = 0;
	virtual void disconnect(TrackableBase* ptr, bool reflect) = 0;
	virtual ~TrackableBase() { }
};

class Trackable : public TrackableBase {
private:
	std::map<TrackableBase*, int> connections;
	std::recursive_mutex mutex;

private:
	inline void connect(TrackableBase* ptr, bool reflect = false) override {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		++connections[ptr];

		if (reflect)
			ptr->connect(this, false);
	}
	inline void disconnect(TrackableBase* ptr, bool reflect = false) override {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		if (!--connections[ptr])
			connections.erase(ptr);

		if (reflect)
			ptr->disconnect(this, false);
	}
protected:
	inline void disconnectAll() {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		while (!connections.empty())
			this->disconnect(connections.begin()->first, true);
	}
	virtual ~Trackable() {
		disconnectAll();
	}
public:
	Trackable() { };
	Trackable(const Trackable& other) = delete;
};

} //namespace libv

//namespace libv {
//
//struct TrackableBase {
//	virtual void connect(TrackableBase* ptr) = 0;
//	virtual void disconnect(TrackableBase* ptr) = 0;
//	virtual void disconnectAll() = 0;
//	//	virtual void connect(TrackableBase* ptr, bool reflect) = 0;
//	//	virtual void disconnect(TrackableBase* ptr, bool reflect) = 0;
//	//	virtual void lock() = 0;
//	//	virtual void unlock() = 0;
//	virtual ~TrackableBase() { }
//};
//
////template <typename ThreadPolicy = SingleThread>
////struct TrackingConnection {
////	TrackableBase* a;
////	TrackableBase* b;
////	TrackingConnection(TrackableBase* lhs, TrackableBase* rhs) :
////		a(lhs < rhs ? lhs : rhs),
////		b(lhs < rhs ? rhs : lhs) { }
////	bool operator<(const TrackingConnection<ThreadPolicy>& rhs) const {
////		return a < rhs.a || a == rhs.a && b < rhs.b;
////	}
////};
//
//template <typename ThreadPolicy = SingleThread>
//class Trackable : public TrackableBase {
//private:
//	using Container =
//			boost::container::small_vector<TrackableBase*, LIBV_SIG_TRACKABLE_SMALL_VECTOR_SIZE>;
//private:
//	//	std::map<TrackableBase*, int> connections;
//	Container connections;
//	//	ThreadPolicy threadPolicy;
//private:
//	inline void connect(TrackableBase* ptr) {
//		//		TrackingConnection conn(ptr, this);
//		auto it = std::find(std::begin(connections), std::end(connections), ptr);
//		if(it != std::end(connections))
//			return true;
//		//		std::lock_guard<ThreadPolicy> thread_guard(threadPolicy);
//		//		++connections[ptr];
//
//		//		if (reflect)
//		//			ptr->connect(this, false);
//	}
//	inline void disconnect(TrackableBase* ptr) {
//		//		std::lock_guard<ThreadPolicy> thread_guard(threadPolicy);
//		//		if (!--connections[ptr])
//		//			connections.erase(ptr);
//		//
//		//		if (reflect)
//		//			ptr->disconnect(this, false);
//	}
//protected:
//	inline void disconnectAll() {
//		//		std::lock_guard<ThreadPolicy> thread_guard(threadPolicy);
//		//		while (!connections.empty())
//		//			this->disconnect(connections.begin()->first);
//	}
//	virtual ~Trackable() {
//		disconnectAll();
//	}
//};
//
//} //namespace libv