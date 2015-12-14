// File: vm3_importer.cpp, Created on 2015. február 18. 18:22, Author: Vader

// vl
#include <vl/log.hpp>
// std
#include <iostream>
// pro
#include <vl/vm3imp/console.hpp>
#include <vl/vm3imp/command/import_cmd.hpp>
#include <vl/vm3imp/command/material_manip_cmd.hpp>

using namespace vl;

int main(int argc, char** argv) {
	vl::log().output(std::cout);

	std::string line;
	for (int i = 1; i < argc; i++)
		line += std::string(" \"") + argv[i] + "\"";

	vm3::Console console;
	console.add<vm3::CommandImport>();
	console.add<vm3::CommandMaterialList>();
	console.add<vm3::CommandMaterialPropertyList>();
	console.add<vm3::CommandMaterialPropertyRemove>();
	console.add<vm3::CommandMaterialPropertySet>();
	console.add<vm3::CommandOpen>();
	console.add<vm3::CommandSave>();
	console.add<vm3::CommandSaveAs>();

	if (!line.empty()) {
		console.execute(line);
		return 0;
	}

	std::cout << "Copyright (c) 2015 Csa'sza'r Ma'tya's \n";
	std::cout << "Vader Model 3 - Importer v1.1\n";
	std::cout << "All rights reserved.\n" << std::endl;

	while (true) {
		std::getline(std::cin, line);
		if (line == "exit" || line == "end" || line == "quit" || line == "stop" || line == "q")
			break;

		console.execute(line);
	}

	return 0;
}

//Open vm3
//material list
//material property list
//material property set
//material property get
