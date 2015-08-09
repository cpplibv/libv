// File: import_cmd.cpp, Created on 2015. március 27. 5:16, Author: Vader

// hpp
#include <vl/vm3imp/command/import_cmd.hpp>
// ext
#include <boost/serialization/serialization.hpp>
//#include <boost/archive/portable_iarchive.hpp>
#include <boost/archive/portable_oarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
//#include <boost/archive/xml_iarchive.hpp>
// vl
#include <vl/file_system.hpp>
//#include <vl/log.hpp>
//#include <vl/string.hpp>
// std
#include <iostream>
#include <fstream>
// pro
#include <vl/vm3/importer.hpp>
//#include <vl/vm3/log.hpp>
#include <vl/vm3/serialization/model.hpp>
#include <vl/vm3imp/console.hpp>
#include <vl/vm3imp/opened_model.hpp>

namespace vl {
namespace vm3 {

CommandOpen::CommandOpen() :
	filePath("Input file name") {
	registerAlias("o");
	registerAlias("open");
	registerArgument(filePath);
}

void CommandOpen::execute() {
	openedModel.reset(new Model());
	openedModel->load(std::ifstream(filePath.value(), std::ios::binary));
}

// -------------------------------------------------------------------------------------------------

CommandSave::CommandSave() :
	text({"t", "-text"}, "Text mode") {
	registerAlias("s");
	registerAlias("save");
	registerFlag(text);
}

void CommandSave::execute() {
	if (!openedModel) {
		std::cout << "No model opened." << std::endl;
		return;
	}

	if (text.set()) {
		std::ofstream ofs(openedModel->getName() + ".xml");
		boost::archive::xml_oarchive oar(ofs);
		oar << VL_NVP_NAMED("model", *openedModel);
	} else
		openedModel->save(std::ofstream(openedModel->getName() + ".vm3", std::ios_base::binary));
}

// -------------------------------------------------------------------------------------------------

CommandSaveAs::CommandSaveAs() :
	filePath("Output file name"),
	text({"t", "-text"}, "Text mode") {
	registerAlias("sa");
	registerAlias("saveas");
	registerArgument(filePath);
	registerFlag(text);
}

void CommandSaveAs::execute() {
	if (!openedModel) {
		std::cout << "No model opened." << std::endl;
		return;
	}

	if (text.set()) {
		std::ofstream ofs(filePath.value());
		boost::archive::xml_oarchive oar(ofs);
		oar << VL_NVP_NAMED("model", *openedModel);
	} else
		openedModel->save(std::ofstream(filePath.value(), std::ios_base::binary));

}

// -------------------------------------------------------------------------------------------------

CommandImport::CommandImport() :
	filePath("Input file name"),
	verbose({"v", "-verbose"}, "Verbose mode"),
test({"t", "-test"}, "Test mode"),
output_file("Output file name"),
manual_output({"o", "-output"}, "Output file name", output_file) {
	registerAlias("import");
	registerAlias("imp");
	registerAlias("vm3imp");
	registerArgument(filePath);
	registerFlag(verbose);
	registerFlag(test);
	registerFlag(manual_output);
}

void CommandImport::execute() {
	if (verbose.set()) {
		std::cout << "Importing model from file: " << filePath.value() << std::endl;
	}

	vl::vm3::Model model;
	vl::vm3::Importer::import(model, filePath.value());
	{
		std::ofstream ofs(vl::file_path::removeExtension(filePath.value()) + ".vm3.xml");
		boost::archive::xml_oarchive oar(ofs);
		oar << VL_NVP(model);
	}
	{
		std::ofstream ofs(vl::file_path::removeExtension(filePath.value()) + ".vm3", std::ios_base::binary);
		eos::portable_oarchive oar(ofs);
		oar << VL_NVP(model);
	}
}

} //namespace vm3
} //namespace vl