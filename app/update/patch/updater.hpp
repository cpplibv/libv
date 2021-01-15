// Project: libv, File: app/update/patch/updater.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <string>
// pro
#include <update/patch/version_number.hpp>
#include <update/common/client.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

// */**
// */**.new
// */**.old
// patch/v0001.0000.patch
// patch/v{to}.{from}.patch
// patch/v*.*.patch
// .lock
// .patch
// program
//
//	update outcomes:
//		failure - operation failure
//			attempt to retry and continue
//			rollback -> do nothing
//			rollback -> fix corruption -> do nothing
//			rollback -> fix corruption -> update again
//			force apply -> fix corruption
//		failure - corruption
//			rollback -> do nothing
//			rollback -> fix corruption -> do nothing
//			rollback -> fix corruption -> update again
//			force apply -> fix corruption
//		success

// -------------------------------------------------------------------------------------------------

//static constexpr auto filename_manifest = ".manifest";
static constexpr auto filename_lock = ".lock";
static constexpr auto filename_journey = ".patch";
//static constexpr auto directory_patch = "patch/";

struct UpdaterSettings {
//	std::string filename_manifest = libv::update::filename_manifest;
	std::string filename_lock = libv::update::filename_lock;
	std::string filename_journey = libv::update::filename_journey;
//	std::string directory_patch = libv::update::directory_patch;

	bool remove_patch_files_after_success = true;
};

// -------------------------------------------------------------------------------------------------

struct Updater {

public:
	UpdaterSettings settings;

public:
	version_number version_current;
	version_number version_target;

public:
	std::filesystem::path filepath_root_dir;
	std::filesystem::path filepath_lock;
	std::filesystem::path filepath_journey;

public:
	void update(
			std::filesystem::path root_dir,
			std::vector<std::filesystem::path> patches,
			version_number current,
			version_number target);
	void update();

//	void migrate(version_number migrate_on, std::function<void()> callback);
};

//void foo() {
//	Updater updater;
//	updater.update();
//	updater.migrate(0, []() {
//
//	});
//	updater.migrate(2, []() {
//
//	});
//	updater.migrate(16, []() {
//
//	});
//}

// -------------------------------------------------------------------------------------------------

//struct UpdateChecker {
//	std::vector<libv::net::Address> server_addresses;
//
//public:
//	UpdateChecker(std::vector<libv::net::Address> servers);
//	std::vector<std::string> check(version_number version_current);
//};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
