// Project: libv.rev2, File: sandbox/rev2/rev2_main.cpp

#include <wish/resource_mapping.hpp>

#include <iostream>
#include <thread>

#include "log.hpp"

#include <libv/audio/audio.hpp>
#include <libv/res/resource_path.hpp>
#include <libv/utility/random/uniform_distribution.hpp>


#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include <soloud/soloud_biquadresonantfilter.h>
// #include "soloud.h"
// #include "soloud_wav.h"

#include <random>

// -------------------------------------------------------------------------------------------------

#include <windows.h>
#include <mmsystem.h>

// -------------------------------------------------------------------------------------------------

void resourceInit(int argc, const char* const* argv) {
	wish::change_current_path(argc, argv);
#if not WISH_BUILD_PACKAGE
	libv::res::init_resource_mapping(wish::resource_mapping());
#endif
}

void list() {
	UINT num = waveOutGetNumDevs();
	for (int i = 0; i < num; ++i) {
	// MMRESULT waveOutGetDevCaps(
	//   UINT          uDeviceID,
	//   LPWAVEOUTCAPS pwoc,
	//   UINT          cbwoc
	// );

		WAVEOUTCAPS caps;
		waveOutGetDevCaps(i, &caps, sizeof(WAVEOUTCAPS));

		log_sandbox.info("Device {}", i);
		log_sandbox.info("    wMid: {}", caps.wMid);
		log_sandbox.info("    wPid: {}", caps.wPid);
		log_sandbox.info("    vDriverVersion: {}", caps.vDriverVersion);
		log_sandbox.info("    szPname: {}", caps.szPname);
		log_sandbox.info("    dwFormats: {}", caps.dwFormats);
		log_sandbox.info("    wChannels: {}", caps.wChannels);
		log_sandbox.info("    wReserved1: {}", caps.wReserved1);
		log_sandbox.info("    dwSupport: {}", caps.dwSupport);
	}
}

int main(int argc, const char* const* argv) {
	std::cout << libv::logger_stream;
	libv::logger_stream.setFormat("{severity} {thread_id:2} {module}: {message}, {file}:{line}\n");

	resourceInit(argc, argv);
	log_sandbox.info("Hello Audio!");

	libv::audio::AudioEngine audio;

	list();

	// Sandbox sandbox;
	// sandbox.show();
	// sandbox.join();

	SoLoud::Soloud gSoloud; // SoLoud engine
	gSoloud.init(); // Initialize SoLoud

	SoLoud::Wav gWave;      // One wave file
	auto result = gWave.load("res/kruplk.wav"); // Load a wave
	SoLoud::Wav gWave2;      // One wave file
	auto result2 = gWave2.load("res/congratulations.ogg"); // Load a wave
	log_sandbox.info("Load result {}", result);

	auto handle = gSoloud.play(gWave); // Play the wave
	auto playing2 = gSoloud.play(gWave2); // Play the wave

	std::this_thread::sleep_for(std::chrono::milliseconds{300});
	gSoloud.setPause(playing2, true);
	std::this_thread::sleep_for(std::chrono::milliseconds{300});
	gSoloud.setPause(playing2, false);
	std::this_thread::sleep_for(std::chrono::milliseconds{300});
	gSoloud.stop(playing2);

	SoLoud::BiquadResonantFilter gBQRFilter;
	gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 500, 2);
	auto playing3 = gSoloud.play(gWave2); // Play the wave
	// Set the filter as the second filter of the bus
	// gBus.setFilter(1, &gBQRFilter);
	gSoloud.setGlobalFilter(0, &gBQRFilter);
	gSoloud.fadeFilterParameter(
			playing3, // Sound handle
			0, // First filter
			SoLoud::BiquadResonantFilter::FREQUENCY, // What to adjust
			2000, // Target value
			3); // Time in seconds
	std::this_thread::sleep_for(std::chrono::milliseconds{1000});
	gSoloud.setGlobalFilter(0, nullptr);
	auto playing4 = gSoloud.play(gWave2); // Play the wave
	std::this_thread::sleep_for(std::chrono::milliseconds{1000});
	auto playing5 = gSoloud.play(gWave2); // Play the wave
	gSoloud.setRelativePlaySpeed(playing5, 0.8f);
	std::this_thread::sleep_for(std::chrono::milliseconds{1000});

	std::mt19937 rng;
	auto dist = libv::make_uniform_distribution_inclusive(0.8f, 1.2f);

	for (int i = 0; i < 10; ++i) {
		auto playing5 = gSoloud.play(gWave2); // Play the wave
		gSoloud.setRelativePlaySpeed(playing5, dist(rng));
		std::this_thread::sleep_for(std::chrono::milliseconds{1000});
	}

	for (int i = 0; i < 10000; ++i) {
		const auto x = static_cast<float>(i) / 300.f * 3.1415f;
		const auto v = std::sin(x);

		// gSoloud.setPause(handle2, true);
		// gSoloud.schedulePause(handle2, true);
		// gSoloud.stop(handle2);

		std::this_thread::sleep_for(std::chrono::milliseconds{10});
		gSoloud.setPan(handle, v);    // Use handle to adjust panning

	}
	std::this_thread::sleep_for(std::chrono::seconds{50});

	gSoloud.deinit(); // Clean up!

	return EXIT_SUCCESS;
}
