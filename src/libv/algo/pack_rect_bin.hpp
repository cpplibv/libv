// Project: libv.algo, File: src/libv/algo/pack_rect_bin.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <memory>


// =================================================================================================
//
// Source: https://blackpawn.com/texts/lightmaps/
// Original implementation: https://github.com/juj/RectangleBinPack/tree/master/old
// For more information, see
// - Rectangle packing: http://www.gamedev.net/community/forums/topic.asp?topic_id=392413
// - Packing lightmaps: http://www.blackpawn.com/texts/lightmaps/default.html
//
// TODO P3: Adopt and implement better packing algorithms https://github.com/juj/RectangleBinPack
//
// =================================================================================================

namespace libv {

// -------------------------------------------------------------------------------------------------

// Performs 'discrete online rectangle packing into a rectangular bin' by maintaining
//	a binary tree of used and free rectangles of the bin. There are several variants
//	of bin packing problems, and this packer is characterized by:
//	- We're solving the 'online' version of the problem, which means that when we're adding
//	  a rectangle, we have no information of the sizes of the rectangles that are going to
//	  be packed after this one.
//	- We are packing rectangles that are not rotated. I.e. the algorithm will not flip
//	  a rectangle of (w,h) to be stored if it were a rectangle of size (h, w). There is no
//	  restriction conserning UV mapping why this couldn't be done to achieve better
//	  occupancy, but it's more work. Feel free to try it out.
//	- The packing is done in discrete integer coordinates and not in rational/real numbers (floats).
//	Internal memory usage is linear to the number of rectangles we've already packed.
//
//	Idea: Instead of just picking the first free rectangle to insert the new rect into,
//	      check all free ones (or maintain a sorted order) and pick the one that minimizes
//			the resulting leftover area. There is no real reason to maintain a tree - in fact
//			it's just redundant structuring. We could as well have two lists - one for free
//			rectangles and one for used rectangles. This method would be faster and might
//			even achieve a considerably better occupancy rate.

class pack_rect_bin {
private:
	///	A node of a binary tree. Each node represents a rectangular area of the texture
	///	we surface. Internal nodes store rectangles of used data, whereas leaf nodes track
	///	rectangles of free space. All the rectangles stored in the tree are disjoint.
	struct Node {
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		libv::vec2i position;
		libv::vec2i size;
	};

private:
	Node root;
	libv::vec2i bin_size; /// The total size of the bin we started with.

public:
	///	Starts a new packing process to a bin of the given dimension.
	[[nodiscard]] explicit pack_rect_bin(libv::vec2i size) noexcept;
	[[nodiscard]] inline pack_rect_bin(int32_t width, int32_t height) noexcept :
		pack_rect_bin(libv::vec2i(width, height)) {}

	///	Inserts a new rectangle of the given size into the bin.
	///	Running time is linear to the number of rectangles that have been already packed.
	///	@return A pointer to the node that stores the newly added rectangle, or nullptr if it didn't fit.
	[[nodiscard]] inline Node* insert(int32_t width, int32_t height) {
		return insert(root, width, height);
	}

	[[nodiscard]] inline Node* insert(libv::vec2i size) {
		return insert(root, size.x, size.y);
	}

	[[nodiscard]] inline size_t used_area() const {
		return used_area(root);
	}

	[[nodiscard]] inline size_t total_area() const {
		return bin_size.x * bin_size.y;
	}

	/// Computes the ratio of used surface area.
	/// @return A value [0, 1] denoting the ratio of total surface area that is in use.
	/// 	0.0f - the bin is totally empty, 1.0f - the bin is full.
	[[nodiscard]] inline float occupancy() const {
		return static_cast<float>(used_area()) / static_cast<float>(total_area());
	}

private:
	/// @return The surface area used by the subtree rooted at node.
	[[nodiscard]] size_t used_area(const Node& node) const;

	/// Inserts a new rectangle in the subtree rooted at the given node.
	Node* insert(Node& node, int32_t width, int32_t height);
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
