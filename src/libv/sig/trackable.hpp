// File: Trackable.hpp, Created on 2014. május 28. 18:45, Author: Vader

#pragma once

#include <map>


namespace libv {

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

} // namespace libv
