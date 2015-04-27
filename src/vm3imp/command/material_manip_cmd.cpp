// File: material_manip.cpp, Created on 2015. március 27. 2:38, Author: Vader

// hpp
#include "vl/vm3imp/command/material_manip_cmd.hpp"
// std
#include <fstream>
// pro
#include "vl/vm3/model.hpp"
#include "vl/vm3imp/opened_model.hpp"

namespace vl {
namespace vm3 {

//  -------------------------------------------------------------------------------------------------

CommandMaterialList::CommandMaterialList() {
	registerAlias("lm");
	registerAlias("list_material");
}

void CommandMaterialList::execute() {
	if (!openedModel) {
		std::cout << "No model opened." << std::endl;
		return;
	}

	for (auto& elem : openedModel->materials) {
		std::cout << elem.getName() << std::endl;
	}
}

//  -------------------------------------------------------------------------------------------------

CommandMaterialPropertyList::CommandMaterialPropertyList() :
	materialName("Material name") {
	registerAlias("lmp");
	registerAlias("list_material_property");
	registerArgument(materialName);
}

void CommandMaterialPropertyList::execute() {
	if (!openedModel) {
		std::cout << "No model opened." << std::endl;
		return;
	}

	// TODO: mat not found
	for (auto& elem : openedModel->materials) {
		if (elem.getName() == materialName.value()) {
			MaterialPropertyPrinter printer;
			elem.accept(printer);
		}
	}
}

//  -------------------------------------------------------------------------------------------------

CommandMaterialPropertyRemove::CommandMaterialPropertyRemove() :
	materialName("Material name"),
	propertyName("Property name") {
	registerAlias("rmp");
	registerAlias("remove_material_property");
	registerArgument(materialName);
	registerArgument(propertyName);
}

void CommandMaterialPropertyRemove::execute() {
	if (!openedModel) {
		std::cout << "No model opened." << std::endl;
		return;
	}
	// TODO: mat not found
	for (auto& elem : openedModel->materials) {
		if (elem.getName() == materialName.value()) {
			elem.remove(propertyName.value());
		}
	}
}

//  -------------------------------------------------------------------------------------------------

CommandMaterialPropertySet::CommandMaterialPropertySet() :
	materialName("Material name"),
	propertyName("Property name"),
	value("Value") {
	registerAlias("smp");
	registerAlias("set_material_property");
	registerArgument(materialName);
	registerArgument(propertyName);
	registerArgument(value);
}

void CommandMaterialPropertySet::execute() {
	if (!openedModel) {
		std::cout << "No model opened." << std::endl;
		return;
	}
	for (auto& elem : openedModel->materials) {
		if (elem.getName() == materialName.value()) {
			elem.set(propertyName.value(), value.value());
		}
	}
}

//  -------------------------------------------------------------------------------------------------

} // namespace vm3
} // namespace vl