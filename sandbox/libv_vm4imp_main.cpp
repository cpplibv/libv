// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
#include <fmt/printf.h>
// libv
#include <libv/serialization/archive/binary.hpp>
#include <libv/serialization/archive/json.hpp>
#include <libv/serialization/types/std_optional.hpp>
// std
#include <iostream>
#include <sstream>
#include <fstream>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4/serialization/model.hpp>
#include <libv/vm4imp/importer.hpp>


// -------------------------------------------------------------------------------------------------

int main(int, char **) {
	std::cout << libv::logger;

	const auto model = libv::vm4::import("res/model/fighter_01_eltanin.dae");

	{
		std::ofstream ofs("model_test_file.json.vm4", std::ios::out | std::ios::binary);
		libv::archive::JSONOutput oar(ofs);
		oar << LIBV_NVP_NAMED("model", model);
	}
	{
		std::ofstream ofs("model_test_file.vm4", std::ios::out | std::ios::binary);
		libv::archive::BinaryOutput oar(ofs);
		oar << LIBV_NVP_NAMED("model", *model);
	}

	for (const auto& material : model->materials) {
		fmt::print("name: {}\n", material.name);
		fmt::print("\t{:20}: {}\n", "shader", material.shader);
		for (const auto& [property, value] : material.properties) {
			std::visit([&](const auto& element) {
				fmt::print("\t{:20}: {}\n", property, element);
			}, value);
		}
	}

	return 0;
}
