// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
//#include <libv/math/vec.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Node {
	std::string name;
//	Node** children;
	std::vector<std::unique_ptr<Node>> children;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
