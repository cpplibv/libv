// Project: libv.glr, File: src/libv/glr/layout_std140.hpp

#pragma once

// std
#include <array>
#include <cstring>
#include <type_traits>
// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/always.hpp>
#include <libv/meta/for_constexpr.hpp>
#include <libv/meta/reflection.hpp>
#include <libv/meta/type_traits.hpp>
#include <libv/utility/align.hpp>
#include <libv/utility/observer_ref.hpp>
// pro
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_block_layout.hpp>


// Specs:
//
//    When using the "std140" storage layout, structures will be laid out in
//    buffer storage with its members stored in monotonically increasing order
//    based on their location in the declaration. A structure and each
//    structure member have a base offset and a base alignment, from which an
//    aligned offset is computed by rounding the base offset up to a multiple of
//    the base alignment. The base offset of the first member of a structure is
//    taken from the aligned offset of the structure itself. The base offset of
//    all other structure members is derived by taking the offset of the last
//    basic machine unit consumed by the previous member and adding one. Each
//    structure member is stored in memory at its aligned offset. The members
//    of a top-level uniform block are laid out in buffer storage by treating
//    the uniform block as a structure with a base offset of zero.
//
//      (1) If the member is a scalar consuming <N> basic machine units, the
//          base alignment is <N>.
//
//      (2) If the member is a two- or four-component vector with components
//          consuming <N> basic machine units, the base alignment is 2<N> or
//          4<N>, respectively.
//
//      (3) If the member is a three-component vector with components consuming
//          <N> basic machine units, the base alignment is 4<N>.
//
//      (4) If the member is an array of scalars or vectors, the base alignment
//          and array stride are set to match the base alignment of a single
//          array element, according to rules (1), (2), and (3), and rounded up
//          to the base alignment of a vec4. The array may have padding at the
//          end; the base offset of the member following the array is rounded up
//          to the next multiple of the base alignment.
//
//      (5) If the member is a column-major matrix with <C> columns and <R>
//          rows, the matrix is stored identically to an array of <C> column
//          vectors with <R> components each, according to rule (4).
//
//      (6) If the member is an array of <S> column-major matrices with <C>
//          columns and <R> rows, the matrix is stored identically to a row of
//          <S>*<C> column vectors with <R> components each, according to rule
//          (4).
//
//      (7) If the member is a row-major matrix with <C> columns and <R> rows,
//          the matrix is stored identically to an array of <R> row vectors
//          with <C> components each, according to rule (4).
//
//      (8) If the member is an array of <S> row-major matrices with <C> columns
//          and <R> rows, the matrix is stored identically to a row of <S>*<R>
//          row vectors with <C> components each, according to rule (4).
//
//      (9) If the member is a structure, the base alignment of the structure is
//          <N>, where <N> is the largest base alignment value of any of its
//          members, and rounded up to the base alignment of a vec4. The
//          individual members of this sub-structure are then assigned offsets
//          by applying this set of rules recursively, where the base offset of
//          the first member of the sub-structure is equal to the aligned offset
//          of the structure. The structure may have padding at the end; the
//          base offset of the member following the sub-structure is rounded up
//          to the next multiple of the base alignment of the structure.
//
//      (10) If the member is an array of <S> structures, the <S> elements of
//           the array are laid out in order, according to rule (9).
//
//    For uniform blocks laid out according to these rules, the minimum buffer
//    object size returned by the UNIFORM_BLOCK_DATA_SIZE query is derived by
//    taking the offset of the last basic machine unit consumed by the last
//    uniform of the uniform block (including any end-of-array or
//    end-of-structure padding), adding one, and rounding up to the next
//    multiple of the base alignment required for a vec4.
//
// Example:
//    The following example illustrates the rules specified by the "std140"
//    layout.
//
//      layout(std140) uniform Example {
//
//                      // Base types below consume 4 basic machine units
//                      //
//                      //       base   base  align
//                      // rule  align  off.  off.  bytes used
//                      // ----  ------ ----  ----  -----------------------
//        float a;      //  1       4     0    0    0..3
//        vec2 b;       //  2       8     4    8    8..15
//        vec3 c;       //  3      16    16   16    16..27
//        struct {      //  9      16    28   32    (align begin)
//          int d;      //  1       4    32   32    32..35
//          bvec2 e;    //  2       8    36   40    40..47
//        } f;          //  9      16    48   48    (pad end)
//        float g;      //  1       4    48   48    48..51
//        float h[2];   //  4      16    52   64    64..67 (h[0])
//                      //                    80    80..83 (h[1])
//                      //  4      16    84   96    (pad end of h)
//        mat2x3 i;     // 5/4     16    96   96    96..107 (i, column 0)
//                      //                   112    112..123 (i, column 1)
//                      // 5/4     16   124  128    (pad end of i)
//        struct {      //  10     16   128  128    (align begin)
//          uvec3 j;    //  3      16   128  128    128..139 (o[0].j)
//          vec2 k;     //  2       8   140  144    144..151 (o[0].k)
//          float l[2]; //  4      16   152  160    160..163 (o[0].l[0])
//                      //                   176    176..179 (o[0].l[1])
//                      //  4      16   180  192    (pad end of o[0].l)
//          vec2 m;     //  2       8   192  192    192..199 (o[0].m)
//          mat3 n[2];  // 6/4     16   200  208    208..219 (o[0].n[0], column 0)
//                      //                   224    224..235 (o[0].n[0], column 1)
//                      //                   240    240..251 (o[0].n[0], column 2)
//                      //                   256    256..267 (o[0].n[1], column 0)
//                      //                   272    272..283 (o[0].n[1], column 1)
//                      //                   288    288..299 (o[0].n[1], column 2)
//                      // 6/4     16   300  304    (pad end of o[0].n)
//                      //  9      16   304  304    (pad end of o[0])
//                      //  3      16   304  304    304..315 (o[1].j)
//                      //  2       8   316  320    320..327 (o[1].k)
//                      //  4      16   328  336    336..347 (o[1].l[0])
//                      //                   352    352..355 (o[1].l[1])
//                      //  4      16   356  368    (pad end of o[1].l)
//                      //  2       8   368  368    368..375 (o[1].m)
//                      // 6/4     16   376  384    384..395 (o[1].n[0], column 0)
//                      //                   400    400..411 (o[1].n[0], column 1)
//                      //                   416    416..427 (o[1].n[0], column 2)
//                      //                   432    432..443 (o[1].n[1], column 0)
//                      //                   448    448..459 (o[1].n[1], column 1)
//                      //                   464    464..475 (o[1].n[1], column 2)
//                      // 6/4     16   476  480    (pad end of o[1].n)
//                      //  9      16   480  480    (pad end of o[1])
//        } o[2];
//      };


