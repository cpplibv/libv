// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
//#include <boost/serialization/serialization.hpp>
//#include <boost/archive/portable_iarchive.hpp>
//#include <boost/archive/portable_oarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
// libv
#include <libv/read_file.hpp>
// std
#include <iostream>
#include <sstream>
#include <fstream>
// pro
//#include <libv/vm3/material.hpp>
//#include <libv/vm3/material_property_visitor.hpp>
//#include <libv/vm3/model.hpp>
#include <libv/vm3/serialization/model.hpp>

using namespace libv::vm3;

int main(int, char **) {
	std::cout << libv::log;

	Model model;

	Material material1("Metal", "Phong-E-v2");
	material1.set("test", 42);
	material1.set("testf", 42.1f);
	material1.set("testd", 42.2);
	material1.set("victor", libv::dvec3(4, 3, 543.32));
	material1.set("zzz", 0xffff);
	material1.set<std::string>("diffuseTexture", "diffuseTexture.dds");
	material1.set<std::string>("emissionTexture", "emissionTexture.dds");

	model.addMaterial(material1);

	//	model.materials.emplace_back(material1);
	//	model.entries.emplace_back();
	//	model.entries.emplace_back();
	//	model.entries[0].baseIndex = 0;
	//	model.entries[0].baseVertex = 0;
	//	model.entries[0].material = &model.materials[0];
	//	model.entries[0].numIndices = 0;
	//	model.entries[0].visibility = true;
	//	model.entries[1].material = &model.materials[0];

	{
		std::ofstream ofs("test_file_xml");
		boost::archive::xml_oarchive oar(ofs);
		oar << LIBV_NVP(model);
	}
	{
		std::ifstream ifs("test_file_xml");
		boost::archive::xml_iarchive iar(ifs);
		iar >> LIBV_NVP(model);
	}
	{
		std::ofstream ofs("test_file_bin", std::ios_base::binary);
		model.save(ofs);
	}
	{
		auto data = libv::read_file("test_file_bin");
		model.load(data.data(), data.size());
	}

	MaterialPropertyPrinter printer;
	model.materials[0].accept(printer);

	return 0;
}


