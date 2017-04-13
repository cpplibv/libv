// File: import_cmd.cpp, Created on 2015. március 27. 5:16, Author: Vader

// hpp
#include <libv/vm3imp/command/import_cmd.hpp>
// ext
#include <libv/serialization/archive/portable_oarchive.hpp>
//#include <libv/serialization/archive/portable_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
//#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
// libv
//#include <libv/log.hpp>
//#include <libv/string.hpp>
#include <libv/read_file.hpp>
// std
#include <iostream>
#include <fstream>
// pro
#include <libv/vm3/importer.hpp>
//#include <libv/vm3/log.hpp>
#include <libv/vm3/serialization/model.hpp>
#include <libv/vm3imp/console.hpp>
#include <libv/vm3imp/opened_model.hpp>


namespace libv {
namespace vm3 {

CommandOpen::CommandOpen() :
	filePath("Input file name") {
	registerAlias("o");
	registerAlias("open");
	registerArgument(filePath);
}

void CommandOpen::execute() {
	openedModel.reset(new Model());
	auto data = libv::read_file(filePath.value());
	openedModel->load(data.data(), data.size());
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
		oar << LIBV_NVP_NAMED("model", *openedModel);
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
		oar << LIBV_NVP_NAMED("model", *openedModel);
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

	libv::vm3::Model model;
	libv::vm3::Importer::import(model, filePath.value());
	{
		boost::filesystem::path path(filePath.value());
		path.replace_extension(".vm3.xml");
		std::ofstream ofs(path.string());
		boost::archive::xml_oarchive oar(ofs);
		oar << LIBV_NVP(model);
	}
	{
		boost::filesystem::path path(filePath.value());
		path.replace_extension(".vm3");
		std::ofstream ofs(path.string(), std::ios_base::binary);
		libv::archive::portable_oarchive oar(ofs);
		oar << LIBV_NVP(model);
	}
}

} // namespace vm3
} // namespace libv