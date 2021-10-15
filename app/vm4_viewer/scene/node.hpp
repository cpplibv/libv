// Project: libv, File: app/vm4_viewer/scene/node.hpp

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
