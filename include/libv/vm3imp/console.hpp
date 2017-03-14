// File: console.hpp, Created on 2015. március 27. 2:41, Author: Vader

#pragma once

// std
#include <deque>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace libv {
namespace vm3 {

std::vector<std::string> splitCommandLine(const std::string& line);

class ArgumentBase {
	friend class Console;
	std::string description;
public:
	ArgumentBase(const std::string& description);
	virtual void setValue(const std::string& value) = 0;
	virtual void reset() = 0;
};

template <typename T>
class Argument : public ArgumentBase {
	T _value;
	bool _set = false;
public:
	Argument(const std::string& argument) : ArgumentBase(argument) { }
	virtual void setValue(const std::string& newValue) override {
		_set = true;
		std::stringstream(newValue) >> _value;
	}
	const T& value() const {
		return _value;
	}
	bool set() const {
		return _set;
	}
	void reset() override {
		_value = T();
		_set = false;
	}
};

template <>
class Argument<std::string> : public ArgumentBase {
	std::string _value;
	bool _set = false;
public:
	Argument(const std::string& argument) : ArgumentBase(argument) { }
	virtual void setValue(const std::string& newValue) override {
		_set = true;
		_value = newValue;
	}
	const std::string& value() const {
		return _value;
	}
	bool set() const {
		return _set;
	}
	void reset() override {
		_value = "";
		_set = false;
	}
};

class Flag {
	friend class Console;
	std::string description;
	std::vector<std::string> aliases;
	std::vector<ArgumentBase*> args;
	bool _set = false;
public:
	template <typename... Args>
	Flag(const std::vector<std::string>& aliases, const std::string& description, Args&... args) :
		description(description),
		aliases(aliases),
		args {
		&args...
	}
	{
	}
	bool set() const;
	void reset();
};

class Command {
	friend class Console;
	std::vector<std::string> aliases;
	std::vector<ArgumentBase*> args;
	std::vector<Flag*> flags;
protected:
	void registerAlias(const std::string& alias);
	void registerArgument(ArgumentBase& arg);
	void registerFlag(Flag& flag);
public:
	virtual void execute() = 0;
	void reset();
};

class Console {
	std::vector<std::unique_ptr<Command>> commands;
private:
	Flag* findFlag(Command* command, const std::string& searchedFlag);
	Command* findCommand(const std::string& cmd);
public:
	template <typename T>
	void add() {
		commands.emplace_back(new T());
	}
	void execute(const std::string& cmd);
};

} // namespace vm3
} // namespace libv