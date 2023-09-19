// Project: libv.re, File: src/libv/re/node/proxy.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/node/node.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Proxy : public Node {
public:
	friend libv::ref_count_access;
	using ptr = Proxy_ptr;
	using cptr = Proxy_cptr;

public:
	Node_ptr targetNode;

public:
	explicit Proxy(Node_ptr targetNode) :
		Node(NodeType::proxy),
		targetNode(std::move(targetNode)) { }
	virtual ~Proxy();
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
