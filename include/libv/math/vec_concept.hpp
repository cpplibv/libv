// File: vec_concept.hpp - Created on 2018.04.15. 07:50 - Author: Vader

#pragma once

// libv
#include <libv/utility/member_offset.hpp>
// ext
#include <range/v3/utility/concepts.hpp>


// -------------------------------------------------------------------------------------------------

namespace libv {

struct _ConceptVec2 {
	template <typename Vector, typename Element>
	auto requires_(Vector vector, Element element) -> decltype(
		ranges::concepts::valid_expr(
			ranges::concepts::same_type(vector.x, element),
			ranges::concepts::same_type(vector.y, element),

			ranges::concepts::is_true(std::bool_constant<sizeof(Vector) == 2 * sizeof(Element)>{}),

			ranges::concepts::is_true(std::bool_constant<(member_offset(&Vector::x) < member_offset(&Vector::y))>{})
		));
	template <typename Vector>
	auto requires_(Vector vector) -> decltype(requires_(vector, vector.x));
};

struct _ConceptVec3 {
	template <typename Vector, typename Element>
	auto requires_(Vector vector, Element element) -> decltype(
		ranges::concepts::valid_expr(
			ranges::concepts::same_type(vector.x, element),
			ranges::concepts::same_type(vector.y, element),
			ranges::concepts::same_type(vector.z, element),

			ranges::concepts::is_true(std::bool_constant<sizeof(Vector) == 3 * sizeof(Element)>{}),

			ranges::concepts::is_true(std::bool_constant<(member_offset(&Vector::x) < member_offset(&Vector::y))>{}),
			ranges::concepts::is_true(std::bool_constant<(member_offset(&Vector::y) < member_offset(&Vector::z))>{})
		));
	template <typename Vector>
	auto requires_(Vector vector) -> decltype(requires_(vector, vector.x));
};

struct _ConceptVec4 {
	template <typename Vector, typename Element>
	auto requires_(Vector vector, Element element) -> decltype(
		ranges::concepts::valid_expr(
			ranges::concepts::same_type(vector.x, element),
			ranges::concepts::same_type(vector.y, element),
			ranges::concepts::same_type(vector.z, element),
			ranges::concepts::same_type(vector.w, element),

			ranges::concepts::is_true(std::bool_constant<sizeof(Vector) == 4 * sizeof(Element)>{}),

			ranges::concepts::is_true(std::bool_constant<(member_offset(&Vector::x) < member_offset(&Vector::y))>{}),
			ranges::concepts::is_true(std::bool_constant<(member_offset(&Vector::y) < member_offset(&Vector::z))>{}),
			ranges::concepts::is_true(std::bool_constant<(member_offset(&Vector::z) < member_offset(&Vector::w))>{})
		));
	template <typename Vector>
	auto requires_(Vector vector) -> decltype(requires_(vector, vector.x));
};

/// Two dimensional mathematical vector concept. Types with x, y as member variables but without z or w as members
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage:\code
/// template <typename V, CONCEPT_REQUIRES_(Vec2<V>())> void function(V& vec) {}\endcode
/// @example Concept usage with Element type specified:\code
/// template <typename V, CONCEPT_REQUIRES_(Vec2<V, float>())> void function(V& vec) {}\endcode
template <typename Type, typename... Element>
using Vec2 = ranges::concepts::models<_ConceptVec2, Type, Element...>;

/// Three dimensional mathematical vector concept. Types with x, y, z as member variables but without w as members
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage:\code
/// template <typename V, CONCEPT_REQUIRES_(Vec3<V>())> void function(V& vec) {}\endcode
/// @example Concept usage with Element type specified:\code
/// template <typename V, CONCEPT_REQUIRES_(Vec3<V, float>())> void function(V& vec) {}\endcode
template <typename Type, typename... Element>
using Vec3 = ranges::concepts::models<_ConceptVec3, Type, Element...>;

/// Four dimensional mathematical vector concept. Types with x, y, z, w as member variables
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage:\code
/// template <typename V, CONCEPT_REQUIRES_(Vec4<V>())> void function(V& vec) {}\endcode
/// @example Concept usage with Element type specified:\code
/// template <typename V, CONCEPT_REQUIRES_(Vec4<V, float>())> void function(V& vec) {}\endcode
template <typename Type, typename... Element>
using Vec4 = ranges::concepts::models<_ConceptVec4, Type, Element...>;

// -------------------------------------------------------------------------------------------------

//struct _ConceptVecData {
//	template <typename Vector, typename Dimension, typename Element>
//	auto requires_(Vector vector, Element element) -> decltype(
//		ranges::concepts::valid_expr(
//			ranges::concepts::same_type(vector.data[0], element),
//
//			ranges::concepts::is_true(std::size(vector.data) == Dimension::value),
//			ranges::concepts::is_true(std::bool_constant<sizeof(Vector) == Dimension::value * sizeof(Element)>{})
//		));
//	template <typename Vector, typename Dimension>
//	auto requires_(Vector vector) -> decltype(requires_<Vector, Dimension>(vector, vector.data[0]));
//};
//
///// N dimensional mathematical vector concept. Types with x, y as member variables but without z or w as members
///// @param Type - The Type to be tested
///// @param Dimension - Number of dimension in the vector
///// @param Element - Optional parameter to specify the Element's Type
///// @example Concept usage:\code
///// template <typename V, CONCEPT_REQUIRES_(Vec<V, 3>())> void function(V& vec) {}\endcode
///// @example Concept usage with Element type specified:\code
///// template <typename V, CONCEPT_REQUIRES_(Vec<V, 3, float>())> void function(V& vec) {}\endcode
//template <typename Type, size_t Dimension, typename... Element>
//using VecData = ranges::concepts::models<_ConceptVecData, Type, std::integral_constant<size_t, Dimension>, Element...>;
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
