// File: vec_concept.hpp - Created on 2018.04.15. 07:50 - Author: Vader

#pragma once

// ext
#include <range/v3/utility/concepts.hpp>


// -------------------------------------------------------------------------------------------------

namespace libv {

struct _ConceptVec4 {
	template<typename Vector, typename Element>
	auto requires_(Vector vector, Element element) -> decltype(
		ranges::concepts::valid_expr(
			ranges::concepts::same_type(vector.x, element),
			ranges::concepts::same_type(vector.y, element),
			ranges::concepts::same_type(vector.z, element),
			ranges::concepts::same_type(vector.w, element)
		));
	template<typename Vector>
	auto requires_(Vector vector) -> decltype(requires_(vector, vector.x));
};

/// Four dimensional mathematical vector concept. Types with x, y, z, w as member variables
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage: \code
/// template <typename V, CONCEPT_REQUIRES_(Vec4<V>())> void function(V& vec) {} \endcode
/// @example Concept usage with Element type specified: \code
/// template <typename V, CONCEPT_REQUIRES_(Vec4<V, float>())> void function(V& vec) {} \endcode
template<typename Type, typename... Element>
using Vec4 = ranges::concepts::models<_ConceptVec4, Type, Element...>;

struct _ConceptVec3 {
	template<typename Vector, typename Element>
	auto requires_(Vector vector, Element element) -> decltype(
		ranges::concepts::valid_expr(
			ranges::concepts::same_type(vector.x, element),
			ranges::concepts::same_type(vector.y, element),
			ranges::concepts::same_type(vector.z, element),
			ranges::concepts::is_false(Vec4<Vector, Element>{})
		));
	template<typename Vector>
	auto requires_(Vector vector) -> decltype(requires_(vector, vector.x));
};

/// Three dimensional mathematical vector concept. Types with x, y, z as member variables but without w as members
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage: \code
/// template <typename V, CONCEPT_REQUIRES_(Vec3<V>())> void function(V& vec) {} \endcode
/// @example Concept usage with Element type specified: \code
/// template <typename V, CONCEPT_REQUIRES_(Vec3<V, float>())> void function(V& vec) {} \endcode
template<typename Type, typename... Element>
using Vec3 = ranges::concepts::models<_ConceptVec3, Type, Element...>;

struct _ConceptVec2 {
	template<typename Vector, typename Element>
	auto requires_(Vector vector, Element element) -> decltype(
		ranges::concepts::valid_expr(
			ranges::concepts::same_type(vector.x, element),
			ranges::concepts::same_type(vector.y, element),
			ranges::concepts::is_false(Vec3<Vector, Element>{}),
			ranges::concepts::is_false(Vec4<Vector, Element>{})
		));
	template<typename Vector>
	auto requires_(Vector vector) -> decltype(requires_(vector, vector.x));
};

/// Two dimensional mathematical vector concept. Types with x, y as member variables but without z or w as members
/// @param Type - The Type to be tested
/// @param Element - Optional parameter to specify the Element's Type
/// @example Concept usage: \code
/// template <typename V, CONCEPT_REQUIRES_(Vec2<V>())> void function(V& vec) {} \endcode
/// @example Concept usage with Element type specified: \code
/// template <typename V, CONCEPT_REQUIRES_(Vec2<V, float>())> void function(V& vec) {} \endcode
template<typename Type, typename... Element>
using Vec2 = ranges::concepts::models<_ConceptVec2, Type, Element...>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
