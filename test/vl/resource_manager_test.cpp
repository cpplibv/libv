// File: ResourceManagerTest.cpp, Created on 2015. február 26. 10:31, Author: Vader

#include <catch.hpp>
#include "vl/resource_manager.hpp"
#include <iostream>

using namespace vl;
template<typename T>
struct TestResourceManager : public ResourceManager<T> {
	using ResourceManager<T>::ResourceManager;
};
struct TestResource {
	int* dtor = nullptr;

	TestResource(const std::string& n = "default") : n(n) { }

	const std::string& getn() const {
		return n;
	}

	~TestResource() {
		if (dtor) {
			++*dtor;
		}
	}

public:

	bool operator<(const TestResource& r) const {
		return n < r.n;
	}

private:
	std::string n;

	friend bool operator<(const std::tuple<std::string>& t, const TestResource& r) {
		return std::get<0>(t) < r.n;
	}

	friend bool operator<(const TestResource& r, const std::tuple<std::string>& t) {
		return r.n < std::get<0>(t);
	}
};
struct TestResource2 {

	TestResource2(const std::string& n = "default", int m = 10) : n(n), m(m) { }
	std::string n;
	int m;

	bool operator<(const TestResource2& r) const {
		if (n != r.n)
			return n < r.n;
		else
			return m < r.m;
	}

private:

	friend bool operator<(const std::tuple<std::string>& t, const TestResource2& r) {
		return std::get<0>(t) < r.n;
	}

	friend bool operator<(const TestResource2& r, const std::tuple<std::string>& t) {
		return r.n < std::get<0>(t);
	}

	friend bool operator<(const std::tuple<std::string, int>& t, const TestResource2& r) {
		if (std::get<0>(t) != r.n)
			return std::get<0>(t) < r.n;
		else
			return std::get<1>(t) < r.m;
	}

	friend bool operator<(const TestResource2& r, const std::tuple<std::string, int>& t) {
		if (r.n != std::get<0>(t))
			return r.n < std::get<0>(t);
		else
			return r.m < std::get<1>(t);
	}
};

// -------------------------------------------------------------------------------------------------

TEST_CASE("RMBasic") {
	ResourceManager<TestResource> rm0;
	Resource<TestResource> tr0(rm0, "x");
	Resource<TestResource> tr1(rm0, "x");

	ResourceManager<TestResource> rm1;
	Resource<TestResource> tr2(rm1, "x");

	CHECK(&tr0.data() == &tr1.data());
	CHECK(&tr0.data() != &tr2.data());
}

TEST_CASE("RMDefault") {
	TestResourceManager<TestResource> rm0;
	Resource<TestResource> tr0(rm0, "x");

	CHECK(&tr0.data() != nullptr);
}

TEST_CASE("RMMultiArg") {
	ResourceManager<TestResource2> rm0;
	Resource<TestResource2> tr0(rm0, "x");
	Resource<TestResource2> tr1(rm0, "x");
	Resource<TestResource2> tr2(rm0, "x", 20);
	Resource<TestResource2> tr3(rm0, "y", 20);
	Resource<TestResource2> tr4(rm0, "y");

	CHECK(&tr0.data() == &tr1.data());
	CHECK(&tr0.data() != &tr2.data());
	CHECK(&tr0.data() != &tr3.data());
	CHECK(&tr0.data() != &tr4.data());

	CHECK(&tr2.data() != &tr3.data());
	CHECK(&tr2.data() != &tr4.data());

	CHECK(&tr3.data() == &tr4.data());
}

TEST_CASE("RMDestruct") {
	int dtor0 = 0;
	{
		TestResourceManager<TestResource> rm0;
		Resource<TestResource> tr0(rm0, "x");
		tr0.data().dtor = &dtor0;
	}
	CHECK(dtor0 == 1);
}

