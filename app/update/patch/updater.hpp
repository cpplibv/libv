// Project: libv, File: app/update/patch/updater.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
//#include <memory>
//#include <string>
//#include <vector>
// pro
//#include <update/patch/patch.hpp>
#include <update/patch/constants.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct UpdaterSettings {
	std::string filename_manifest = libv::update::filename_manifest;
	std::string filename_lock = libv::update::filename_lock;
	std::string filename_progress = libv::update::filename_progress;
};

struct Updater {

public:
	std::filesystem::path filepath_lock;
	std::filesystem::path filepath_journey;

public:
	void update();
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
