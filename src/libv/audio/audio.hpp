//

#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <soloud/soloud.h>


namespace libv::audio {

// -------------------------------------------------------------------------------------------------

enum class DeviceID : int32_t { };
DeviceID defaultDeviceID{-1};
// DeviceID nullDeviceID{-2};

// enum class Backend : int32_t {
// 	null,
// 	winmm,
// 	// sdl_mixer,
// };

struct AudioOutputDevice {
	DeviceID deviceID{0};
	std::string name;
};

// -------------------------------------------------------------------------------------------------

class AudioEngine {
	SoLoud::Soloud gSoloud;

public:
	std::vector<const AudioOutputDevice> listOutputDevices();

	void init(DeviceID);


};

// -------------------------------------------------------------------------------------------------

} // namespace libv::audio
