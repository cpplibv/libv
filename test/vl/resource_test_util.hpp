// File:   resource_test_util.hpp

#pragma once

#include <vl/resource.hpp>

namespace vl {

// -------------------------------------------------------------------------------------------------

struct DataIO {
	int n;
	DataIO(int n) :
		n(n) { }

	bool ready = false;
	void load(std::shared_ptr<DataIO>) {
		std::cout << n << " IO load" << std::endl;
		ready = true;
	}
	void unload(std::shared_ptr<DataIO>) {
		std::cout << n << " IO unload" << std::endl;
		ready = false;
	}
	bool operator<(const DataIO& r) const;
	friend bool operator<(int t, const DataIO& r);
	friend bool operator<(const DataIO& r, int t);
};
bool DataIO::operator<(const DataIO& r) const {
	return n < r.n;
}
bool operator<(int t, const DataIO& r) {
	return t < r.n;
}
bool operator<(const DataIO& r, int t) {
	return r.n < t;
}

// -------------------------------------------------------------------------------------------------

struct DataGL {
	int n;
	DataGL(int n) :
		n(n) { }

	bool ready = false;
	void load(std::shared_ptr<DataGL>) {
		std::cout << n << " GL load" << std::endl;
		ready = true;
	}
	void unload(std::shared_ptr<DataGL>) {
		std::cout << n << " GL unload" << std::endl;
		ready = false;
	}
	bool operator<(const DataGL& r) const;
	friend bool operator<(int t, const DataGL& r);
	friend bool operator<(const DataGL& r, int t);
};
bool DataGL::operator<(const DataGL& r) const {
	return n < r.n;
}
bool operator<(int t, const DataGL& r) {
	return t < r.n;
}
bool operator<(const DataGL& r, int t) {
	return r.n < t;
}

struct ResourceManager : ContextHost {
	std::shared_ptr<vl::WorkerThread> any;
	std::shared_ptr<vl::WorkerThread> gl;
	std::shared_ptr<vl::WorkerThread> io;
	std::shared_ptr<vl::WorkerThread> net;

	vl::LoaderCache<DataIO> cacheIO;
	vl::LoaderCache<DataGL> cacheGL;
	ResourceManager() :
		any(std::make_shared<vl::WorkerThread>("ANY")),
		gl(std::make_shared<vl::WorkerThread>("GL")),
		io(std::make_shared<vl::WorkerThread>("IO")),
		net(std::make_shared<vl::WorkerThread>("NET")) { }
	virtual std::shared_ptr<vl::WorkerThread> get(vl::ContextID cid) override {
		switch (cid) {
		default:
		case vl::ContextID::ANY: return any;
			break;
		case vl::ContextID::GL: return gl;
			break;
		case vl::ContextID::IO: return io;
			break;
		case vl::ContextID::NET: return net;
			break;
		}
	}
};
using ResourceManagerPtr = std::shared_ptr<ResourceManager>;

// -------------------------------------------------------------------------------------------------

struct Resource {
	int n;
	std::shared_ptr<ContextHost> rm;

	std::shared_ptr<DataIO> dataIO;
	std::shared_ptr<DataGL> dataGL;
	Resource(ResourceManagerPtr rm, int n) :
		n(n),
		rm(rm) {
		dataIO = rm->cacheIO.get(n);
		dataGL = rm->cacheGL.get(n);
	}
	void loadIO() { }
	void loadGL() { }
	void unloadIO() { }
	void unloadGL() { }
	void load(std::shared_ptr<Resource> self) {
		vl::FunctionSequence ls(rm);
		ls.addStepIO(&Resource::loadIO, self);
		ls.addStepGL(&Resource::loadGL, self);
		ls.execute();
	}
	void unload(std::shared_ptr<Resource> self) {
		vl::FunctionSequence ls(rm);
		ls.addStepIO(&Resource::unloadGL, self);
		ls.addStepGL(&Resource::unloadIO, self);
		ls.execute();
	}
	bool operator<(const Resource& r) const {
		return n < r.n;
	}
	friend bool operator<(const std::tuple<ResourceManagerPtr, int>& t, const Resource& r) {
		return std::get<1>(t) < r.n;
	}
	friend bool operator<(const Resource& r, const std::tuple<ResourceManagerPtr, int>& t) {
		return r.n < std::get<1>(t);
	}
};

// -------------------------------------------------------------------------------------------------

struct TestResource {
	int n;
	TestResource(int n) : n(n) { }
	bool operator<(const TestResource& r) const {
		return n < r.n;
	}
	friend bool operator<(int t, const TestResource& r) {
		return t < r.n;
	}
	friend bool operator<(const TestResource& r, int t) {
		return r.n < t;
	}
};

// -------------------------------------------------------------------------------------------------

struct LoadableTestResource {
	int n;
	int loaded = 0;
	int* unloaded = nullptr;
	LoadableTestResource(int n) : n(n) { }
	void load(std::shared_ptr<LoadableTestResource>) {
		++loaded;
	}
	void unload(std::shared_ptr<LoadableTestResource>) {
		if (unloaded)
			++(*unloaded);
	}
	bool operator<(const LoadableTestResource& r) const {
		return n < r.n;
	}
	friend bool operator<(int t, const LoadableTestResource& r) {
		return t < r.n;
	}
	friend bool operator<(const LoadableTestResource& r, int t) {
		return r.n < t;
	}
};

// -------------------------------------------------------------------------------------------------

struct LoadableTestResourceB {
	int n;
	int argCount;

	template<typename... Args>
	LoadableTestResourceB(int n, Args...) : n(n), argCount(sizeof...(Args) + 1) { }
	void load(std::shared_ptr<LoadableTestResourceB>) { }
	void unload(std::shared_ptr<LoadableTestResourceB>) { }
	bool operator<(const LoadableTestResourceB& r) const {
		return n < r.n;
	}
	friend bool operator<(int t, const LoadableTestResourceB& r) {
		return t < r.n;
	}
	friend bool operator<(const LoadableTestResourceB& r, int t) {
		return r.n < t;
	}
};

} //namespace vl