// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// ext
#include <fmt/printf.h>
// libv
#include <libv/utility/timer.hpp>
// std
#include <random>
// pro
#include <libv/ecs/system.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

template <size_t ID, typename T, size_t N>
struct TestComp : libv::ecs::Component<TestComp<ID, T, N>, ID> {
	std::array<T, N> data;

	inline TestComp() = default;
	inline TestComp(std::array<T, N> data) : data(std::move(data)) { }
};

using TestCompA = TestComp<1, uint8_t, 16>;
using TestCompB = TestComp<2, uint8_t, 32>;
using TestCompC = TestComp<3, uint8_t, 64>;

} // namespace

// -------------------------------------------------------------------------------------------------

// TODO P3: time and measure everything
libv::ecs::System es;
int dummy = 0;

TEST_CASE("ecs fill time", "[libv.ecs]") {
	const auto num_entity = 100000;
	const auto rng_seed = 0x5EED;
	const auto group_size = 16;

	std::mt19937_64 rng{rng_seed};
    auto chanceA = std::bernoulli_distribution{0.75};
    auto chanceB = std::bernoulli_distribution{0.50};
    auto chanceC = std::bernoulli_distribution{0.25};
//    auto distData = std::uniform_int_distribution<std::byte>{0, 255};

	std::vector<int> has_a; has_a.resize(num_entity);
	std::vector<int> has_b; has_b.resize(num_entity);
	std::vector<int> has_c; has_c.resize(num_entity);

	libv::Timer timer;

	for (size_t i = 0; i < num_entity; ++i) {
		has_a[i] = chanceA(rng);
		has_b[i] = chanceB(rng);
		has_c[i] = chanceC(rng);
	}

	{
		auto time = timer.timef_ms().count();
		fmt::print("Generation    : {:7.3f}ms {:7.3f}μs/entity\n", time, time * 1000.f / num_entity);
		timer.reset();
	}

	for (size_t i = 0; i < num_entity; ++i) {
		auto entity = es.create();

		if (has_a[i])
			es.insertComponent(entity, TestCompA{});
		if (has_b[i])
			es.insertComponent(entity, TestCompB{});
		if (has_c[i])
			es.insertComponent(entity, TestCompC{});
	}

	{
		auto time = timer.timef_ms().count();
		fmt::print("Filling       : {:7.3f}ms {:7.3f}μs/entity\n", time, time * 1000.f / num_entity);
		timer.reset();
	}


	struct Layout {
		std::string name;
		int index;
		uint64_t mask;
		int matching = 0;
		int count = 0;
		int entity_skipped = 0;
		int group_skipped = 0;

		Layout(std::string_view name, int index, uint64_t mask) : name(name), index(index), mask(mask) {}
	};

	const auto scan = [&](libv::ecs::System& es, Layout& layout) {
		bool skipping = true;
		int group_index = 0;

		es.foreach_entity([&](const libv::ecs::Entity& entity) {
			if (entity.components.to_ullong() == layout.mask)
				layout.count++;

			if ((entity.components.to_ullong() & layout.mask) == layout.mask) {
				skipping = false;
				layout.matching++;
			} else if (skipping) {
				layout.entity_skipped++;
			}

			if (++group_index == group_size) {
				if (skipping)
					layout.group_skipped++;

				group_index = 0;
				skipping = true;
			}
		});
	};

	const auto layout_types = [](auto&& func) {
		func(                                     );
		func(TestCompA{}                          );
		func(             TestCompB{}             );
		func(TestCompA{}, TestCompB{}             );
		func(                          TestCompC{});
		func(TestCompA{},              TestCompC{});
		func(             TestCompB{}, TestCompC{});
		func(TestCompA{}, TestCompB{}, TestCompC{});
	};

	std::vector<Layout> layouts;
	scan(es, layouts.emplace_back("---", 0, 0 << 1));
	scan(es, layouts.emplace_back("A--", 1, 1 << 1));
	scan(es, layouts.emplace_back("-B-", 2, 2 << 1));
	scan(es, layouts.emplace_back("AB-", 3, 3 << 1));
	scan(es, layouts.emplace_back("--C", 4, 4 << 1));
	scan(es, layouts.emplace_back("A-C", 5, 5 << 1));
	scan(es, layouts.emplace_back("-BC", 6, 6 << 1));
	scan(es, layouts.emplace_back("ABC", 7, 7 << 1));

	// ---------------------------------------------------------------------------------------------

	fmt::print("\n");
	fmt::print("Component A: {:5}\n", es.componentCount<TestCompA>());
	fmt::print("Component B: {:5}\n", es.componentCount<TestCompB>());
	fmt::print("Component C: {:5}\n", es.componentCount<TestCompC>());

	// ---------------------------------------------------------------------------------------------

	fmt::print("\n");
	for (const auto& layout : layouts) {
		fmt::print("Layout: {}, count: {:5}, match:{:6}, skipped: {:5} {:5.2f}%, group_skipped: {:3}\n",
				layout.name,
				layout.count,
				layout.matching,
				layout.entity_skipped, layout.entity_skipped / static_cast<float>(num_entity) * 100,
				layout.group_skipped);
	}

	// ---------------------------------------------------------------------------------------------

	fmt::print("\n");
	size_t layout_index = 0;
	layout_types([&](auto... layout_types) {
		auto& layout = layouts[layout_index++];

		timer.reset();

		es.foreach<decltype(layout_types)...>([&](const auto&... comps) {
			dummy = (dummy + ... + comps.data[0]);
		});

		auto time = timer.timef_ms().count();
		fmt::print("Foreach: {}, hit:{:6.2f}%, time: {:7.3f} ms {:7.3f} μs/entity {:7.3f} μs/row {:7.3f} GB/s\n",
				layout.name,
				layout.matching / static_cast<float>(num_entity) * 100,
				time,
				time * 1000.f / num_entity,
				time * 1000.f / layout.matching,
				(1000 / time) * (0 + ... + sizeof(layout_types)) * layout.matching / 1024 / 1024 / 1024
		);
	});

	fmt::print("\n{}\n", dummy);

//	CHECK(resultA == 2);
//	CHECK(resultB == 2);
//	CHECK(es.componentCount<TestCompA>() == 2);
//	CHECK(es.componentCount<TestCompB>() == 2);
//	CHECK(es.componentCount<TestCompC>() == 2);
}
