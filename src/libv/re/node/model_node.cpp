// Project: libv.re, File: src/libv/re/node/model_node.cpp

#include <libv/re/node/model_node.hpp>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(ModelNode* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(ModelNode* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

ModelNode::ModelNode() :
	Node(NodeType::model) {
}

ModelNode::ModelNode(libv::vec3f position) :
	Node(NodeType::model, position) {
}

ModelNode::ModelNode(const Transform& transform) :
	Node(NodeType::model, transform) {
}

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
