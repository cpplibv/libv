// File: ecs.cpp Author: Vader Created on 2017. július 8., 23:48

// ext
#include <fmt/format.h>
// std
#include <iostream>
// pro
#include <libv/ecs/system.hpp>
#include <libv/utility/timer.hpp>
#include <random>


// -------------------------------------------------------------------------------------------------

struct A : libv::ecs::Component<A, 0> { int value = 0; };
struct B : libv::ecs::Component<B, 1> { int value = 0; };
struct C : libv::ecs::Component<C, 2> { int value = 0; };
struct D : libv::ecs::Component<D, 3> { int value = 0; };
struct E : libv::ecs::Component<E, 4> { int value = 0; };

struct ABCDE : libv::ecs::Component<ABCDE, 15> { int value = 0; char d[32]; };

// -------------------------------------------------------------------------------------------------

size_t store = 0;
int mod = 1543;
size_t result;
std::vector<ABCDE*> r;
std::vector<ABCDE> r2;

int main(int, char**) {
	libv::Timer timer;

// IDEA: es.range<...>() could yield an iterator pair
//		libv::ecs::System es;
//		for (auto& [a, b, c, d] : es.range<A, B, C, D>()) {
//			// ...
//		}

	{
		libv::ecs::System es;

		for (size_t i = 0; i < 2'000'000; ++i) {
			es.create<A, B, C, D>([] (auto& a, auto&...) {
				a.value = 0;
			});
		}

		timer.reset();

		es.foreach<A, B, C, D>([&](auto& a, auto&...){
			result += a.value % mod;
		});

		std::cout << "ECS: " << static_cast<float>(timer.time_us().count()) / 1000.f << "ms" << std::endl;
	}
	{
		libv::ecs::System es;

		for (size_t i = 0; i < 2'000'000; ++i) {
			es.create<A, B, C, D>([] (auto& a, auto&...) {
				a.value = 0;
			});
		}

		timer.reset();

		es.foreach_children<A, B, C, D>(0, [&](auto& e, auto& a, auto&...) {
			(void) e;
			result += a.value % mod;
		});

		std::cout << "ECScs: " << static_cast<float>(timer.time_us().count()) / 1000.f << "ms" << std::endl;
	}
	{
		libv::ecs::System es;

		for (size_t i = 0; i < 2'000'000; ++i) {
			es.create<ABCDE>([] (auto& a) {
				a.value = 0;
			});
		}

		timer.reset();

		es.foreach<ABCDE>([&](auto& a) {
			result += a.value % mod;
		});

		std::cout << "ECS2: " << static_cast<float>(timer.time_us().count()) / 1000.f << "ms"  << std::endl;
	}
	{
		libv::ecs::System es;

		for (size_t i = 0; i < 2'000'000; ++i) {
			es.create<ABCDE>([] (auto& a) {
				a.value = 0;
			});
		}

		timer.reset();

		es.foreach_children<ABCDE>(0, [&](auto& e, auto& a) {
			(void) e;
			result += a.value % mod;
		});

		std::cout << "ECS2ch: " << static_cast<float>(timer.time_us().count()) / 1000.f << "ms"  << std::endl;
	}
	{
		for (size_t i = 0; i < 2'000'000; ++i) {
			r.emplace_back(new ABCDE());
		}

		std::shuffle(r.begin(), r.end(), std::default_random_engine());

		timer.reset();

		for (size_t i = 0; i < 2'000'000; ++i) {
			result += r[i]->value % mod;
		}

		std::cout << "RAW: " << static_cast<float>(timer.time_us().count()) / 1000.f << result << "ms"  << std::endl;
	}
	{
		for (size_t i = 0; i < 2'000'000; ++i) {
			r2.emplace_back();
		}

		std::shuffle(r.begin(), r.end(), std::default_random_engine());

		timer.reset();

		for (size_t i = 0; i < 2'000'000; ++i) {
			result += r2[i].value % mod;
		}

		std::cout << "loc: " << static_cast<float>(timer.time_us().count()) / 1000.f << result << "ms"  << std::endl;
	}

	return 0;
}
