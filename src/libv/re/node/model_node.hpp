// Project: libv.re, File: src/libv/re/node/model_node.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/node/node.hpp>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

class ModelNode : public Node {
public:
	using ptr = ModelNode_ptr;
	using cptr = ModelNode_cptr;

public:
	ModelNode();
	explicit ModelNode(libv::vec3f position);
	explicit ModelNode(const Transform& transform);
};

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
