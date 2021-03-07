// Project: libv.update, File: src/libv/update/server/update_info_database.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/server/update_info_database.hpp>
// libv
#include <libv/utility/enum.hpp>
// pro
#include <libv/update/log.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

void UpdateInfoDatabase::register_update(
		std::string program_name,
		std::string variant_name,
		version_number version_source,
		version_number version_target,
		uint64_t size,
		update_signature signature) {

	const auto lock = std::unique_lock(mutex);

	auto& program_info = programs[program_name];
	auto& variant_info = program_info.variants[variant_name];
	auto& source_version_info = variant_info.versions[version_source];
	auto& target_version_info = variant_info.versions[version_target];

	source_version_info.program = program_name;
	source_version_info.variant = variant_name;
	source_version_info.version = version_source;

	target_version_info.program = program_name;
	target_version_info.variant = variant_name;
	target_version_info.version = version_target;

	source_version_info.update.emplace(
			&target_version_info,
			size,
			signature
	);
}

void UpdateInfoDatabase::debug_dump() {
	const auto lock = std::unique_lock(mutex);

	log_update.info("Printing update database of {} program...", programs.size());
	for (const auto& [key, value] : programs) {
		log_update.info("\tProgram {} has {} variant:", key, value.variants.size());
		for (const auto& [key, value] : value.variants) {
			log_update.info("\t\tVariant {} has {} version:", key, value.versions.size());
			for (const auto& [key, value] : value.versions) {
				if (value.update)
					log_update.info("\t\t\t{} -> {}, size: {}, signature: {}", libv::to_value(value.version), libv::to_value(value.update->target->version), value.update->size, libv::to_value(value.update->signature));
				else
					log_update.info("\t\t\t{} has no updates", libv::to_value(value.version));
			}
		}
	}
}

update_route_variant UpdateInfoDatabase::get_update_route(std::string program_name, std::string variant_name, version_number version) {
	const auto lock = std::unique_lock(mutex);

	const auto pit = programs.find(program_name);
	if (pit == programs.end())
		return msg::VersionNotSupported{};

	const auto& variants = pit->second.variants;
	const auto vit = variants.find(variant_name);
	if (vit == variants.end())
		return msg::VersionNotSupported{};

	const auto& versions = vit->second.versions;
	const auto eit = versions.find(version);
	if (eit == versions.end())
		return msg::VersionNotSupported{};

	const auto& version_info = eit->second;
	if (!version_info.update)
		return msg::VersionUpToDate{};

	msg::UpdateRoute response;
	response.program = program_name;
	response.variant = variant_name;

	for (auto it = &eit->second; it->update; it = it->update->target) {
		response.updates.emplace_back(
				it->version,
				it->update->target->version,
				it->update->size,
				it->update->signature
		);
	}

	return response;
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
