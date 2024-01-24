// Project: libv.utility, File: test/libv/utility/test_entity_ptr.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/utility/entity/entity_ptr.hpp>




#include <libv/utility/hex_dump.hpp>
#include <iostream>

// -------------------------------------------------------------------------------------------------

struct Entity {
	uint32_t ref_count = 0;
	bool is_dead = false;

	void kill() {
		is_dead = true;
	}
};

struct Fleet : Entity {
	uint32_t id;
	std::string name;

	Fleet(uint32_t id, const std::string& name) : id(id), name(name) {}
};

struct Planet {
	uint32_t ref_count = 0;
	bool& kill_was_called;

	uint32_t id;
	std::string name;

	Planet(bool& kill_was_called, uint32_t id, const std::string& name) :
			kill_was_called(kill_was_called),
			id(id),
			name(name) {}

	void kill() {
		kill_was_called = true;
	}
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
	CHECK(fleets.capacity() == 1364);
}

TEST_CASE("entity_store ctor create entity_ptr", "[libv.utility.entity_ptr]") {
	libv::entity_store<Fleet> fleets(2);
	CHECK(fleets.size() == 0);

	{
		auto fleet = fleets.create_secondary(0, "Long long long long long long long long long fleet name 0");

		CHECK(fleets.size() == 1);
		CHECK(fleet->ref_count == 1);
	}

	CHECK(fleets.size() == 0);
}

