// File:   resource_manager.hpp

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// std
#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace vl {

// -------------------------------------------------------------------------------------------------

//TODO P2: ResourceScope: The real ownership will be at the scope and only observer_ptr-s in
//		resources? Think about it...
//TODO P4: Generalization of a ResourceProxy
//IDEA: After vsig is usable in some atomic form, use it for this callback.
//IDEA: Is there a possibilty for lock freeing this?

enum class ResourceState {
	READY,
	UNREADY,
	FAILED
};

class Resource {
private:
	ResourceState state{ResourceState::UNREADY};
	mutable std::recursive_mutex mutex;
	std::vector<Resource*> dependents;
	boost::container::flat_map<Resource*, ResourceState> dependencies;
	std::function<void()> callback;
protected:
	void addDependency(const std::shared_ptr<Resource>& dependency) {
		if (!dependency)
			return;

		std::unique_lock < decltype(mutex) > dependency_lock(dependency->mutex);
		std::lock_guard < decltype(mutex) > lock(mutex);
		dependency->dependents.emplace_back(this);
		dependencies.emplace(dependency.get(), dependency->state);
		dependency_lock.unlock();

		if (callback)
			callback();
	}
	void setDependencyCallback(std::function<void() > callback) {
		std::lock_guard < decltype(mutex) > lock(mutex);
		this->callback = callback;
		if (callback)
			callback();
	}
	void changeResourceState(ResourceState state) {
		std::lock_guard < decltype(mutex) > lock(mutex);
		this->state = state;

		for (const auto& dependent : dependents) {
			std::lock_guard < decltype(mutex) > lock(dependent->mutex);
			dependent->dependencies[this] = state;
			if (dependent->callback)
				dependent->callback();
		}
	}
	bool isEveryDependency(const ResourceState state) const {
		std::lock_guard < decltype(mutex) > lock(mutex);
		for (const auto& dependency : dependencies) {
			if (dependency.second != state)
				return false;
		}
		return true;
	}
	bool isAnyDependency(const ResourceState state) const {
		std::lock_guard < decltype(mutex) > lock(mutex);
		for (const auto& dependency : dependencies) {
			if (dependency.second == state)
				return true;
		}
		return false;
	}
	ResourceState getResourceState() const {
		std::lock_guard < decltype(mutex) > lock(mutex);
		return state;
	}
	virtual ~Resource() {
		{
			std::lock_guard < decltype(mutex) > lock(mutex);
			callback = nullptr;
		}

		for (const auto& dependency : dependencies) {
			std::lock_guard<decltype(mutex)> dependency_lock(dependency.first->mutex);
			auto& vec = dependency.first->dependents;
			vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
		}
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace vl