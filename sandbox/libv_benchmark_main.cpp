// Project: libv, File: sandbox/libv_benchmark_main.cpp

// ext
#include <fmt/printf.h>
// libv
#include <libv/rev/effect/particle.hpp>
// #include <libv/rev/effect/particle_engine.hpp>
// #include <libv/rev/effect/particle_static.hpp>
#include <libv/utility/timer.hpp>


// -------------------------------------------------------------------------------------------------
















int main() {
	libv::Timer timer;
	int64_t numParticle = 0;
	const int repeat_test_count = 5;
	const int duration_test_count = 60 * 60; // 60 sec at 60 UPS
	auto time = timer.timef_ms();

	for (int i = 0; i < repeat_test_count; ++i) {
		libv::rev::ParticleEffect system;

		system.addModule(std::make_unique<libv::rev::ParticleModuleEmitter>(true, libv::vec3f{0}, libv::vec3f{0, 0, 100.f}, 1000.f));
		system.addModule(std::make_unique<libv::rev::ParticleModuleGravity>());

		for (int j = 0; j < duration_test_count; ++j)
			system.update(libv::time_duration_f{1.f / 60.f});

		numParticle += system.numParticle();
	}

	time = timer.timef_ms();
	fmt::print("{}, Dynamic {:7.3f}ms, Cost / Frame: {:7.3f}us\n", static_cast<double>(numParticle) / repeat_test_count, time.count(), time.count() * 1000 / repeat_test_count / duration_test_count);
	numParticle = 0;
	timer.reset();

	// ---

	// for (int i = 0; i < repeat_test_count; ++i) {
	// 	idea::ParticleSystemStatic system{
	// 			idea::ModuleEmitter(true, libv::vec3f{0}, libv::vec3f{0, 0, 100.f}, 1000.f),
	// 			idea::ModuleEmitter(true, libv::vec3f{0}, libv::vec3f{0, 0, 10.f}, 10000.f),
	// 			idea::ModuleGravity(),
	// 			idea::ModuleGravity(libv::vec3f{-1, 0, 0})
	// 	};
	//
	// 	for (int j = 0; j < duration_test_count; ++j)
	// 		system.update(1.f / 60.f);
	//
	// 	numParticle += system.numParticle();
	// }
	//
	// time = timer.timef_ms();
	// fmt::print("{}, Static {:7.3f}ms {:7.3f}us\n", static_cast<double>(numParticle) / repeat_test_count, time.count(), time.count() * 1000 / repeat_test_count / duration_test_count);
	// numParticle = 0;
	// timer.reset();

	return EXIT_SUCCESS;
}