namespace libv {
namespace glr {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename T>
static constexpr bool is_glsl_scalar_v = libv::meta::is_same_as_any_v<T,
		bool, int32_t, uint32_t, int64_t, uint64_t, float, double>;

template <typename T>
static constexpr bool is_glsl_mat_scalar_v = libv::meta::is_same_as_any_v<T,
		float, double>;

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AlignSTD140;

// 1) scalar
template <> struct AlignSTD140<bool> {
	static constexpr uint32_t align = 4;
	static constexpr uint32_t size = 4;
};
template <> struct AlignSTD140<int32_t> {
	static constexpr uint32_t align = sizeof (int32_t);
	static constexpr uint32_t size = sizeof (int32_t);
};
template <> struct AlignSTD140<uint32_t> {
	static constexpr uint32_t align = sizeof (uint32_t);
	static constexpr uint32_t size = sizeof (uint32_t);
};
template <> struct AlignSTD140<int64_t> {
	static constexpr uint32_t align = sizeof (int64_t);
	static constexpr uint32_t size = sizeof (int64_t);
};
template <> struct AlignSTD140<uint64_t> {
	static constexpr uint32_t align = sizeof (uint64_t);
	static constexpr uint32_t size = sizeof (uint64_t);
};
template <> struct AlignSTD140<float> {
	static constexpr uint32_t align = sizeof (float);
	static constexpr uint32_t size = sizeof (float);
};
template <> struct AlignSTD140<double> {
	static constexpr uint32_t align = sizeof (double);
	static constexpr uint32_t size = sizeof (double);
};

// 2) two- or four-component vector
template <typename T> struct AlignSTD140<libv::vec2_t<T>> {
	static_assert(is_glsl_scalar_v<T>, "T has to be a glsl scalar type in vec<T>");
	static constexpr uint32_t align = 2 * AlignSTD140<T>::align;
	static constexpr uint32_t size = 2 * AlignSTD140<T>::size;
};
template <typename T> struct AlignSTD140<libv::vec4_t<T>> {
	static_assert(is_glsl_scalar_v<T>, "T has to be a glsl scalar type in vec<T>");
	static constexpr uint32_t align = 4 * AlignSTD140<T>::align;
	static constexpr uint32_t size = 4 * AlignSTD140<T>::size;
};

// 3) three-component vector
template <typename T> struct AlignSTD140<libv::vec3_t<T>> {
	static_assert(is_glsl_scalar_v<T>, "T has to be a glsl scalar type in vec<T>");
	static constexpr uint32_t align = 4 * AlignSTD140<T>::align;
	static constexpr uint32_t size = 3 * AlignSTD140<T>::align;
};

// 4) array of scalars or vectors
template <typename T, std::size_t S> WISH_REQUIRES(is_glsl_scalar_v<T>) struct AlignSTD140<std::array<T, S>> {
	static constexpr uint32_t align = libv::align(AlignSTD140<T>::align, AlignSTD140<libv::vec4f>::align);
	static constexpr uint32_t size = S * libv::align(AlignSTD140<T>::size, AlignSTD140<libv::vec4f>::size);
};
template <typename T, std::size_t N, std::size_t S> struct AlignSTD140<std::array<libv::vec_t<N, T>, S>> {
	static_assert(is_glsl_scalar_v<T>, "T has to be a glsl scalar type in array<vec<T>>");
	static constexpr uint32_t align = libv::align(AlignSTD140<libv::vec_t<N, T>>::align, AlignSTD140<libv::vec4f>::align);
	static constexpr uint32_t size = S * libv::align(AlignSTD140<libv::vec_t<N, T>>::size, AlignSTD140<libv::vec4f>::size);
};
template <typename T, std::size_t S> struct AlignSTD140<std::array<T, S>> {
	static_assert(libv::meta::always_false_v<T>, "T has to be a scalar or vec type in array<T>");
};

// 5) 7) matrix
template <typename T, std::size_t R, std::size_t C> struct AlignSTD140<libv::mat_t<R, C, T>> :
	AlignSTD140<std::array<libv::vec_t<C, T>, R>> {
	static_assert(2 <= C && C <= 4, "Matrix type column dimension has to be 2, 3 or 4");
	static_assert(2 <= R && R <= 4, "Matrix type row dimension has to be 2, 3 or 4");
	static_assert(is_glsl_mat_scalar_v<T>, "T has to be a mat scalar type");
};

// 6) 8) array of matrices
template <typename T, std::size_t R, std::size_t C, std::size_t S> struct AlignSTD140<std::array<libv::mat_t<R, C, T>, S>> :
	AlignSTD140<std::array<libv::vec_t<C, T>, R * S>> {
	static_assert(2 <= C && C <= 4, "Matrix type column dimension has to be 2, 3 or 4");
	static_assert(2 <= R && R <= 4, "Matrix type row dimension has to be 2, 3 or 4");
	static_assert(is_glsl_mat_scalar_v<T>, "T has to be a mat scalar type");
};

// 9) 10) structure or array of structures
template <typename T> struct AlignSTD140 {
	static constexpr uint32_t calculate_align() {
		uint32_t result = 0;
		libv::meta::foreach_static_member<T>([&](auto member) {
			using M = typename std::decay_t<decltype(member)>::value_type;

			if constexpr (isUniform_v<M>)
				result = std::max(result, AlignSTD140<typename M::value_type>::align);
			else if constexpr (libv::meta::is_array_v<M>)
				result = std::max(result, AlignSTD140<typename M::value_type>::align);
			else
				result = std::max(result, AlignSTD140<M>::align);
		});
		return libv::align(result, AlignSTD140<libv::vec4f>::align);
	}

	static constexpr uint32_t calculate_size() {
		uint32_t result = 0;
		libv::meta::foreach_static_member<T>([&](auto member) {
			using M = typename std::decay_t<decltype(member)>::value_type;

			if constexpr (isUniform_v<M>) {
				result = libv::align(result, AlignSTD140<typename M::value_type>::align);
				result += AlignSTD140<typename M::value_type>::size;
			} else if constexpr (libv::meta::is_array_v<M>) {
				libv::meta::for_constexpr<0, libv::meta::array_size_v<M>>([&](auto) {
					// TODO P5: for_constexpr's loop variable should be optional
					result = libv::align(result, AlignSTD140<typename M::value_type>::align);
					result += AlignSTD140<typename M::value_type>::size;
				});
			} else {
				result = libv::align(result, AlignSTD140<M>::align);
				result += AlignSTD140<M>::size;
			}
		});
		return libv::align(result, align);
	}

	static constexpr uint32_t align = calculate_align();
	static constexpr uint32_t size = calculate_size();
};

// -------------------------------------------------------------------------------------------------

template <typename T>
constexpr inline uint32_t layout_std140_align() {
	return AlignSTD140<T>::align;
}

template <typename T>
constexpr inline uint32_t layout_std140_size() {
	return AlignSTD140<T>::size;
}

template <typename T>
constexpr inline uint32_t layout_std140_stride() {
	return libv::align(layout_std140_size<T>(), layout_std140_align<T>());
}

template <typename T>
constexpr inline void assign_layout_std140(T& object, uint32_t offset) {
	libv::meta::foreach_member_reference(object, [&](auto& member) {
		using M = std::decay_t<decltype(member)>;

		if constexpr (isUniform_v<M>) {
			const auto align = layout_std140_align<typename M::value_type>();
			const auto size = layout_std140_size<typename M::value_type>();

			member.location = libv::align(offset, align);
			offset = member.location + size;

		} else if constexpr (libv::meta::is_array_v<M>) {
			const auto align = layout_std140_align<typename M::value_type>();
			const auto size = layout_std140_size<typename M::value_type>();

			for (auto& item : member) {
				offset = libv::align(offset, align);
				assign_layout_std140(item, offset);
				offset += size;
			}

		} else {
			const auto align = layout_std140_align<M>();
			const auto size = layout_std140_size<M>();

			offset = libv::align(offset, align);
			assign_layout_std140(member, offset);
			offset += size;
		}
	});
}

} // namespace detail ------------------------------------------------------------------------------

using detail::layout_std140_align;
using detail::layout_std140_size;
using detail::layout_std140_stride;

template <typename T>
inline UniformBlockLayout<T> layout_std140(const libv::glr::UniformBlockBinding& binding) {
	UniformBlockLayout<T> result;
	result.size = layout_std140_size<T>();
	result.binding = binding.binding;
	detail::assign_layout_std140(static_cast<T&>(result), 0);
	return result;
}

inline void write_std140(const libv::observer_ref<std::byte> target, const bool value) noexcept {
	const auto value_ = static_cast<int32_t>(value);
	std::memcpy(target, &value_, sizeof(value_));
}

template <typename T>
inline void write_std140(const libv::observer_ref<std::byte> target, const T& value) noexcept {
	std::memcpy(target, &value, sizeof(T));
}

template <typename T, std::size_t N>
inline void write_std140(const libv::observer_ref<std::byte> target, const std::array<T, N>& value) noexcept {
	const auto stride = layout_std140_stride<T>();

	libv::meta::for_constexpr<0, N>([&](auto i) {
		std::memcpy(target + i * stride, &value[i], sizeof(value[i]));
	});
}

template <typename T, std::size_t N>
inline void write_std140(const libv::observer_ref<std::byte> target, const libv::mat_t<N, N, T>& value) noexcept {
	const auto stride = layout_std140_stride<libv::vec_t<N, T >> ();

	libv::meta::for_constexpr<0, N>([&](auto i) {
		std::memcpy(target + i * stride, &value[i], sizeof(value[i]));
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv

//// -------------------------------------------------------------------------------------------------
//
// Consider this alternative approach:
//
//struct UnfirormLayoutRule {
//	uint32_t align = 0;
//	uint32_t size = 0;
//};
//
//template <typename T> using identity = libv::meta::identity_t<T>;
//
//inline UnfirormLayoutRule rule_std140(...) { static_assert }
//
//inline UnfirormLayoutRule rule_std140(identity<bool>)          { return { 4,  4}; }
//inline UnfirormLayoutRule rule_std140(identity<int32_t>)       { return { 4,  4}; }
//inline UnfirormLayoutRule rule_std140(identity<uint32_t>)      { return { 4,  4}; }
//inline UnfirormLayoutRule rule_std140(identity<int64_t>)       { return { 8,  8}; }
//inline UnfirormLayoutRule rule_std140(identity<uint64_t>)      { return { 8,  8}; }
//inline UnfirormLayoutRule rule_std140(identity<float>)         { return { 4,  4}; }
//inline UnfirormLayoutRule rule_std140(identity<double>)        { return { 8,  8}; }
//
//inline UnfirormLayoutRule rule_std140(identity<libv::vec2b>)   { return { 8,  8}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec3b>)   { return {16, 12}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec4b>)   { return {16, 16}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec2i>)   { return { 8,  8}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec3i>)   { return {16, 12}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec4i>)   { return {16, 16}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec2ui>)  { return { 8,  8}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec3ui>)  { return {16, 12}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec4ui>)  { return {16, 16}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec2l>)   { return {16, 16}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec3l>)   { return {32, 24}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec4l>)   { return {32, 32}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec2ul>)  { return {16, 16}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec3ul>)  { return {32, 24}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec4ul>)  { return {32, 32}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec2f>)   { return { 8,  8}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec3f>)   { return {16, 12}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec4f>)   { return {16, 16}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec2d>)   { return {16, 16}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec3d>)   { return {32, 24}; }
//inline UnfirormLayoutRule rule_std140(identity<libv::vec4d>)   { return {32, 32}; }
//
//template <typename T, std::size_t N>
//inline UnfirormLayoutRule rule_std140(identity<std::array<T, N>) {
//	const auto member_rule = rule_std140(identity<T>{});
//	const auto vec4_rule = rule_std140(identity<libv::vec4f>{});
//
//	return {
//		libv::align(member_rule.align, vec4_rule.align),
//		libv::align(member_rule.size, vec4_rule.size) * N
//	};
//}
//
//inline UnfirormLayoutRule rule_std140(identity<libv::mat2f>)   { return rule_std140(identity<std::array<libv::vec2f, 2>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat3f>)   { return rule_std140(identity<std::array<libv::vec3f, 3>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat4f>)   { return rule_std140(identity<std::array<libv::vec4f, 4>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat2d>)   { return rule_std140(identity<std::array<libv::vec2f, 2>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat3d>)   { return rule_std140(identity<std::array<libv::vec3f, 3>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat4d>)   { return rule_std140(identity<std::array<libv::vec4f, 4>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat2x3f>) { return rule_std140(identity<std::array<libv::vec3f, 2>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat2x4f>) { return rule_std140(identity<std::array<libv::vec4f, 2>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat3x2f>) { return rule_std140(identity<std::array<libv::vec2f, 3>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat3x4f>) { return rule_std140(identity<std::array<libv::vec4f, 3>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat4x2f>) { return rule_std140(identity<std::array<libv::vec2f, 4>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat4x3f>) { return rule_std140(identity<std::array<libv::vec3f, 4>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat2x3d>) { return rule_std140(identity<std::array<libv::vec3d, 2>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat2x4d>) { return rule_std140(identity<std::array<libv::vec4d, 2>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat3x2d>) { return rule_std140(identity<std::array<libv::vec2d, 3>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat3x4d>) { return rule_std140(identity<std::array<libv::vec4d, 3>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat4x2d>) { return rule_std140(identity<std::array<libv::vec2d, 4>>{}); }
//inline UnfirormLayoutRule rule_std140(identity<libv::mat4x3d>) { return rule_std140(identity<std::array<libv::vec3d, 4>>{}); }
//
//// -------------------------------------------------------------------------------------------------
