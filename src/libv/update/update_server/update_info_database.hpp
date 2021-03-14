// Project: libv.update, File: src/libv/update/update_server/update_info_database.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <mutex>
#include <string>
#include <unordered_map>
#include <variant>
// pro
#include <libv/update/common/protocol_upd.hpp>
#include <libv/update/update_signature.hpp>
#include <libv/update/version_number.hpp>



//	struct UpdateRouteResult {
//		update_check_result result;
//	};
//
//	using update_route_outcome = std::variant<msg::UpdateRoute, msg::VersionUpToDate, msg::VersionNotSupported>;
//struct ResultUpdateRoute() {
//	bool up_to_date;
//	bool not_supported;
//};

namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

using update_route_variant = std::variant<msg_upd::UpdateRoute, msg_upd::VersionUpToDate, msg_upd::VersionNotSupported>;

// -------------------------------------------------------------------------------------------------

struct UpdateInfo {
	class VersionInfo* target = nullptr;
	uint64_t size;
	update_signature signature;
};

struct VersionInfo {
	std::string program;
	std::string variant;
	version_number version;

	std::optional<UpdateInfo> update;
};

struct VariantInfo {
	std::unordered_map<version_number, VersionInfo> versions;
};

struct ProgramInfo {
	std::unordered_map<std::string, VariantInfo> variants;
};

// -------------------------------------------------------------------------------------------------

class UpdateInfoDatabase {
	std::mutex mutex;
	std::unordered_map<std::string, ProgramInfo> programs;

public:
	void register_update(std::string program, std::string variant,
			version_number source, version_number target, uint64_t size, update_signature signature);
	void debug_dump();
	update_route_variant get_update_route(std::string program_name, std::string variant_name, version_number version);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
