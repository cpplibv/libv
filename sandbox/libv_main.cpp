// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

//#include <algorithm>
//#include <boost/container/small_vector.hpp>
//#include <boost/fusion/adapted/std_pair.hpp>
//#include <boost/fusion/adapted/std_tuple.hpp>
//#include <boost/fusion/include/adapt_struct.hpp>
//#include <boost/spirit/home/x3.hpp>
//#include <boost/spirit/home/x3/support/ast/variant.hpp>
//#include <boost/variant.hpp>
//#include <boost/container/flat_map.hpp>
//#include <boost/container/flat_set.hpp>
//#include <boost/container/small_vector.hpp>
//#include <chrono>
//#include <ctime>
//#include <fstream>
//#include <functional>
//#include <libv/parse/color.hpp>
//#include <libv/parse/text_tag.hpp>
//#include <libv/range/view_uft8_codepoints.hpp>
//#include <map>
//#include <memory>
//#include <range/v3/algorithm/copy.hpp>
//#include <range/v3/algorithm/for_each.hpp>
//#include <range/v3/all.hpp>
//#include <range/v3/core.hpp>
//#include <range/v3/view/reverse.hpp>
//#include <set>
//#include <string>
//#include <unordered_map>
//#include <utility>
//#include <vector>

//http://ciere.com/cppnow15/x3_docs/index.html
//http://ciere.com/cppnow15/x3_docs/spirit/tutorials/semantic_actions.html
//
//	std::string in = ""
//			"hjt"
//			"[color=1]"
//			"f\\e\\[col\\or=25]"
//			"[color=fe]"
//			"[/tag]"
//			"2gre"
//			"[/tag]"
//			"gre [non_tag]4\\5°t\\\\r"
//			"[tag]"
//			"freḼơᶉëᶆ ȋṕšᶙṁ ḍ"
//			"[tag=\\\"gre\\\"]"
//			"[tag=]"
//			"ỡḽǭᵳ ʂǐť ӓṁệẗ, ĉṓɲṩḙċťᶒ"
//			"[tag=\"țûɾ ấɖḯƥĭṩčįɳ]ġ >\\]<ḝłįʈ, \\\\]șếᶑ] \"]"
//			"[tag=\"țûɾ ấɖḯƥĭṩčįɳ]ġ \"ᶁⱺ ẽḭŭŝḿꝋď ]ṫĕᶆᶈṓɍ ỉñḉīḑȋᵭṵńť ṷŧ ḹẩḇőꝛế éȶ đôȇ ᵯáᶇā ąⱡîɋṹẵ.";
//
////	for (const auto& ch : in | libv::view::uft8_codepoints) {
////		std::cout << ch << ' ';
////	}
//
//	const auto parsed_values = libv::parse::parseTextTag(in);
//
//	for (const auto& v : parsed_values) {
//		std::cout << (!v.close ? " tag open: " : "tag close: ") << v.name << (v.value.empty() ? "" : " = ") << v.value << '\n';
//	}

//#include <iostream>
//#include <glm/mat4x4.hpp>
//#include <glm/detail/precision.hpp>


//struct vec4
//{
//	union
//	{
//		struct { float x, y, z, w;};
//		struct { float r, g, b, a; };
//		struct { float s, t, p, q; };
//
//		float data[4];
////		typename glm::detail::storage<float, sizeof(float) * 4, glm::detail::is_aligned<P>::value>::type data;
//	};
//
//	float& operator[](size_t i) { return data[i]; }
//	const float& operator[](size_t i) const { return data[i]; }
//
//	vec4(){}
//	vec4(float a, float b, float c, float d) {
//		x = a;
//		y = b;
//		z = c;
//		w = d;
//	}
//
//	// -- Unary arithmetic operators --
//
//	vec4 & operator=(vec4 const & v) {
//		x = v.x;
//		y = v.y;
//		z = v.z;
//		w = v.w;
//		return *this;
//	}
//
//	vec4 operator*(float v) const {
//		return vec4{x * v, y*v, z*v, w * v};
//	}
//	vec4 operator+(vec4 v) const {
//		return vec4{x * v.x, y*v.y, z*v.z, w * v.w};
//	}
//
//};
//
//struct mat4
//{
//
//private:
//	vec4 value[4];
//
//public:
//	// -- Constructors --
//	mat4() {};
//	mat4(mat4 const & m) = default;
//
//	explicit mat4(float const & x);
//	mat4(
//		float const & x0, float const & y0, float const & z0, float const & w0,
//		float const & x1, float const & y1, float const & z1, float const & w1,
//		float const & x2, float const & y2, float const & z2, float const & w2,
//		float const & x3, float const & y3, float const & z3, float const & w3) {
//
//		this->value[0] = vec4(x0, y0, z0, w0);
//		this->value[1] = vec4(x1, y1, z1, w1);
//		this->value[2] = vec4(x2, y2, z2, w2);
//		this->value[3] = vec4(x3, y3, z3, w3);
//	}
////	mat4(
////		vec4 const & v0,
////		vec4 const & v1,
////		vec4 const & v2,
////		vec4 const & v3);
//
//	// -- Accesses --
//
//	vec4 const & operator[](size_t i) const {
//		return value[i];
//	}
//
//	vec4 & operator[](size_t i) {
//		return value[i];
//	}
//
//	mat4 & operator*=(mat4 const & m);
//};
//
//mat4 operator*(mat4 const & m1, mat4 const & m2)
//{
//	vec4 const SrcA0 = m1[0];
//	vec4 const SrcA1 = m1[1];
//	vec4 const SrcA2 = m1[2];
//	vec4 const SrcA3 = m1[3];
//
//	vec4 const SrcB0 = m2[0];
//	vec4 const SrcB1 = m2[1];
//	vec4 const SrcB2 = m2[2];
//	vec4 const SrcB3 = m2[3];
//
//	mat4 Result;
//	Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
//	Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
//	Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
//	Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
//	return Result;
//}
//
//mat4 & mat4::operator*=(mat4 const & m) {
//	return (*this = *this * m);
//}
//
//
//
//void foo(mat4& matptr) {
//	mat4 trans{
//		1,0,0,0,
//		0,1,0,0,
//		0,0,1,0,
//		1,0,0,1
//	};
//	matptr *= trans;
//
//	float a = (matptr)[3][0];
//
//	std::cout << std::endl; // or just: asm volatile ("" : : : "memory");
//
//	float b = (matptr)[3][0];
//
//	std::cout << a << b << std::endl;
//}
//
//int main() {
//	mat4 x{
//		1,0,0,0,
//		0,1,0,0,
//		0,0,1,0,
//		0,0,0,1
//	};
//	foo(x);
//
//	return 0;
//}

#include <iostream>
#include <glm/mat4x4.hpp>

void foo(glm::mat4& mat) {
	glm::mat4 trans{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 0, 0, 1};
	mat *= trans;

	float a = mat[3][0]; // a = 0

	asm volatile ("" : : : "memory"); // or just use anything that can serve as a memory barrier like: std::cout << std::endl

	float b = mat[3][0]; // b = 1

	std::cout << a << b << std::endl;
}

int main() {
	glm::mat4 x{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1};
	foo(x);

	return 0;
}
