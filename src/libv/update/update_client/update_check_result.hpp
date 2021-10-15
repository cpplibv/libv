// Project: libv.update, File: src/libv/update/update_client/update_check_result.hpp

#pragma once


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

enum class update_check_result {
	version_outdated,
	version_up_to_date,
	version_not_supported,
	communication_error,
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