TEST_CASE("RMResourceDestruct") {
	int dtor0 = 0;
	int dtor1 = 0;
	ResourceManager<TestResource> rm0;
	{
		Resource<TestResource> tr0(rm0, "x");
		Resource<TestResource> tr1(rm0, "y");
		Resource<TestResource> tr2(rm0, "y");
		tr0.data().dtor = &dtor0;
		tr1.data().dtor = &dtor1;
	}
	CHECK(dtor0 == 1);
	CHECK(dtor1 == 1);
}

TEST_CASE("RMRMDestruct") {
	int dtor0 = 0;
	int dtor1 = 0;
	{
		ResourceManager<TestResource> rm0;
		Resource<TestResource> tr0(rm0, "x");
		Resource<TestResource> tr1(rm0, "y");
		Resource<TestResource> tr2(rm0, "y");
		tr0.data().dtor = &dtor0;
		tr1.data().dtor = &dtor1;
	}
	CHECK(dtor0 == 1);
	CHECK(dtor1 == 1);
}

TEST_CASE("RMDestructOnLastRef") {
	int dtor0 = 0;
	ResourceManager<TestResource> rm0;
	{
		Resource<TestResource> tr0(rm0, "x");
		tr0.data().dtor = &dtor0;
	}
	{
		Resource<TestResource> tr0(rm0, "x");
		tr0.data().dtor = &dtor0;
	}
	CHECK(dtor0 == 2);
}

TEST_CASE("RMNoDestructOnNonLast") {
	int dtor0 = 0;
	ResourceManager<TestResource> rm0;
	{
		Resource<TestResource> tr0{rm0, "x"};
		tr0.data().dtor = &dtor0;
		{
			Resource<TestResource> tr1{rm0, "x"};
		}
		{
			Resource<TestResource> tr1{rm0, "x"};
		}
		CHECK(dtor0 == 0);
	}
	CHECK(dtor0 == 1);
}

//TEST_CASE("RMNoDestructOnStillRef") {
//	int dtor0 = 0;
//	
//	auto* rm0 = new ResourceManager<TestResource>{};
//	auto* tr0 = new Resource<TestResource>{*rm0, "x"};
//	tr0->data().dtor = &dtor0;
//	
//	CHECK(dtor0 == 0);
//	delete rm0;
//	CHECK(dtor0 == 0);
//	delete tr0;
//	CHECK(dtor0 == 1);
//}

TEST_CASE("RSKeepAlive") {
	int dtor0 = 0;
	{
		ResourceManager<TestResource> rm0;
		{
			ResourceScope<TestResource> rs0(rm0);
			{
				Resource<TestResource> tr0{rs0, "x"};
				tr0.data().dtor = &dtor0;
				CHECK(dtor0 == 0);
			}
			{
				Resource<TestResource> tr0{rs0, "x"};
				tr0.data().dtor = &dtor0;
				CHECK(dtor0 == 0);
			}
			CHECK(dtor0 == 0);
		}
		CHECK(dtor0 == 1);
	}
	CHECK(dtor0 == 1);
}

TEST_CASE("RSRSKeepAlive") {
	int dtor0 = 0;
	int dtor1 = 0;
	{
		ResourceManager<TestResource> rm0;
		{
			ResourceScope<TestResource> rs0(rm0);
			{
				ResourceScope<TestResource> rs1(rs0);
				{
					Resource<TestResource> tr0{rs0, "x"};
					Resource<TestResource> tr1{rs1, "y"};
					tr0.data().dtor = &dtor0;
					tr1.data().dtor = &dtor1;
					CHECK(dtor0 == 0);
					CHECK(dtor1 == 0);
				}
				CHECK(dtor0 == 0);
				CHECK(dtor1 == 0);
			}
			CHECK(dtor0 == 0);
			CHECK(dtor1 == 1);
		}
		CHECK(dtor0 == 1);
		CHECK(dtor1 == 1);
	}
	CHECK(dtor0 == 1);
	CHECK(dtor1 == 1);
}