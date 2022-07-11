// Project: libv.math, File: src/libv/math/vec_concept.hpp

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Two dimensional mathematical vector concept. Types with x, y member variables
/// @param Type - The Type to be tested
template <typename Vector>
concept Vec2 = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), decltype(vec.y)>;
	requires sizeof(vec) == 2 * sizeof(decltype(vec.x));
	requires &vec.x < &vec.y;
};

/// Two dimensional mathematical vector concept. Types with x, y member variables
/// @param Type - The Type to be tested
/// @param Element - Specify the Element's Type
template <typename Vector, typename Element>
concept Vec2Type = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), Element>;
	requires std::is_same_v<decltype(vec.y), Element>;
	requires sizeof(vec) == 2 * sizeof(Element);
	requires &vec.x < &vec.y;
};

/// Three dimensional mathematical vector concept. Types with x, y, z member variables
/// @param Type - The Type to be tested
template <typename Vector>
concept Vec3 = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), decltype(vec.y)>;
	requires std::is_same_v<decltype(vec.x), decltype(vec.z)>;
	requires sizeof(vec) == 3 * sizeof(decltype(vec.x));
	requires &vec.x < &vec.y;
	requires &vec.y < &vec.z;
};

/// Three dimensional mathematical vector concept. Types with x, y, z member variables
/// @param Type - The Type to be tested
/// @param Element - Specify the Element's Type
template <typename Vector, typename Element>
concept Vec3Type = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), Element>;
	requires std::is_same_v<decltype(vec.y), Element>;
	requires std::is_same_v<decltype(vec.z), Element>;
	requires sizeof(vec) == 3 * sizeof(Element);
	requires &vec.x < &vec.y;
	requires &vec.y < &vec.z;
};

/// Four dimensional mathematical vector concept. Types with x, y, z, w as member variables
/// @param Type - The Type to be tested
template <typename Vector>
concept Vec4 = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), decltype(vec.y)>;
	requires std::is_same_v<decltype(vec.x), decltype(vec.z)>;
	requires std::is_same_v<decltype(vec.x), decltype(vec.w)>;
	requires sizeof(vec) == 4 * sizeof(decltype(vec.x));
	requires &vec.x < &vec.y;
	requires &vec.y < &vec.z;
	requires &vec.z < &vec.w;
};

/// Four dimensional mathematical vector concept. Types with x, y, z, w as member variables
/// @param Type - The Type to be tested
/// @param Element - Specify the Element's Type
template <typename Vector, typename Element>
concept Vec4Type = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), Element>;
	requires std::is_same_v<decltype(vec.y), Element>;
	requires std::is_same_v<decltype(vec.z), Element>;
	requires sizeof(vec) == 4 * sizeof(Element);
	requires &vec.x < &vec.y;
	requires &vec.y < &vec.z;
	requires &vec.z < &vec.w;
};

// -------------------------------------------------------------------------------------------------

///// N dimensional mathematical vector concept. Types with x, y as member variables but without z or w as members
///// @param Type - The Type to be tested
///// @param Dimension - Number of dimension in the vec
///// @param Element - Specify the Element's Type
//template <typename Vector, std::size_t N, typename Element = void>
//concept Vec = requires(Vector vec) {
//		requires std::is_void_v<Element> || std::is_same_v<decltype(vec[0]), Element>;
//		requires sizeof(vec) == N * sizeof(libv::meta::lnv_t<Element, decltype(vec[0])>);
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename Type, std::size_t Dimension, typename... Element>
//struct Vec : VecData<Type, Dimension, Element...> {};
//
//template <typename Type, typename... Element>
//struct Vec<Type, 2, Element...> : Vec2<Type, Element...> {};
//
//template <typename Type, typename... Element>
//struct Vec<Type, 3, Element...> : Vec3<Type, Element...> {};
//
//template <typename Type, typename... Element>
//struct Vec<Type, 4, Element...> : Vec4<Type, Element...> {};

// -------------------------------------------------------------------------------------------------

} // namespace libv
