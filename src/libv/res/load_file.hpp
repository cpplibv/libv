//

#include <libv/res/common_resource_event.hpp>
#include <libv/res/settings.hpp>
#include <libv/utility/function_ref.hpp>

#include <expected>
#include <string>
#include <string_view>


namespace libv::res {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::expected<std::string, FileLoadFailure> loadFile(
		const ResourceLoaderSettings& resourceLoaderSettings,
		std::string_view fileIdentifier,
		libv::function_ref<void(std::string_view physicalFilepath)> fileTracker);

[[nodiscard]] std::string toErrorReport(const FileLoadFailure& error);

// -------------------------------------------------------------------------------------------------

} // namespace libv::res
