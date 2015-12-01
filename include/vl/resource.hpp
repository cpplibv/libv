// File:   resource_manager.hpp

#pragma once

// std
#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace vl {

// -------------------------------------------------------------------------------------------------

enum class ResourceState {
	READY,
	UNREADY,
	FAILED
};

//TODO P0: This Resource is bad. Contains multiple data race. I MUST rewrite it! I leave it here for
//		now, but there will be segfaults...
//TODO P2: ResourceScope: The real ownership will be at the scope and only observer_ptr-s in
//		resources? Think about it...
//TODO P3: Make the communication one direction only (Resource -> Dependent)
//TODO P3: This Resource is awful slow... lock free it!
//TODO P4: Generalization of a ResourceProxy
//TODO P5: After vsig is usable in some atomic form, use it for this callback.

class Resource {
private:
	ResourceState state{ResourceState::UNREADY};
	mutable std::recursive_mutex mutex;
	std::vector<Resource*> dependents;
	std::vector<Resource*> dependencies;
	std::function<void() > callback;
protected:
	void addDependency(const std::shared_ptr<Resource>& r) {
		if (!r)
			return;

		std::unique_lock < decltype(mutex) > lockdep(r->mutex);
		std::lock_guard < decltype(mutex) > lock(mutex);
		r->dependents.emplace_back(this);
		dependencies.emplace_back(r.get());
		lockdep.unlock();

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
			if (dependent->callback)
				dependent->callback();
		}
	}
	bool isEveryDependency(const ResourceState state) const {
		decltype(dependencies) tmpDependencies;
		{
			std::lock_guard < decltype(mutex) > lock(mutex);
			tmpDependencies = dependencies;
		}
		// Race Cond

		for (const auto& dependency : tmpDependencies) {
			std::lock_guard < decltype(mutex) > lock(dependency->mutex);
			if (state != dependency->state)
				return false;
		}
		return true;
	}
	bool isAnyDependency(const ResourceState state) const {
		decltype(dependencies) tmpDependencies;
		{
			std::lock_guard < decltype(mutex) > lock(mutex);
			tmpDependencies = dependencies;
		}
		// Race Cond

		for (const auto& dependency : tmpDependencies) {
			std::lock_guard < decltype(mutex) > lock(dependency->mutex);
			if (state == dependency->state)
				return true;
		}
		return false;
	}
	ResourceState getResourceState() const {
		std::lock_guard < decltype(mutex) > lock(mutex);
		return state;
	}
	virtual ~Resource() {
		decltype(dependencies) tmpDependencies;
		{
			std::lock_guard < decltype(mutex) > lock(mutex);
			tmpDependencies = dependencies;
		}
		// Race Cond

		for (const auto& dependency : tmpDependencies) {
			std::lock_guard < decltype(mutex) > lock(dependency->mutex);
			auto& vec = dependency->dependents;
			vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
		}
		// Seg fault
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace vl