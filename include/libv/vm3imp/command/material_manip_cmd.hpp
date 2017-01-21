// File: material_manip.hpp, Created on 2015. március 27. 2:38, Author: Vader

#pragma once

// pro
#include <libv/vm3imp/console.hpp>

namespace libv {
namespace vm3 {

struct CommandMaterialList : public Command {
	CommandMaterialList();
	virtual void execute() override;
};

struct CommandMaterialPropertyList : public Command {
	Argument<std::string> materialName;
	CommandMaterialPropertyList();
	virtual void execute() override;
};

struct CommandMaterialPropertyRemove : public Command {
	Argument<std::string> materialName;
	Argument<std::string> propertyName;
	CommandMaterialPropertyRemove();
	virtual void execute() override;
};

struct CommandMaterialPropertySet : public Command {
	Argument<std::string> materialName;
	Argument<std::string> propertyName;
	Argument<std::string> value;
	CommandMaterialPropertySet();
	virtual void execute() override;
};

} // namespace vm3
} // namespace libv