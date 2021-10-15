// Project: libv.vm4imp, File: sandbox/libv_vm4imp_main.cpp

// ext
#include <fmt/printf.h>
// libv
#include <libv/algo/wildcard.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/types/std_optional.hpp>
#include <libv/utility/generic_path.hpp>
// std
#include <filesystem>
#include <fstream>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4/model_serial.hpp>
#include <libv/vm4imp/importer.hpp>
#include <libv/vm4imp/log.hpp>


// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	std::cout << libv::logger_stream;

	const auto path = "res/model";
//	const auto filter_pattern = "**.dae";
	const auto filter_pattern = "**.game.fbx";

	for(const auto& p : std::filesystem::recursive_directory_iterator(path)) {
		if (not p.is_regular_file())
			continue;

		const auto gp = libv::generic_path(p);

		if (not libv::match_wildcard_glob(gp, filter_pattern))
			continue;

		libv::vm4::log_vm4.info("Importing {}...", gp);

		const auto model = libv::vm4::import(gp);
		const auto output_path = std::filesystem::path(gp).replace_extension("vm4");

		if (model) {
			libv::vm4::log_vm4.info("Saving    {}...", libv::generic_path(output_path));

			std::ofstream ofs(output_path, std::ios::out | std::ios::binary);
			libv::archive::BinaryOutput oar(ofs);
			oar << LIBV_NVP_NAMED("model", *model);
		} else {
			libv::vm4::log_vm4.error("Failed to import {}", gp);
		}
	}

	return 0;
}
