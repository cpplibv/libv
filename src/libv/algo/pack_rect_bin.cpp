// Project: libv.algorithm, File: src/libv/algorithm/pack_rect_bin.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/algo/pack_rect_bin.hpp>
// std
#include <memory>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// These bin dimensions stay fixed during the whole packing process, i.e. to change the bin size,
/// the packing must be restarted again with a new packing object.
pack_rect_bin::pack_rect_bin(libv::vec2i size) noexcept :
	bin_size(size) {
	root.left = nullptr;
	root.right = nullptr;
	root.position = libv::vec2i(0, 0);
	root.size = size;
}

///	Recursively calls itself.
size_t pack_rect_bin::used_area(const Node& node) const {
	if (node.left || node.right) {
		size_t usedSurfaceArea = node.size.x * node.size.y;
		if (node.left)
			usedSurfaceArea += used_area(*node.left);
		if (node.right)
			usedSurfaceArea += used_area(*node.right);

		return usedSurfaceArea;
	}

	// This is a leaf node, it doesn't constitute to the total surface area.
	return 0;
}

///	Running time is linear to the number of rectangles already packed. Recursively calls itself.
///	@return 0 If the insertion didn't succeed.
pack_rect_bin::Node* pack_rect_bin::insert(Node& node, int32_t width, int32_t height) {
	// If this node is an internal node, try both leaves for possible space.
	// (The rectangle in an internal node stores used space, the leaves store free space)
	if (node.left || node.right) {
		if (node.left) {
			Node* newNode = insert(*node.left, width, height);
			if (newNode)
				return newNode;
		}
		if (node.right) {
			Node* newNode = insert(*node.right, width, height);
			if (newNode)
				return newNode;
		}
		return nullptr; // Didn't fit into either subtree!
	}

	// This node is a leaf, but can we fit the new rectangle here?
	if (width > node.size.x || height > node.size.y)
		return nullptr; // Too bad, no space.

	// The new cell will fit, split the remaining space along the shorter axis,
	// that is probably more optimal.
	int32_t w = node.size.x - width;
	int32_t h = node.size.y - height;
	node.left = std::make_unique<Node>();
	node.right = std::make_unique<Node>();

	if (w <= h) { // Split the remaining space in horizontal direction.
		node.left->position.x = node.position.x + width;
		node.left->position.y = node.position.y;
		node.left->size.x = w;
		node.left->size.y = height;

		node.right->position.x = node.position.x;
		node.right->position.y = node.position.y + height;
		node.right->size.x = node.size.x;
		node.right->size.y = h;

	} else { // Split the remaining space in vertical direction.
		node.left->position.x = node.position.x;
		node.left->position.y = node.position.y + height;
		node.left->size.x = width;
		node.left->size.y = h;

		node.right->position.x = node.position.x + width;
		node.right->position.y = node.position.y;
		node.right->size.x = w;
		node.right->size.y = node.size.y;
	}

	// Note that as a result of the above, it can happen that node.left or node.right
	// is now a degenerate (zero area) rectangle. No need to do anything about it,
	// like remove the nodes as "unnecessary" since they need to exist as children of
	// this node (this node can't be a leaf anymore).

	// This node is now a non-leaf, so shrink its area - it now denotes
	// *occupied* space instead of free space. Its children spawn the resulting
	// area of free space.
	node.size.x = width;
	node.size.y = height;
	return &node;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
