// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
#include <cereal/archives/json.hpp>
// libv
#include <libv/serialization/archive/cereal_binary_portable.hpp>
#include <libv/serialization/types/std_optional.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <iostream>
#include <sstream>
#include <fstream>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4/serialization/model.hpp>
#include <libv/vm4imp/importer.hpp>


#include <cereal/types/vector.hpp>
// -------------------------------------------------------------------------------------------------

int main(int, char **) {
	std::cout << libv::log;

	const auto model = libv::vm4::import("fighter_01_eltanin.dae");

	{
		std::ofstream ofs("model_test_file.json.vm4", std::ios::out | std::ios::binary);
		cereal::JSONOutputArchive oar(ofs);
		oar << cereal::make_nvp<cereal::JSONOutputArchive>("model", model);
	}
	{
		std::ofstream ofs("model_test_file.vm4", std::ios::out | std::ios::binary);
		libv::archive::CerealPortableBinaryOutput oar(ofs);
		oar << cereal::make_nvp<libv::archive::CerealPortableBinaryOutput>("model", *model);
	}

	std::cout << std::endl;
//	{
//		std::ofstream ofs("test_file_xml");
//		boost::archive::xml_oarchive oar(ofs);
//		oar << LIBV_NVP(model);
//	}
//	{
//		std::ifstream ifs("test_file_xml");
//		boost::archive::xml_iarchive iar(ifs);
//		iar >> LIBV_NVP(model);
//	}
//	{
//		std::ofstream ofs("test_file_bin", std::ios_base::binary);
//		model.save(ofs);
//	}
//	{
//		auto data = libv::read_file("test_file_bin");
//		model.load(data.data(), data.size());
//	}
//
//	MaterialPropertyPrinter printer;
//	model.materials[0].accept(printer);

	return 0;
}


