// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#include <catch.hpp>

#include <libv/ecs/system.hpp>


// -------------------------------------------------------------------------------------------------

template <size_t N>
struct Value3 : libv::ecs::Component<Value3<N>, N> {
	uint32_t value0 = 0;
	uint32_t value1 = 0;
	uint32_t value2 = 0;

	Value3() { }
	Value3(uint32_t v0) : value0(v0) { }
	Value3(uint32_t v0, uint32_t v1) : value0(v0), value1(v1) { }
	Value3(uint32_t v0, uint32_t v1, uint32_t v2) : value0(v0), value1(v1), value2(v2) { }
};

using TestCompA = Value3<1>;
using TestCompB = Value3<2>;
using TestCompC = Value3<3>;

// -------------------------------------------------------------------------------------------------

TEST_CASE("basic create", "[ecs]") {
	libv::ecs::System es;
	CHECK(es.entityCount() == 0);

	es.create<>();
	CHECK(es.entityCount() == 1);
}

TEST_CASE("basic create and foreach on ecs", "[ecs]") {
	libv::ecs::System es;

	const auto entity = es.create<TestCompA>([](auto& a) {
		a.value0 = 1;
	});

	uint32_t resultA = 0;
	es.foreach<TestCompA>([&](const auto& a) {
		++resultA;
		CHECK(a.value0 == 1);
	});

	CHECK(resultA == 1);
	CHECK(es.entityCount() == 1);
}

TEST_CASE("insert or emplace component", "[ecs]") {
	libv::ecs::System es;

	const auto entity = es.create();

	SECTION("emplace") {
		es.emplaceComponent<TestCompA>(entity);
		es.emplaceComponent<TestCompB>(entity, 42);
		es.emplaceComponent<TestCompC>(entity, 43, 44);
	}
	SECTION("emplace move") {
		es.emplaceComponent<TestCompA>(entity, TestCompA{});
		es.emplaceComponent<TestCompB>(entity, TestCompB{42});
		es.emplaceComponent<TestCompC>(entity, TestCompC{43, 44});
	}
	SECTION("insert") {
		es.insertComponent(entity, TestCompA{});
		es.insertComponent(entity, TestCompB{42});
		es.insertComponent(entity, TestCompC{43, 44});
	}

	size_t foreach_run_count = 0;
	es.foreach<TestCompA, TestCompB, TestCompC>([&](const auto& a, const auto& b, const auto& c) {
		++foreach_run_count;
		CHECK(a.value0 == 0);
		CHECK(a.value1 == 0);
		CHECK(a.value2 == 0);
		CHECK(b.value0 == 42);
		CHECK(b.value1 == 0);
		CHECK(b.value2 == 0);
		CHECK(c.value0 == 43);
		CHECK(c.value1 == 44);
		CHECK(c.value2 == 0);
	});

	CHECK(foreach_run_count == 1);
	CHECK(es.entityCount() == 1);
	CHECK(es.componentCount<TestCompA>() == 1);
	CHECK(es.componentCount<TestCompB>() == 1);
	CHECK(es.componentCount<TestCompC>() == 1);
}

TEST_CASE("non matching foreach", "[ecs]") {
	libv::ecs::System es;

	const auto entity0 = es.create();
	const auto entity1 = es.create();

	es.emplaceComponent<TestCompA>(entity0);
	es.emplaceComponent<TestCompB>(entity0, 42);

	es.emplaceComponent<TestCompC>(entity1, 42);

	size_t foreach_run_count = 0;
	es.foreach<TestCompA, TestCompB, TestCompC>([&](const auto&...) {
		++foreach_run_count;
	});

	CHECK(foreach_run_count == 0);
	CHECK(es.entityCount() == 2);
}

TEST_CASE("non existing component store", "[ecs]") {
	libv::ecs::System es;

	const auto entity0 = es.create();

	es.emplaceComponent<TestCompA>(entity0);

	size_t foreach_run_count = 0;
	es.foreach<TestCompA, TestCompB>([&](const auto&...) {
		++foreach_run_count;
	});

	CHECK(foreach_run_count == 0);
	CHECK(es.entityCount() == 1);
}

TEST_CASE("foreach children", "[ecs]") {
	libv::ecs::System es;

	const auto parent0 = es.create();
	const auto parent1 = es.create();

	SECTION("order 00111") {
		es.create(parent0);
		es.create(parent0);
		es.create(parent1);
		es.create(parent1);
		es.create(parent1);
	}
	SECTION("order 01011") {
		es.create(parent0);
		es.create(parent1);
		es.create(parent0);
		es.create(parent1);
		es.create(parent1);
	}
	SECTION("order 11001") {
		es.create(parent1);
		es.create(parent1);
		es.create(parent0);
		es.create(parent0);
		es.create(parent1);
	}

	size_t foreach_run_count0 = 0;
	es.foreach_children<>(parent0, [&](const auto&) {
		++foreach_run_count0;
	});

	size_t foreach_run_count1 = 0;
	es.foreach_children<>(parent1, [&](const auto&) {
		++foreach_run_count1;
	});

	CHECK(es.entityCount() == 7);
	CHECK(foreach_run_count0 == 2);
	CHECK(foreach_run_count1 == 3);
}

