// Project: libv.utility, File: test/libv/utility/test_entity_ptr.cpp

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/utility/entity/entity_ptr.hpp>




#include <libv/utility/hex_dump.hpp>
#include <iostream>

// -------------------------------------------------------------------------------------------------

struct Entity {
	uint32_t ref_count = 0;
	bool is_dead();
};

struct Fleet : Entity {
	uint32_t id;
	std::string name;

	Fleet(uint32_t id, const std::string& name) : id(id), name(name) {}
};

// =================================================================================================

struct FleetFwd4;

struct TestClassFwd4 {
	int ctx;
	libv::entity_store<FleetFwd4> arena;

	TestClassFwd4();
	~TestClassFwd4();
};

TEST_CASE("entity_store usage with fwd declared type without definition", "[libv.utility.entity_ptr]") {
	[[maybe_unused]] TestClassFwd4* ignore = nullptr;

//	 TestClassFwd4 x; // would compile, but fail to link
}

struct FleetFwd5;

struct TestClassFwd5 {
	int ctx = 42;
	libv::entity_store<FleetFwd5> arena;

	TestClassFwd5();
	~TestClassFwd5();
};

struct FleetFwd5 {
};

TestClassFwd5::TestClassFwd5() : arena(5000) {}
TestClassFwd5::~TestClassFwd5() = default;

TEST_CASE("entity_store usage with fwd declared type with definition", "[libv.utility.entity_ptr]") {
	[[maybe_unused]] FleetFwd5 ignore;
}

TEST_CASE("entity_store ctor", "[libv.utility.entity_ptr]") {
	libv::entity_store<Fleet> fleets(2);
	CHECK(fleets.size() == 0);
	CHECK(fleets.capacity() == 1637);
}

TEST_CASE("entity_store ctor create entity_ptr", "[libv.utility.entity_ptr]") {
	libv::entity_store<Fleet> fleets(2);
	CHECK(fleets.size() == 0);

	{
		auto fleet = fleets.create(0, "Long long long long long long long long long fleet name 0");

		CHECK(fleets.size() == 1);
		CHECK(fleet->ref_count == 1);
	}

	CHECK(fleets.size() == 0);
}

TEST_CASE("entity_ptr ctor op= and ref_count", "[libv.utility.entity_ptr]") {
	libv::entity_store<Fleet> fleets(2);
	CHECK(fleets.size() == 0);

	{
		const auto fleetA0 = fleets.create(0, "Long long long long long long long long long fleet name A");

		CHECK(fleets.size() == 1);

		{
			// Copy ctor
			CHECK(fleetA0->ref_count == 1);
			auto fleetA1 = fleetA0;
			CHECK(fleets.size() == 1);

			CHECK(fleetA1 == fleetA0);
			CHECK(fleetA0->ref_count == 2);
		}
		{
			// Move ctor
			CHECK(fleetA0->ref_count == 1);
			auto fleetA1 = fleetA0;
			auto fleetA2 = std::move(fleetA1);
			CHECK(fleets.size() == 1);

			CHECK(fleetA1 == nullptr);
			CHECK(fleetA2 == fleetA0);
			CHECK(fleetA0->ref_count == 2);
		}
		{
			// Copy assign
			CHECK(fleetA0->ref_count == 1);
			auto fleetA1 = fleetA0;
			auto fleetA2 = fleetA1;
			fleetA1 = fleetA2;
			CHECK(fleets.size() == 1);

			CHECK(fleetA1 == fleetA0);
			CHECK(fleetA2 == fleetA0);
			CHECK(fleetA0->ref_count == 3);
		}
		{
			// Move assign
			CHECK(fleetA0->ref_count == 1);
			auto fleetA1 = fleetA0;
			auto fleetA2 = fleetA1;
			fleetA1 = std::move(fleetA2);
			CHECK(fleets.size() == 1);

			CHECK(fleetA1 == fleetA0);
			CHECK(fleetA2 == nullptr);
			CHECK(fleetA0->ref_count == 2);
		}
	}

	CHECK(fleets.size() == 0);
}

TEST_CASE("entity_store capacity growth", "[libv.utility.entity_ptr]") {
	libv::entity_store<Fleet> fleets(2);

	const auto block_capacity = 1637;

	CHECK(fleets.size() == 0);
	CHECK(fleets.capacity() == block_capacity);

	{
		std::vector<libv::entity_ptr<Fleet>> bucket;
		for (auto i = 0; i < block_capacity; ++i)
			bucket.emplace_back(fleets.create(i, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity);
		CHECK(fleets.capacity() == block_capacity);

		bucket.emplace_back(fleets.create(42, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity + 1);
		CHECK(fleets.capacity() == block_capacity * 2);

		for (auto i = 0; i < block_capacity - 1; ++i)
			bucket.emplace_back(fleets.create(i, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 2);
		CHECK(fleets.capacity() == block_capacity * 2);
	}

	CHECK(fleets.size() == 0);
	CHECK(fleets.capacity() == block_capacity * 2);

	{
		std::vector<libv::entity_ptr<Fleet>> bucket;
		for (auto i = 0; i < block_capacity * 2; ++i)
			bucket.emplace_back(fleets.create(i, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 2);
		CHECK(fleets.capacity() == block_capacity * 2);

		bucket.emplace_back(fleets.create(block_capacity * 2, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 2 + 1);
		CHECK(fleets.capacity() == block_capacity * 3);

		for (auto i = block_capacity * 2 + 1; i < block_capacity * 3; ++i)
			bucket.emplace_back(fleets.create(i, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 3);
		CHECK(fleets.capacity() == block_capacity * 3);

		bucket.emplace_back(fleets.create(block_capacity * 3, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 3 + 1);
		CHECK(fleets.capacity() == block_capacity * 5);
	}

	CHECK(fleets.size() == 0);
}