TEST_CASE("entity_ptr ctor op= and ref_count", "[libv.utility.entity_ptr]") {
	libv::entity_store<Fleet> fleets(2);
	CHECK(fleets.size() == 0);

	{
		const auto fleetA0 = fleets.create_secondary(0, "Long long long long long long long long long fleet name A");

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

	const auto block_capacity = 1364;

	CHECK(fleets.size() == 0);
	CHECK(fleets.capacity() == block_capacity);

	{
		std::vector<libv::entity_ptr<Fleet>> bucket;
		for (auto i = 0; i < block_capacity; ++i)
			bucket.emplace_back(fleets.create_secondary(i, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity);
		CHECK(fleets.capacity() == block_capacity);

		bucket.emplace_back(fleets.create_secondary(42, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity + 1);
		CHECK(fleets.capacity() == block_capacity * 2);

		for (auto i = 0; i < block_capacity - 1; ++i)
			bucket.emplace_back(fleets.create_secondary(i, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 2);
		CHECK(fleets.capacity() == block_capacity * 2);
	}

	CHECK(fleets.size() == 0);
	CHECK(fleets.capacity() == block_capacity * 2);

	{
		std::vector<libv::entity_ptr<Fleet>> bucket;
		for (auto i = 0; i < block_capacity * 2; ++i)
			bucket.emplace_back(fleets.create_secondary(i, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 2);
		CHECK(fleets.capacity() == block_capacity * 2);

		bucket.emplace_back(fleets.create_secondary(block_capacity * 2, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 2 + 1);
		CHECK(fleets.capacity() == block_capacity * 3);

		for (auto i = block_capacity * 2 + 1; i < block_capacity * 3; ++i)
			bucket.emplace_back(fleets.create_secondary(i, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 3);
		CHECK(fleets.capacity() == block_capacity * 3);

		bucket.emplace_back(fleets.create_secondary(block_capacity * 3, "Long long long long long long long long long fleet name A"));

		CHECK(fleets.size() == block_capacity * 3 + 1);
		CHECK(fleets.capacity() == block_capacity * 5);
	}

	CHECK(fleets.size() == 0);
}

TEST_CASE("entity_store move", "[libv.utility.entity_ptr]") {
	libv::entity_store<Fleet> original(2);

	const auto block_capacity = 1364;

	CHECK(original.size() == 0);
	CHECK(original.capacity() == block_capacity);

	{
		std::vector<libv::entity_ptr<Fleet>> bucket;
		for (auto i = 0; i < block_capacity; ++i)
			bucket.emplace_back(original.create_secondary(i, "Long long long long long long long long long fleet name A"));

		CHECK(original.size() == block_capacity);
		CHECK(original.capacity() == block_capacity);

		bucket.emplace_back(original.create_secondary(42, "Long long long long long long long long long fleet name A"));

		CHECK(original.size() == block_capacity + 1);
		CHECK(original.capacity() == block_capacity * 2);

		for (const auto& fleet : bucket)
			CHECK(&original == libv::entity_store<Fleet>::store_from_pointer(fleet));

		// Move the store
		auto moved = std::move(original);

		CHECK(original.size() == 0);
		CHECK(original.capacity() == 0);

		CHECK(moved.size() == block_capacity + 1);
		CHECK(moved.capacity() == block_capacity * 2);

		for (const auto& fleet : bucket)
			CHECK(&moved == libv::entity_store<Fleet>::store_from_pointer(fleet));

		// Continue manipulating the moved store
		for (auto i = 0; i < block_capacity - 1; ++i)
			bucket.emplace_back(moved.create_secondary(i, "Long long long long long long long long long fleet name A"));

		for (const auto& fleet : bucket)
			CHECK(&moved == libv::entity_store<Fleet>::store_from_pointer(fleet));

		CHECK(original.size() == 0);
		CHECK(original.capacity() == 0);

		CHECK(moved.size() == block_capacity * 2);
		CHECK(moved.capacity() == block_capacity * 2);

		bucket.clear();

		CHECK(moved.size() == 0);
		CHECK(moved.capacity() == block_capacity * 2);
	}

	CHECK(original.size() == 0);
	CHECK(original.capacity() == 0);

	// Start using a moved from state
	// Hmmm... Lets say its not allowed for now
//	{
//		std::vector<libv::entity_ptr<Fleet>> bucket;
//		for (auto i = 0; i < block_capacity; ++i)
//			bucket.emplace_back(original.create_secondary(i, "Long long long long long long long long long fleet name A"));
//
//		CHECK(original.size() == block_capacity);
//		CHECK(original.capacity() == block_capacity);
//
//		bucket.emplace_back(original.create_secondary(42, "Long long long long long long long long long fleet name A"));
//
//		CHECK(original.size() == block_capacity + 1);
//		CHECK(original.capacity() == block_capacity * 2);
//
//		for (auto i = 0; i < block_capacity - 1; ++i)
//			bucket.emplace_back(original.create_secondary(i, "Long long long long long long long long long fleet name A"));
//
//		CHECK(original.size() == block_capacity * 2);
//		CHECK(original.capacity() == block_capacity * 2);
//
//		for (const auto& fleet : bucket)
//			CHECK(&original == libv::entity_store<Fleet>::store_from_pointer(fleet));
//	}
//
//	CHECK(original.size() == 0);
//	CHECK(original.capacity() == block_capacity * 2);
}

TEST_CASE("primary_entity_ptr", "[libv.utility.entity_ptr]") {
	libv::entity_store<Planet> store(2);

	bool kill_was_called = false;
	libv::primary_entity_ptr<Planet> strong = store.create(kill_was_called, 1, "name");

	CHECK(store.size() == 1);

	SECTION("nothing") {
		CHECK(strong);
	}

	SECTION("weak_cpct") {
		libv::entity_ptr<Planet> weak_cpct{strong};
		CHECK(strong);
		CHECK(strong == weak_cpct);
		CHECK(store.size() == 1);
	}
	SECTION("weak_cpas") {
		libv::entity_ptr<Planet> weak_cpas = strong;
		CHECK(strong);
		CHECK(strong == weak_cpas);
		CHECK(store.size() == 1);
	}

	SECTION("weak_mvct") {
		libv::entity_ptr<Planet> weak_mvct{std::move(strong)};
		CHECK(!strong);
		CHECK(kill_was_called);
		CHECK(store.size() == 1);
		weak_mvct.reset();
		CHECK(store.size() == 0);
	}
	SECTION("weak_mvas") {
		libv::entity_ptr<Planet> weak_mvas = std::move(strong);
		CHECK(!strong);
		CHECK(kill_was_called);
		CHECK(store.size() == 1);
		weak_mvas.reset();
		CHECK(store.size() == 0);
	}

	SECTION("strong_mvct") {
		libv::primary_entity_ptr<Planet> strong_mvct{std::move(strong)};
		CHECK(!strong);
		CHECK(strong_mvct);
		CHECK(!kill_was_called);
		CHECK(store.size() == 1);
		strong_mvct.reset();
		CHECK(kill_was_called);
		CHECK(store.size() == 0);
	}
	SECTION("strong_mvas") {
		libv::primary_entity_ptr<Planet> strong_mvas = std::move(strong);
		CHECK(!strong);
		CHECK(strong_mvas);
		CHECK(!kill_was_called);
		CHECK(store.size() == 1);
		strong_mvas.reset();
		CHECK(kill_was_called);
		CHECK(store.size() == 0);
	}

	SECTION("strong_mvct + weak anchor") {
		libv::entity_ptr<Planet> weak{strong};
		libv::primary_entity_ptr<Planet> strong_mvct{std::move(strong)};
		CHECK(!strong);
		CHECK(weak);
		CHECK(strong_mvct);
		CHECK(!kill_was_called);
		CHECK(store.size() == 1);
		strong_mvct.reset();
		CHECK(weak);
		CHECK(kill_was_called);
		CHECK(store.size() == 1);
		weak.reset();
		CHECK(store.size() == 0);
	}

	SECTION("strong_mvas + weak anchor") {
		libv::entity_ptr<Planet> weak{strong};
		libv::primary_entity_ptr<Planet> strong_mvas = std::move(strong);
		CHECK(!strong);
		CHECK(weak);
		CHECK(strong_mvas);
		CHECK(!kill_was_called);
		CHECK(store.size() == 1);
		strong_mvas.reset();
		CHECK(weak);
		CHECK(kill_was_called);
		CHECK(store.size() == 1);
		weak.reset();
		CHECK(store.size() == 0);
	}

	CHECK((bool(strong) == !kill_was_called));
	strong.reset();

	CHECK(kill_was_called);
	CHECK(store.size() == 0);
}