TEST_CASE("optional component request", "[ecs]") {
	libv::ecs::System es;

	const auto entity0 = es.create();
	es.emplaceComponent<TestCompA>(entity0);

	const auto entity1 = es.create();
	es.emplaceComponent<TestCompB>(entity1);

	size_t foreach_run_count = 0;
	size_t a_found_count = 0;
	es.foreach<TestCompA::optional>([&](auto&& a) {
		++foreach_run_count;
		if (a)
			++a_found_count;

		static_assert(std::is_same_v<decltype(a), TestCompA*&&>, "");
	});

	CHECK(es.entityCount() == 2);
	CHECK(a_found_count == 1);
	CHECK(foreach_run_count == 2);
}

TEST_CASE("hybrid optional and basic component request", "[ecs]") {
	libv::ecs::System es;

	size_t expected_foreach_run_count = 0;
	size_t expected_c_found_count = 0;

	const auto entity0 = es.create();
	const auto entity1 = es.create();
	const auto entity2 = es.create();

	SECTION("Present first") {
		es.emplaceComponent<TestCompA>(entity0);
		es.emplaceComponent<TestCompC>(entity0);

		es.emplaceComponent<TestCompA>(entity1);

		expected_foreach_run_count = 2;
		expected_c_found_count = 1;
	}

	SECTION("Optional first") {
		es.emplaceComponent<TestCompA>(entity0);

		es.emplaceComponent<TestCompA>(entity1);
		es.emplaceComponent<TestCompC>(entity1);

		expected_foreach_run_count = 2;
		expected_c_found_count = 1;
	}

	SECTION("Present middle") {
		es.emplaceComponent<TestCompA>(entity0);

		es.emplaceComponent<TestCompA>(entity1);
		es.emplaceComponent<TestCompC>(entity1);

		es.emplaceComponent<TestCompA>(entity2);

		expected_foreach_run_count = 3;
		expected_c_found_count = 1;
	}

	SECTION("Optional middle") {
		es.emplaceComponent<TestCompA>(entity0);
		es.emplaceComponent<TestCompC>(entity0);

		es.emplaceComponent<TestCompA>(entity1);

		es.emplaceComponent<TestCompA>(entity2);
		es.emplaceComponent<TestCompC>(entity2);

		expected_foreach_run_count = 3;
		expected_c_found_count = 2;
	}

	size_t foreach_run_count = 0;
	size_t c_found_count = 0;
	es.foreach<TestCompA, TestCompC::optional>([&](auto&& a, auto&& c) {
		++foreach_run_count;
		if (c)
			++c_found_count;

		static_assert(std::is_same_v<decltype(a), TestCompA&>, "");
		static_assert(std::is_same_v<decltype(c), TestCompC*&&>, "");
	});

	CHECK(c_found_count == expected_c_found_count);
	CHECK(foreach_run_count == expected_foreach_run_count);
}

TEST_CASE("foreach component", "[ecs]") {
	libv::ecs::System es;

	const auto entity0 = es.create();
	const auto entity1 = es.create();

	size_t foreach_run_left = 0;

	SECTION("none") {
		foreach_run_left = 0;
	}
	SECTION("one") {
		SECTION("first") {
			es.emplaceComponent<TestCompA>(entity0);
		}
		SECTION("middle") {
			es.emplaceComponent<TestCompA>(entity1);
			es.create();
		}
		SECTION("last") {
			es.emplaceComponent<TestCompA>(entity1);
		}

		foreach_run_left = 1;
	}
	SECTION("multiple") {
		es.emplaceComponent<TestCompA>(entity0);
		es.emplaceComponent<TestCompA>(entity1);

		foreach_run_left = 2;
	}

	es.foreach_component<TestCompA>([&](auto&&) {
		--foreach_run_left;
	});

	CHECK(foreach_run_left == 0);
}

TEST_CASE("get simple", "[ecs]") {
	libv::ecs::System es;

	const auto entity0 = es.create();
	es.emplaceComponent<TestCompA>(entity0);

	size_t foreach_run_left = 1;
	es.get<TestCompA>(entity0, [&](auto&& a) {
		--foreach_run_left;
		static_assert(std::is_same_v<decltype(a), TestCompA&>, "");
	});

	CHECK(foreach_run_left == 0);
}

TEST_CASE("get optional", "[ecs]") {
	libv::ecs::System es;

	const auto entity0 = es.create();
	es.emplaceComponent<TestCompA>(entity0);

	size_t foreach_run_left = 1;
	es.get<TestCompA, TestCompC::optional>(entity0, [&](auto&& a, auto&& c) {
		--foreach_run_left;
		static_assert(std::is_same_v<decltype(a), TestCompA&>, "");
		static_assert(std::is_same_v<decltype(c), TestCompC*&&>, "");
	});

	CHECK(foreach_run_left == 0);
}
