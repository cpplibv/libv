// Project: libv, File: app/update/patch/updater.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
//#include <memory>
//#include <string>
//#include <vector>
// pro
//#include <update/patch/patch.hpp>
//#include <update/patch/constants.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

static constexpr auto filename_manifest = ".manifest";
static constexpr auto filename_lock = ".lock";
static constexpr auto filename_progress = ".patch";

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
	void update(std::filesystem::path root_dir, class Patch& patch);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
