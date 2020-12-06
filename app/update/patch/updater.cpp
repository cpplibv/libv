// Project: libv, File: app/update/patch/updater.cpp, Author: Császár Mátyás [Vader]

// hpp
//#include <update/patch/patch_applier.hpp>
#include <update/patch/updater.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
//#include <vector>
// pro
//#include <update/patch/patch.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

void Updater::update(std::filesystem::path root_dir, class Patch& patch) {

//	const auto failure_cb = [this](std::string filepath, std::error_code ec) {
//		failures.emplace_back(std::move(filepath), ec);
//	};
//
//	filepath_lock = root_dir / filepath_lock;
//	filepath_journey = root_dir / filepath_journey;
//
//	std::error_code ec;
//
//	try {
//		libv::process::lock_file_guard lock_file(filepath_lock);
//
//		const auto journey_exists = std::filesystem::exists(filepath_journey, ec);
//		if (ec)
//			failure_cb(filepath_journey.generic_string(), ec);
//
//		if (journey_exists) {
//			const auto journey_data = libv::read_file(filepath_journey, ec);
//			if (ec)
//				failure_cb(filepath_journey.generic_string(), ec);
//
//			// load patch file based journey_data
//		}
//
//		progress2(root_dir, journey_exists);
//
//		if (failures.empty()) {
//			std::filesystem::remove(filepath_journey, ec);
//			if (ec)
//				failure_cb(filepath_journey.generic_string(), ec);
//		}
//	} catch (const std::exception& e) {
//
//	}
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
