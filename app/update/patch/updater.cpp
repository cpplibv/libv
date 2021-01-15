// Project: libv, File: app/update/patch/updater.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/patch/updater.hpp>
// libv
#include <libv/process/lock_file.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
// pro
#include <update/patch/patch_applier.hpp>
#include <update/patch/patch.hpp>



//#include <vector>
// pro
//#include <update/patch/patch.hpp>


//namespace fs {
//
//bool exists_throw(const std::filesystem::path& path) {
//	std::error_code ec;
//
//	const auto exists = std::filesystem::exists(path, ec);
//	if (ec)
//		throw io_exception(path, ec);
//
//	return exists;
//}
//
//} // namespace fs

namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

void Updater::update(
		std::filesystem::path root_dir,
		std::vector<std::filesystem::path> patches,
		version_number current,
		version_number target) {

	version_current = current;
	version_target = target;

	filepath_root_dir = root_dir;
	filepath_lock = root_dir / settings.filename_lock;
	filepath_journey = root_dir / settings.filename_lock;
}

void Updater::update() {

	try {
		libv::process::lock_file_guard lock_file(filepath_lock);

		const auto journey_exists = std::filesystem::exists(filepath_journey);
		if (journey_exists) {
			const auto journey_content = libv::read_file_or_throw(filepath_journey);


		}

//		std::error_code ec;
//
//		const auto journey_exists = std::filesystem::exists(filepath_journey, ec);
//		if (ec)
//			fail;
//
//		if (journey_exists) {
////			const auto journey_content = libv::read_file(filepath_journey, ec);
////			if (ec)
////				fail;
//
////			root_dir / id_of_journey_content
////			id_of_journey_content
//
//			const auto journey_exists = std::filesystem::exists(id_of_journey_content, ec);
//			if (ec || !journey_exists) {
//				// exists failure can just assume invalid content of journey file
//				libv::write_file(filepath_journey, id_for_patch, ec);
//				if (ec)
//					fail;
//			}
//
//			patch = Patch::load_from_file(id_for_patch_or_journey_content);
//
//			const auto continue_ = journey_exists and journey_is_good;
//
//			PatchApplier applier(root_dir, patch, journey_exists and journey_good);
//			while (applier.progress());
//			const auto success = applier.failures().empty();
//
//			if (!success)
//				fail;
//
//		} else {
//
//
//		}







	} catch (const std::filesystem::filesystem_error& e) {
		e.code();

		e.path1();
		if (!e.path2().empty())
			e.path2();

	} catch (const std::exception& e) {

	}
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
