// File: import.hpp, Created on 2015. március 27. 3:01, Author: Vader

#pragma once

// pro
#include "vl/vm3imp/console.hpp"

namespace vl {
namespace vm3 {

struct CommandOpen : public Command {
	Argument<std::string> filePath;
	//Flag format flag?
	CommandOpen();
	virtual void execute() override;
};

struct CommandSave : public Command {
	Flag text;
	CommandSave();
	virtual void execute() override;
};

struct CommandSaveAs : public Command {
	Argument<std::string> filePath;
	Flag text;
	//Flag format flag?
	CommandSaveAs();
	virtual void execute() override;
};

struct CommandImport : public Command {
	Argument<std::string> filePath;
	Flag verbose;
	Flag test;
	Argument<std::string> output_file;
	Flag manual_output;

	CommandImport();
	virtual void execute() override;
};

} //namespace vm3
} //namespace vl