// Project: libv.math, File: src/libv/math/vec_concept.hpp

#pragma once

// std
#include <concepts>
#include <type_traits>
// libv
#include <libv/meta/lnv.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Two dimensional mathematical vector concept. Types with x, y member variables
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage:\code
/// template <typename V> WISH_REQUIRES(Vec2<V>) void function(V& vec) {}\endcode
/// @example Concept usage with Element type specified:\code
/// template <typename V> WISH_REQUIRES(Vec2<V, float>) void function(V& vec) {}\endcode
template <typename Vector, typename Element = void>
concept Vec2 = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires std::is_same_v<decltype(vec.y), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires sizeof(vec) == 2 * sizeof(libv::meta::lnv_t<Element, decltype(vec.x)>);
	requires &vec.x < &vec.y;
};

/// Three dimensional mathematical vector concept. Types with x, y, z member variables
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage:\code
/// template <typename V> WISH_REQUIRES(Vec3<V>) void function(V& vec) {}\endcode
/// @example Concept usage with Element type specified:\code
/// template <typename V> WISH_REQUIRES(Vec3<V, float>) void function(V& vec) {}\endcode
template <typename Vector, typename Element = void>
concept Vec3 = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires std::is_same_v<decltype(vec.y), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires std::is_same_v<decltype(vec.z), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires sizeof(vec) == 3 * sizeof(libv::meta::lnv_t<Element, decltype(vec.x)>);
	requires &vec.x < &vec.y;
	requires &vec.y < &vec.z;
};

/// Four dimensional mathematical vector concept. Types with x, y, z, w as member variables
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage:\code
/// template <typename V> WISH_REQUIRES(Vec4<V>) void function(V& vec) {}\endcode
/// @example Concept usage with Element type specified:\code
/// template <typename V> WISH_REQUIRES(Vec4<V, float>) void function(V& vec) {}\endcode
template <typename Vector, typename Element = void>
concept Vec4 = requires(Vector vec) {
	requires std::is_same_v<decltype(vec.x), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires std::is_same_v<decltype(vec.y), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires std::is_same_v<decltype(vec.z), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires std::is_same_v<decltype(vec.w), libv::meta::lnv_t<Element, decltype(vec.x)>>;
	requires sizeof(vec) == 4 * sizeof(libv::meta::lnv_t<Element, decltype(vec.x)>);
	requires &vec.x < &vec.y;
	requires &vec.y < &vec.z;
	requires &vec.z < &vec.w;
};

// -------------------------------------------------------------------------------------------------

///// N dimensional mathematical vector concept. Types with x, y as member variables but without z or w as members
///// @param Type - The Type to be tested
///// @param Dimension - Number of dimension in the vec
///// @param Element - Optional parameter to specify the Element's Type
///// @example Concept usage:\code
///// template <typename V, CONCEPT_REQUIRES_(Vec<V, 3>())> void function(V& vec) {}\endcode
///// @example Concept usage with Element type specified:\code
///// template <typename V, CONCEPT_REQUIRES_(Vec<V, 3, float>())> void function(V& vec) {}\endcode
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
