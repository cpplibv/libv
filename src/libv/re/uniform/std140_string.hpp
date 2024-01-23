// Project: libv.re, File: src/libv/re/uniform/std140_string.hpp

#pragma once

#include <boost/container/flat_set.hpp>

#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/identity.hpp>
#include <libv/re/uniform/std140_rules.hpp>
#include <libv/utility/type_key.hpp>

#include <sstream>


namespace libv {
namespace re {
namespace detail {

// -------------------------------------------------------------------------------------------------

struct StringBuilderSTD140 {
	std::ostream& os;
	boost::container::flat_set<libv::type_uid> seen_types;

	explicit StringBuilderSTD140(std::ostream& os) : os(os) { }

	[[nodiscard]] std::string_view type_string(libv::meta::identity<bool>) { return "bool"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<int32_t>) { return "int"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<uint32_t>) { return "uint"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<int64_t>) { return "int64_t"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<uint64_t>) { return "uint64_t"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<float>) { return "float"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<double>) { return "double"; }

	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec2b>) { return "bvec2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec3b>) { return "bvec3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec4b>) { return "bvec4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec2i>) { return "ivec2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec3i>) { return "ivec3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec4i>) { return "ivec4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec2ui>) { return "uvec2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec3ui>) { return "uvec3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec4ui>) { return "uvec4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec2l>) { return "i64vec2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec3l>) { return "i64vec3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec4l>) { return "i64vec4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec2ul>) { return "u64vec2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec3ul>) { return "u64vec3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec4ul>) { return "u64vec4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec2f>) { return "vec2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec3f>) { return "vec3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec4f>) { return "vec4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec2d>) { return "dvec2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec3d>) { return "dvec3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::vec4d>) { return "dvec4"; }

	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat2f>) { return "mat2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat3f>) { return "mat3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat4f>) { return "mat4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat2d>) { return "dmat2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat3d>) { return "dmat3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat4d>) { return "dmat4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat2x3f>) { return "mat2x3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat2x4f>) { return "mat2x4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat3x2f>) { return "mat3x2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat3x4f>) { return "mat3x4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat4x2f>) { return "mat4x2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat4x3f>) { return "mat4x3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat2x3d>) { return "dmat2x3"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat2x4d>) { return "dmat2x4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat3x2d>) { return "dmat3x2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat3x4d>) { return "dmat3x4"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat4x2d>) { return "dmat4x2"; }
	[[nodiscard]] std::string_view type_string(libv::meta::identity<libv::mat4x3d>) { return "dmat4x3"; }

	template <typename T>
	[[nodiscard]] std::string_view type_string(libv::meta::identity<T>) {
		if constexpr (requires { {T::name} -> std::convertible_to<std::string_view>; })
			return T::name;
		else if constexpr (requires { {T::name()} -> std::convertible_to<std::string_view>; })
			return T::name();
		else
			return typeid(T).name();
	}

	template <typename T>
	void print_array_size() {
		if constexpr (is_uniform_array<T>) {
			os << '[' << libv::meta::array_size_v<T> << "]";
			print_array_size<typename T::value_type>();
		}
	}

	template <typename T>
	auto innermost_type() {
		if constexpr (is_uniform_array<T>)
			return innermost_type<typename T::value_type>();
		else
			return libv::meta::identity<T>{};
	}

	template <typename T>
	auto& innermost_element_zero(T& object) {
		if constexpr (is_uniform_array<T>)
			return innermost_element_zero(object[0]);
		else
			return object;
	}

	template <typename T>
	void print_members(T& object) {
		auto func = [&]<typename M>(M& member, std::string_view name) {
			(void) member;
			if constexpr (is_uniform_padding<M>) {
				return;
			} else {
				os << '\t';
				os << type_string(innermost_type<M>());
				os << ' ' << name;
				if constexpr (is_uniform_array<M>)
					print_array_size<M>();
				os << ";\n";
			}
		};
		object.access_uniforms(func);
	}

	template <typename T>
	void print_struct(T& object) {
		os << "struct " << type_string(libv::meta::identity<T>{}) << " {\n";
		print_members(object);
		os << "};\n\n";
	}

	template <typename T>
	void process_structs(T& object) {
		auto func = [&]<typename M>(M& member, std::string_view name) {
			(void) name;
			using innermost_type = typename decltype(innermost_type<M>())::type;
			if constexpr (is_uniform_structure<innermost_type>) {
				if (!seen_types.emplace(libv::type_key<innermost_type>()).second)
					return;
				auto& elementZero = innermost_element_zero(member);
				process_structs(elementZero);
				print_struct(elementZero);
			}
		};
		object.access_uniforms(func);
	}

	template <typename T>
	void process_block(T& object, std::string_view name) {
		process_structs(object);

		os << "layout(std140) uniform " << name << " {\n";
		print_members(object);
		os << "};\n";
	}
};

} // namespace detail ------------------------------------------------------------------------------

// String std140 works differently from layout (same way as verify does).
// It works on native C++ types and not rev::Uniform<T> types
template <typename T>
std::string string_std140(std::string_view blockName) {
	static_assert(is_uniform_structure<T>);

	std::ostringstream ss;
	T block{};
	detail::StringBuilderSTD140 layouter{ss};
	layouter.process_block(block, blockName);

	return std::move(ss).str();
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv


