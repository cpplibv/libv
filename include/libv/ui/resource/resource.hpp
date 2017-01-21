// File:   resource.hpp Author: Vader Created on 2016. szeptember 9., 17:49

#pragma once

// cfg
#include <libv/ui/config.hpp>
//// ext
//#include <boost/container/small_vector.hpp>
// libv
#include <libv/memory.hpp>
//#include <libv/utility.hpp>
//#include <libv/worker_thread.hpp>
// std
#include <atomic>
//#include <map>
// pro
#include <libv/ui/resource/resource_fwd.hpp>

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//class Context;
//
//template <typename T>
//struct ResourceWrapper;

// -------------------------------------------------------------------------------------------------

//struct ResourceBase {
////	template <typename T>
////	friend class ResourceWrapper;
////private:
////	mutable std::atomic<uint64_t> use_count_{0};
////
////public:
////	inline uint64_t use_count() const {
////		return use_count_;
////	}
////	virtual ~ResourceBase() { }
//};

template <typename T>
struct Resource {
protected:
	struct Step {
		ResourceStepResult(T::*func)();
		ThreadAffinity affinity;
	};
};

//template <typename T>
//struct ResourceWrapper {
//	observer_ptr<Context> context;
//	T resource;
//
//public:
//	template <typename... Args>
//	ResourceWrapper(observer_ptr<Context> context, Args&&... args) :
//		context(context),
//		resource(std::forward<Args>(args)...) { }
////	T& operator*() {
////		return resource;
////	}
////	T* operator->() {
////		return &resource;
////	}
////	const T& operator*() const {
////		return resource;
////	}
////	const T* operator->() const {
////		return &resource;
////	}
////	void add_ref() {
////		++resource.use_count_;
////	}
////	template <typename = void>
////	void release() {
////		if (--resource.use_count_)
////			context->resource->realese(ResourceView<T>(this));
////	}
//
//	virtual ~ResourceWrapper() {
//		move
//		context->release();
//	}
//};



//	template <typename T>
//	void Resource<T>::loadingSequence(size_t i) {
//		if (i >= count_of(T::ResourceLoadSequence))
//			return;
//
//		auto& step = T::ResourceLoadSequence[i];
//
//		if (ThreadAffinity::IN) {
//			const auto& step = T::ResourceLoadSequence[i];
//			static_cast<T*>(this)->*(step.func)();
//			loadingSequence(i + 1);
//			return;
//		}
//
//		switch (step.affinity) {
//		default:
//		case ThreadAffinity::ANY: executor = context.executer.any;
//			break;
//		case ThreadAffinity::GL: executor = context.executer.gl;
//			break;
//		case ThreadAffinity::IO: executor = context.executer.io;
//			break;
//		}
//
//		executor->executeAsync([this, ptr, i] {
//			const auto& step = T::ResourceLoadSequence[i];
//			static_cast<T*> (this)->*(step.func)();
//			loadingSequence(i + 1);
//		}, step.priority);
//	}
//	template <typename T>
//	void Resource<T>::unloadingSequence(size_t i) {
//		if (i >= count_of(T::ResourceUnloadSequence))
//			notifyDependents();
//
//		auto& step = T::ResourceUnloadSequence[i];
//
//		if (ThreadAffinity::IN) {
//			const auto& step = T::ResourceUnloadSequence[i];
//			static_cast<T*> (this)->*(step.func)();
//			unloadingSequence(i + 1);
//			return;
//		}
//
//		observer_ptr<WorkerThread> executor;
//
//		switch (step.affinity) {
//		default:
//		case ThreadAffinity::ANY: executor = context.executer.any;
//			break;
//		case ThreadAffinity::GL: executor = context.executer.gl;
//			break;
//		case ThreadAffinity::IO: executor = context.executer.io;
//			break;
//		}
//
//		executor->executeAsync([this, i] {
//			const auto& step = T::ResourceUnloadSequence[i];
//			static_cast<T*> (this)->*(step.func)();
//			unloadingSequence(i + 1);
//		}, step.priority);
//	}

