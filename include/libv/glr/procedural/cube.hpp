// File: sphere.hpp Author: Vader Created on January 28, 2017, 3:22 PM

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

// TODO P5: Possible to make a Triangle Strip version
template <typename Precision = float, typename Position, typename Normal, typename Texture0, typename Index>
void generateCube(Position& position, Normal& normal, Texture0& texture0, Index& index) {
	using in = uint32_t;
	using vec2 = libv::vec2_t<Precision>;
	using vec3 = libv::vec3_t<Precision>;

	const in ioffset = static_cast<in>(position.size());

	static constexpr std::array data_position = {
		vec3{-1, -1,  1}, vec3{ 1, -1,  1}, vec3{ 1,  1,  1}, vec3{-1,  1,  1},
		vec3{ 1,  1, -1}, vec3{ 1, -1, -1}, vec3{-1, -1, -1}, vec3{-1,  1, -1},
		vec3{ 1,  1,  1}, vec3{ 1,  1, -1}, vec3{-1,  1, -1}, vec3{-1,  1,  1},
		vec3{-1, -1, -1}, vec3{ 1, -1, -1}, vec3{ 1, -1,  1}, vec3{-1, -1,  1},
		vec3{ 1, -1, -1}, vec3{ 1,  1, -1}, vec3{ 1,  1,  1}, vec3{ 1, -1,  1},
		vec3{-1,  1,  1}, vec3{-1,  1, -1}, vec3{-1, -1, -1}, vec3{-1, -1,  1},
	};

	static constexpr std::array data_normal = {
		vec3{ 0,  0,  1}, vec3{ 0,  0,  1}, vec3{ 0,  0,  1}, vec3{ 0,  0,  1},
		vec3{ 0,  0, -1}, vec3{ 0,  0, -1}, vec3{ 0,  0, -1}, vec3{ 0,  0, -1},
		vec3{ 0,  1,  0}, vec3{ 0,  1,  0}, vec3{ 0,  1,  0}, vec3{ 0,  1,  0},
		vec3{ 0, -1,  0}, vec3{ 0, -1,  0}, vec3{ 0, -1,  0}, vec3{ 0, -1,  0},
		vec3{ 1,  0,  0}, vec3{ 1,  0,  0}, vec3{ 1,  0,  0}, vec3{ 1,  0,  0},
		vec3{-1,  0,  0}, vec3{-1,  0,  0}, vec3{-1,  0,  0}, vec3{-1,  0,  0},
	};

	static constexpr std::array data_texture0 = {
		vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{0, 1},
		vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{0, 1},
		vec2{1, 1}, vec2{1, 0}, vec2{0, 0}, vec2{0, 1},
		vec2{1, 1}, vec2{1, 0}, vec2{0, 0}, vec2{0, 1},
		vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{0, 1},
		vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{0, 1},
	};

	static constexpr std::array data_index = {
		in{ 0}, in{ 1}, in{ 2}, in{ 2}, in{ 3}, in{ 0},
		in{ 4}, in{ 5}, in{ 6}, in{ 6}, in{ 7}, in{ 4},
		in{ 8}, in{ 9}, in{10}, in{10}, in{11}, in{ 8},
		in{12}, in{13}, in{14}, in{14}, in{15}, in{12},
		in{16}, in{17}, in{18}, in{18}, in{19}, in{16},
		in{20}, in{21}, in{22}, in{22}, in{23}, in{20},
	};

	position(data_position);
	normal(data_normal);
	texture0(data_texture0);
	index(data_index);

	// Adjust indices if geometry was non-empty
	if (ioffset != 0)
		for (auto& i : index.view_last(data_index.size()))
			i += ioffset;
}

template <typename Precision = float, typename Position, typename Index>
void generateInnerCube(Position& position, Index& index) {
	using in = uint32_t;
	using vec3 = libv::vec3_t<Precision>;

	const in ioffset = static_cast<in>(position.size());

	static constexpr std::array data_position = {
		vec3{-1, -1,  1}, vec3{ 1, -1,  1}, vec3{ 1,  1,  1}, vec3{-1,  1,  1},
		vec3{ 1,  1, -1}, vec3{ 1, -1, -1}, vec3{-1, -1, -1}, vec3{-1,  1, -1},
		vec3{ 1,  1,  1}, vec3{ 1,  1, -1}, vec3{-1,  1, -1}, vec3{-1,  1,  1},
		vec3{-1, -1, -1}, vec3{ 1, -1, -1}, vec3{ 1, -1,  1}, vec3{-1, -1,  1},
		vec3{ 1, -1, -1}, vec3{ 1,  1, -1}, vec3{ 1,  1,  1}, vec3{ 1, -1,  1},
		vec3{-1,  1,  1}, vec3{-1,  1, -1}, vec3{-1, -1, -1}, vec3{-1, -1,  1},
	};

	static constexpr std::array data_index = {
		in{ 0}, in{ 2}, in{ 1}, in{ 2}, in{ 0}, in{ 3},
		in{ 4}, in{ 6}, in{ 5}, in{ 6}, in{ 4}, in{ 7},
		in{ 8}, in{10}, in{ 9}, in{10}, in{ 8}, in{11},
		in{12}, in{14}, in{13}, in{14}, in{12}, in{15},
		in{16}, in{18}, in{17}, in{18}, in{16}, in{19},
		in{20}, in{22}, in{21}, in{22}, in{20}, in{23},
	};

	position(data_position);
	index(data_index);

	// Adjust indices if geometry was non-empty
	if (ioffset != 0)
		for (auto& i : index.view_last(data_index.size()))
			i += ioffset;
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
