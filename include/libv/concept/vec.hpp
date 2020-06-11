// File: vec_concept.hpp - Created on 2018.04.15. 07:50 - Author: Vader

#pragma once

// std
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
concept Vec2 = requires(Vector vector) {
		requires std::is_same_v<decltype(vector.x), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires std::is_same_v<decltype(vector.y), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires sizeof(vector) == 2 * sizeof(libv::meta::lnv_t<Element, decltype(vector.x)>);
		requires &vector.x < &vector.y;
};

/// Three dimensional mathematical vector concept. Types with x, y, z member variables
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage:\code
/// template <typename V> WISH_REQUIRES(Vec3<V>) void function(V& vec) {}\endcode
/// @example Concept usage with Element type specified:\code
/// template <typename V> WISH_REQUIRES(Vec3<V, float>) void function(V& vec) {}\endcode
template <typename Vector, typename Element = void>
concept Vec3 = requires(Vector vector) {
		requires std::is_same_v<decltype(vector.x), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires std::is_same_v<decltype(vector.y), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires std::is_same_v<decltype(vector.z), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires sizeof(vector) == 3 * sizeof(libv::meta::lnv_t<Element, decltype(vector.x)>);
		requires &vector.x < &vector.y;
		requires &vector.y < &vector.z;
};

/// Four dimensional mathematical vector concept. Types with x, y, z, w as member variables
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage:\code
/// template <typename V> WISH_REQUIRES(Vec4<V>) void function(V& vec) {}\endcode
/// @example Concept usage with Element type specified:\code
/// template <typename V> WISH_REQUIRES(Vec4<V, float>) void function(V& vec) {}\endcode
template <typename Vector, typename Element = void>
concept Vec4 = requires(Vector vector) {
		requires std::is_same_v<decltype(vector.x), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires std::is_same_v<decltype(vector.y), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires std::is_same_v<decltype(vector.z), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires std::is_same_v<decltype(vector.w), libv::meta::lnv_t<Element, decltype(vector.x)>>;
		requires sizeof(vector) == 4 * sizeof(libv::meta::lnv_t<Element, decltype(vector.x)>);
		requires &vector.x < &vector.y;
		requires &vector.y < &vector.z;
		requires &vector.z < &vector.w;
};

// -------------------------------------------------------------------------------------------------

///// N dimensional mathematical vector concept. Types with x, y as member variables but without z or w as members
///// @param Type - The Type to be tested
///// @param Dimension - Number of dimension in the vector
///// @param Element - Optional parameter to specify the Element's Type
///// @example Concept usage:\code
///// template <typename V, CONCEPT_REQUIRES_(Vec<V, 3>())> void function(V& vec) {}\endcode
///// @example Concept usage with Element type specified:\code
///// template <typename V, CONCEPT_REQUIRES_(Vec<V, 3, float>())> void function(V& vec) {}\endcode
//template <typename Vector, size_t N, typename Element = void>
//concept Vec = requires(Vector vector) {
//		requires std::is_void_v<Element> || std::is_same_v<decltype(vector[0]), Element>;
//		requires sizeof(vector) == N * sizeof(libv::meta::lnv_t<Element, decltype(vector[0])>);
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename Type, size_t Dimension, typename... Element>
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