//using ResourceID = uint64_t;
//
//// -------------------------------------------------------------------------------------------------
//
//struct DependencyManager {
//	std::unordered_map<ResourceID, boost::container::small_vector<ResourceID, 5 >> dependencyMap;
//	void addDep(ResourceID resource, ResourceID dependency) {
//		dependencyMap[dependency].emplace_back(resource);
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename T>
//struct Resource {
//	friend class ResourceReference<T>;
//private:
//	mutable std::atomic<uint64_t> use_count_{0};
//	//	const ResourceID id{0};
//	//protected:
//	//	std::atomic<observer_ptr<ResourceLoader<T>>> loader;
//	//	observer_ptr<ResourceLoader<T>> loader;
//private:
//	void loadingSequence(size_t i = 0);
//	void unloadingSequence(size_t i = 0);
//	void notifyDependents();
//public:
//	uint64_t use_count() const {
//		return use_count_;
//	}
//	~Resource() {
//		LIBV_UI_DEBUG_ASSERT(use_count_ == 0 && "Resource destroyed before every corresponding proxy.");
//	}
//};
//template <typename T>
//void Resource<T>::loadingSequence(size_t i) {
//	if (i >= count_of(T::ResourceLoadSequence))
//		notifyDependents();
//
//	auto& step = T::ResourceLoadSequence[i];
//
//	if (ThreadAffinity::IN) {
//		const auto& step = T::ResourceLoadSequence[i];
//		static_cast<T*> (this)->*(step.func)();
//		loadingSequence(i + 1);
//		return;
//	}
//
//	observer_ptr<WorkerThread> executor;
//
//	switch (step.affinity) {
//	default:
//	case ThreadAffinity::ANY: executor = context.executer.any;
//		break;
//	case ThreadAffinity::GL: executor = context.executer.gl;
//		break;
//	case ThreadAffinity::IO: executor = context.executer.io;
//		break;
//	}
//
//	executor->executeAsync([this, ptr, i] {
//		const auto& step = T::ResourceLoadSequence[i];
//		static_cast<T*> (this)->*(step.func)();
//		loadingSequence(i + 1);
//	}, step.priority);
//}
//template <typename T>
//void Resource<T>::unloadingSequence(size_t i) {
//	if (i >= count_of(T::ResourceUnloadSequence))
//		notifyDependents();
//
//	auto& step = T::ResourceUnloadSequence[i];
//
//	if (ThreadAffinity::IN) {
//		const auto& step = T::ResourceUnloadSequence[i];
//		static_cast<T*> (this)->*(step.func)();
//		unloadingSequence(i + 1);
//		return;
//	}
//
//	observer_ptr<WorkerThread> executor;
//
//	switch (step.affinity) {
//	default:
//	case ThreadAffinity::ANY: executor = context.executer.any;
//		break;
//	case ThreadAffinity::GL: executor = context.executer.gl;
//		break;
//	case ThreadAffinity::IO: executor = context.executer.io;
//		break;
//	}
//
//	executor->executeAsync([this, i] {
//		const auto& step = T::ResourceUnloadSequence[i];
//		static_cast<T*> (this)->*(step.func)();
//		unloadingSequence(i + 1);
//	}, step.priority);
//}
//template <typename T>
//void Resource<T>::notifyDependents() { }
//
////template <typename T>
////struct ResourceLoader {
////	ResourceReference<T> ptr;
////	//	struct Callback {
////	//		std::function<void(const ResourceReference<T>&) > func;
////	//		ResourceReference<T> ref;
////	//	};
////	//
////	//	uint32_t dependencyCount = 0;
////	//	uint32_t dependencyResults[ResourceStatus::_LAST];
////	//	std::vector<Callback> callbacks;
////	//	uint32_t loadStep = 0;
////	//	uint32_t unloadStep = 0;
////	//
////	//	template <typename K>
////	//	void dependency(Resource<K>& dep) {
////	//		++dependencyCount;
////	//		//		dep.loader->callback();
////	//		dep.loader->callbacks.emplace_back(ptr, [this](ResourceStatus s) {
////	//			++dependencyResults[s];
////	//			--dependencyCount;
////	//			if (dependencyCount == 0) {
////	//				loadingSequence();
////	//				unloadingSequence();
////	//			}
////	//		});
////	//	}
////};
//
////template <typename T>
////struct ResourceReference {
////	observer_ptr<Resource<T>> ptr;
////	std::function<void(observer_ptr<Resource<T>>, ResourceState) > callback;
////	void loadCallback() { }
////	ResourceReference(observer_ptr<Resource<T>> ptr) : ptr(ptr) {
////		++ptr->use_count_;
////	}
////	~ResourceReference() {
////		--ptr->use_count_;
////		ptr->loader->forget(this);
////	}
////};
//
//template <typename K, typename T>
//struct ResourceProvider {
//	std::map<K, ResourceView<T>> cache;
//private:
//	std::vector<ResourceView<T>> resourceLoadQue;
//	std::vector<ResourceView<T>> resourceUnloadQue;
//public:
//	ResourceView<T> get();
//
//};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
