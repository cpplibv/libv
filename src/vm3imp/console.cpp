// File: console.cpp, Created on 2015. március 27. 2:48, Author: Vader

// hpp
#include <vl/vm3imp/console.hpp>
// vl
#include <vl/string.hpp>
// std
#include <iostream>

namespace vl {
namespace vm3 {

// -------------------------------------------------------------------------------------------------

//TODO P3: split line at ;
std::vector<std::string> splitCommandLine(const std::string& line) {
	std::vector<std::string> result;
	bool in_quotes = false;
	std::string curr_word;
	for (size_t i = 0; i < line.size(); i++) {
		char c = line[i];
		char pc = i > 1 ? line[i - 1] : ' ';
		if (in_quotes || (c != ' ')) {
			curr_word.push_back(c);
			in_quotes = (c == '"' && pc != '\\') != in_quotes;
		} else if (!curr_word.empty()) {
			result.push_back(curr_word);
			curr_word.clear();
		}
	}
	if (!curr_word.empty())
		result.push_back(curr_word);

	for (auto& word : result) {
		word = vl::trim(word);
		auto size = word.size();
		if(size > 1 && word[0] == '"' && word[size - 1] == '"')
			word = word.substr(1, size - 2);
		word = vl::trim(word);
	}
	return result;
}

// -------------------------------------------------------------------------------------------------

ArgumentBase::ArgumentBase(const std::string& description) : description(description) { }

// -------------------------------------------------------------------------------------------------

bool Flag::set() const{
	return _set;
}

void Flag::reset() {
	_set = false;
	for (auto& it : args) {
		it->reset();
	}
}


// -------------------------------------------------------------------------------------------------

void Command::registerAlias(const std::string& alias) {
	aliases.push_back(alias);
}

void Command::registerArgument(ArgumentBase& arg) {
	args.push_back(&arg);
}

void Command::registerFlag(Flag& flag) {
	flags.push_back(&flag);
}

void Command::reset() {
	for (auto& it : args) {
		it->reset();
	}
	for (auto& it : flags) {
		it->reset();
	}
}

// -------------------------------------------------------------------------------------------------

Command* Console::findCommand(const std::string& cmd) {
	for (auto& command : commands) {
		for (auto& alias : command->aliases) {
			if (cmd == alias) {
				return &*command;
			}
		}
	}
	return nullptr;
}

Flag* Console::findFlag(Command* command, const std::string& searchedFlag) {
	for (auto& flag : command->flags) {
		for (auto& alias : flag->aliases) {
			if (searchedFlag == alias) {
				return &*flag;
			}
		}
	}
	return nullptr;
}

void Console::execute(const std::string& cmd) {
	std::vector<std::string> line(splitCommandLine(cmd));

	if (line.empty())
		return;
	if (auto match = findCommand(line[0])) {
		for (auto& flag : match->flags) {
			flag->_set = false;
		}

		std::deque<ArgumentBase*> argQue;
		argQue.insert(argQue.begin(), match->args.begin(), match->args.end());
		std::deque<std::string> lineQue;
		lineQue.insert(lineQue.begin(), line.begin(), line.end());
		lineQue.pop_front();

		while (!argQue.empty() || (!lineQue.empty() && lineQue.front()[0] == '-')) {
			if (lineQue.empty()) {
				
				for (auto* arg : argQue) {
					std::cout << "Missing argument: [" << arg->description << "]" << std::endl;
				}
				//return; //TODO: optional argument
				break; //Validate this... most likely breaks something?
			}

			const auto& currentSection = lineQue.front();
			if (currentSection[0] == '-') {
				if (currentSection[1] != '-') { //flag
					for (unsigned int i = 1; i < currentSection.length(); i++) {
						auto currentFlagString = currentSection.substr(i, 1);
						auto* currentFlag = findFlag(match, currentFlagString);
						if (currentFlag) {
							argQue.insert(argQue.begin(), currentFlag->args.begin(), currentFlag->args.end());
							currentFlag->_set = true;
						} else {
							std::cout << "Unknown flag: [" << currentFlagString << "]" << std::endl;
						}
					}
				} else { //long flag
					auto currentFlagString = currentSection.substr(1);
					auto* currentFlag = findFlag(match, currentFlagString);
					if (currentFlag) {
						argQue.insert(argQue.begin(), currentFlag->args.begin(), currentFlag->args.end());
						currentFlag->_set = true;
					} else {
						std::cout << "Unknown flag: [" << currentFlagString << "]" << std::endl;
					}
				}
			} else { //argument
				argQue.front()->setValue(currentSection);
				argQue.pop_front();
			}
			lineQue.pop_front();
		}
		match->execute();
		match->reset();
	} else {
		std::cout << "Unknown command: " << line[0] << std::endl;
	}
}

// -------------------------------------------------------------------------------------------------

} //namespace vm3
} //namespace vl