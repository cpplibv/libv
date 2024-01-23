// Project: libv.res, File: src/libv/res/load_file.cpp

#include <libv/res/load_file.hpp>

#include <fmt/format.h>

#include <libv/algo/slice.hpp>
#include <libv/container/small_vector.hpp>
#include <libv/res/common_resource_event.hpp>
#include <libv/res/settings.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/read_file.hpp>

#include <expected>
#include <sstream>
#include <string>
#include <string_view>


namespace libv::res {

// -------------------------------------------------------------------------------------------------

std::expected<std::string, FileLoadFailure> loadFile(
		const ResourceLoaderSettings& resourceLoaderSettings,
		std::string_view fileIdentifier,
		libv::function_ref<void(std::string_view physicalFilepath)> fileTracker) {

	libv::small_vector<ResourceMappingError, 8> mappingErrors;
	libv::small_vector<LoadFilePhysicalError, 8> physicalErrors;

	for (const auto& [resourcePrefix, virtualPrefix] : resourceLoaderSettings.resourceMappings) {
		if (!fileIdentifier.starts_with(resourcePrefix))
			continue;

		// Apply mapping rules
		const auto virtualName = libv::slice_off_view(fileIdentifier, static_cast<int64_t>(resourcePrefix.size()));
		auto virtualFilepath = fmt::format("{}{}", virtualPrefix, virtualName);
		auto physicalFilepathResult = libv::res::resource_path_ec(virtualFilepath);
		if (!physicalFilepathResult) {
			mappingErrors.emplace_back(virtualFilepath, physicalFilepathResult.error());
			continue;
		}
		auto physicalFilepath = physicalFilepathResult.value();

		// File tracking
		if (resourceLoaderSettings.trackFiles && fileTracker)
			fileTracker(physicalFilepath);

		// Read file data
		auto resource_data = libv::read_file_ec(physicalFilepath);
		if (!resource_data.ec) {
			return std::move(resource_data.data);
		} else {
			physicalErrors.emplace_back(std::string(physicalFilepath), resource_data.ec);

			if (resource_data.ec == std::errc::no_such_file_or_directory)
				continue; // no_such_file_or_directory is not a hard error, move onto the next rule
			else
				break; // treat every other error as hard error
		}
	}

	// Compose the error details about the failures
	std::vector<ResourceMappingUnmatched> unmatchedMappings;
	for (const auto& [resourcePrefix, virtualPrefix] : resourceLoaderSettings.resourceMappings)
		if (!fileIdentifier.starts_with(resourcePrefix))
			unmatchedMappings.emplace_back(resourcePrefix, virtualPrefix);

	return std::unexpected(FileLoadFailure{
			std::move(unmatchedMappings),
			std::vector<ResourceMappingError>{mappingErrors.begin(), mappingErrors.end()},
			std::vector<LoadFilePhysicalError>{physicalErrors.begin(), physicalErrors.end()}});
}

std::string toErrorReport(const FileLoadFailure& error) {
	std::ostringstream errorMsg;
	for (const auto& [resourcePrefix, virtualPrefix] : error.unmatchedMappings)
		errorMsg << "\t\tMapping '" << resourcePrefix << "' -> '" << virtualPrefix << "' - Unmatched prefix\n";
	for (const auto& [virtualFilepath, ec] : error.mappingErrors)
		errorMsg << "\t\tVirtual '" << virtualFilepath << "' - " << libv::res::to_message(ec) << "\n";
	for (const auto& [physicalFilepath, ec] : error.physicalErrors)
		errorMsg << "\t\tPhysical '" << physicalFilepath << "' - " << ec.message() << " (" << ec << ")\n";
	return std::move(errorMsg).str();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv::res
