// Project: libv.noise, File: sandbox/libv_noise_main.cpp

// ext
#include <fmt/printf.h>
#include <glm/gtc/noise.hpp>
// libv
#include <libv/noise/noise.hpp>
#include <libv/utility/timer.hpp>
// std
#include <iostream>


// -------------------------------------------------------------------------------------------------

int main() {
	float value0 = 0.f;
	float value1 = 0.f;
	float value2 = 0.f;
	float value3 = 0.f;
	float value4 = 0.f;
	float value5 = 0.f;
	float value6 = 0.f;

	const auto numIteration = 8044200;
	// const auto seed = 5065233;

	for (int j = 0; j < 10; ++j) {
		const auto fj = static_cast<float>(j - 2) * 0.053209f;
		libv::Timer timer;


		for (int i = 0; i < numIteration; ++i) {
			const auto fi = (static_cast<float>(i)  - static_cast<float>(numIteration) * 0.2f) * 0.01597f;
			value6 += glm::simplex(glm::vec2(fi * fj, fi * 2.f));
		}

		fmt::print("GLM Simplex  : {:6.3f}\n", timer.timef_ms().count());
		timer.reset();


		for (int i = 0; i < numIteration; ++i) {
			const auto fi = (static_cast<float>(i)  - static_cast<float>(numIteration) * 0.2f) * 0.01597f;
			value1 += libv::noise::simplex(12345, fi * fj, fi * 2.f);
		}

		fmt::print("Noise 2      : {:6.3f}\n", timer.timef_ms().count());
		timer.reset();
//
//		for (int i = 0; i < numIteration; ++i) {
//			const auto fi = (static_cast<float>(i)  - static_cast<float>(numIteration) * 0.2f) * 0.01597f;
//			value2 += libv::noise_simplex_v(seed, fi * fj, fi * 2.f);
//		}
//
//		fmt::print("SIMD 1       : {:6.3f}\n", timer.timef_ms().count());
//		timer.reset();
//
//		for (int i = 0; i < numIteration; i += 4) {
//			const auto fi = (static_cast<float>(i)  - static_cast<float>(numIteration) * 0.2f) * 0.01597f;
//			libv::vec4f xs = libv::vec4f{fi + 0, fi + 1, fi + 2, fi + 3} * fj;
//			libv::vec4f ys{(fi + 0) * 2.f, (fi + 1) * 2.f, (fi + 2) * 2.f, (fi + 3) * 2.f};
//			const auto vs = libv::noise_simplex_v(seed, xs, ys);
//			value3 += vs.x + vs.y + vs.z + vs.w;
//
//	//		const auto fi = static_cast<float>(i);
//	//		value += libv::noise_simplex(0, fi, fi * 2.f);
//		}
//
//		fmt::print("SIMD 4       : {:6.3f}\n", timer.timef_ms().count());
		std::cout << std::endl;
	}

	fmt::print("\n");
	fmt::print("Value of Simplex      : {}\n", value0);
	fmt::print("Value of Simplex2S    : {}\n", value5);
	fmt::print("Value of GLM Simplex  : {}\n", value6);
	fmt::print("Value of Wombat GLSL V: {}\n", value1);
	fmt::print("Value of Wombat GLSL  : {} {}\n", value4, value1 == value4 ? "OK" : "WRONG!");
	fmt::print("Value of SIMD 1       : {}\n", value2);
	fmt::print("Value of SIMD 4       : {}\n", value3);

//	for (int i = 0; i < 1000000; i += 100)
//		fmt::print("{:7.5f} vs {:7.5f}\n",
//				libv::noise_simplex(3523, i * 1.332f, i * 2.21321f),
//				libv::noise_simplex_v(3523, i * 1.332f, i * 2.21321f));
}

// -------------------------------------------------------------------------------------------------
