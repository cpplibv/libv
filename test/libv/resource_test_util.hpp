// File:   resource_test_util.hpp

#pragma once

#include <libv/resource.hpp>
#include <libv/cache.hpp>
#include <libv/worker_thread.hpp>

// -------------------------------------------------------------------------------------------------

struct DataIO : libv::Resource {
	int n;
	DataIO(int n) :
		n(n) { }

	bool ready = false;
	void load(std::shared_ptr<DataIO>) {
		changeResourceState(libv::ResourceState::READY);
	}
	void unload(std::shared_ptr<DataIO>) {
		changeResourceState(libv::ResourceState::UNREADY);
	}
	bool operator<(const DataIO& r) const {
		return n < r.n;
	}
	friend bool operator<(int t, const DataIO& r) {
		return t < r.n;
	}
	friend bool operator<(const DataIO& r, int t) {
		return r.n < t;
	}
};

// -------------------------------------------------------------------------------------------------

struct DataGL : libv::Resource {
	int n;
	DataGL(int n) :
		n(n) { }
	void load(std::shared_ptr<DataGL>) {
		changeResourceState(libv::ResourceState::READY);
	}
	void unload(std::shared_ptr<DataGL>) {
		changeResourceState(libv::ResourceState::UNREADY);
	}
	bool operator<(const DataGL& r) const {
		return n < r.n;
	}
	friend bool operator<(int t, const DataGL& r) {
		return t < r.n;
	}
	friend bool operator<(const DataGL& r, int t) {
		return r.n < t;
	}
};

// -------------------------------------------------------------------------------------------------

struct ResourceService {
	libv::LoaderCache<DataIO> cacheDataIO;
	libv::LoaderCache<DataGL> cacheDataGL;
};

// -------------------------------------------------------------------------------------------------

struct TestResource : libv::Resource {
	int n;
	volatile bool depsLoaded = false;
	ResourceService* rm;

	std::shared_ptr<DataIO> dataIO;
	std::shared_ptr<DataGL> dataGL;
	TestResource(ResourceService* rm, int n) :
		n(n),
		rm(rm) {
		dataIO = rm->cacheDataIO.get(n);
		dataGL = rm->cacheDataGL.get(n);
	}
	void load(std::shared_ptr<TestResource>) {
		addDependency(dataIO);
		addDependency(dataGL);

		setDependencyCallback([this] {
			if (!isEveryDependency(libv::ResourceState::READY)) return;
			depsLoaded = true;
		});
	}
	void unload(std::shared_ptr<TestResource>) { }
	bool operator<(const TestResource& r) const {
		return n < r.n;
	}
	friend bool operator<(const std::tuple<int>& t, const TestResource& r) {
		return std::get<0>(t) < r.n;
	}
	friend bool operator<(const TestResource& r, const std::tuple<int>& t) {
		return r.n < std::get<0>(t);
	}
};
