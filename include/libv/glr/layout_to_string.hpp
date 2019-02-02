// File:   std140.hpp Author: Vader Created on 2018. december 19., 17:20

#pragma once

// ext
#include <boost/container/flat_set.hpp>
// std
#include <array>
#include <ostream>
#include <sstream>
#include <typeindex>
// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/identity.hpp>
#include <libv/meta/reflection.hpp>
#include <libv/meta/type_traits.hpp>
// pro
#include <libv/glr/uniform.hpp>


namespace libv {
namespace glr {
namespace detail {

// -------------------------------------------------------------------------------------------------

struct ToStringLayouter {
	std::ostream& os;
	boost::container::flat_set<std::type_index> seen_types;

	std::string_view type_string(libv::meta::identity<bool>) { return "bool"; }
	std::string_view type_string(libv::meta::identity<int32_t>) { return "int"; }
	std::string_view type_string(libv::meta::identity<uint32_t>) { return "unsigned"; }
	std::string_view type_string(libv::meta::identity<int64_t>) { return "int64_t"; }
	std::string_view type_string(libv::meta::identity<uint64_t>) { return "uint64_t"; }
	std::string_view type_string(libv::meta::identity<float>) { return "float"; }
	std::string_view type_string(libv::meta::identity<double>) { return "double"; }

	std::string_view type_string(libv::meta::identity<libv::vec2b>) { return "vec2b"; }
	std::string_view type_string(libv::meta::identity<libv::vec3b>) { return "vec3b"; }
	std::string_view type_string(libv::meta::identity<libv::vec4b>) { return "vec4b"; }
	std::string_view type_string(libv::meta::identity<libv::vec2i>) { return "vec2i"; }
	std::string_view type_string(libv::meta::identity<libv::vec3i>) { return "vec3i"; }
	std::string_view type_string(libv::meta::identity<libv::vec4i>) { return "vec4i"; }
	std::string_view type_string(libv::meta::identity<libv::vec2ui>) { return "vec2u"; }
	std::string_view type_string(libv::meta::identity<libv::vec3ui>) { return "vec3u"; }
	std::string_view type_string(libv::meta::identity<libv::vec4ui>) { return "vec4u"; }
	std::string_view type_string(libv::meta::identity<libv::vec2l>) { return "i64vec2"; }
	std::string_view type_string(libv::meta::identity<libv::vec3l>) { return "i64vec3"; }
	std::string_view type_string(libv::meta::identity<libv::vec4l>) { return "i64vec4"; }
	std::string_view type_string(libv::meta::identity<libv::vec2ul>) { return "u64vec2"; }
	std::string_view type_string(libv::meta::identity<libv::vec3ul>) { return "u64vec3"; }
	std::string_view type_string(libv::meta::identity<libv::vec4ul>) { return "u64vec4"; }
	std::string_view type_string(libv::meta::identity<libv::vec2f>) { return "vec2"; }
	std::string_view type_string(libv::meta::identity<libv::vec3f>) { return "vec3"; }
	std::string_view type_string(libv::meta::identity<libv::vec4f>) { return "vec4"; }
	std::string_view type_string(libv::meta::identity<libv::vec2d>) { return "vec2d"; }
	std::string_view type_string(libv::meta::identity<libv::vec3d>) { return "vec3d"; }
	std::string_view type_string(libv::meta::identity<libv::vec4d>) { return "vec4d"; }

	std::string_view type_string(libv::meta::identity<libv::mat2f>) { return "mat2"; }
	std::string_view type_string(libv::meta::identity<libv::mat3f>) { return "mat3"; }
	std::string_view type_string(libv::meta::identity<libv::mat4f>) { return "mat4"; }
	std::string_view type_string(libv::meta::identity<libv::mat2d>) { return "mat2d"; }
	std::string_view type_string(libv::meta::identity<libv::mat3d>) { return "mat3d"; }
	std::string_view type_string(libv::meta::identity<libv::mat4d>) { return "mat4d"; }
	std::string_view type_string(libv::meta::identity<libv::mat2x3f>) { return "mat2x3"; }
	std::string_view type_string(libv::meta::identity<libv::mat2x4f>) { return "mat2x4"; }
	std::string_view type_string(libv::meta::identity<libv::mat3x2f>) { return "mat3x2"; }
	std::string_view type_string(libv::meta::identity<libv::mat3x4f>) { return "mat3x4"; }
	std::string_view type_string(libv::meta::identity<libv::mat4x2f>) { return "mat4x2"; }
	std::string_view type_string(libv::meta::identity<libv::mat4x3f>) { return "mat4x3"; }
	std::string_view type_string(libv::meta::identity<libv::mat2x3d>) { return "mat2x3d"; }
	std::string_view type_string(libv::meta::identity<libv::mat2x4d>) { return "mat2x4d"; }
	std::string_view type_string(libv::meta::identity<libv::mat3x2d>) { return "mat3x2d"; }
	std::string_view type_string(libv::meta::identity<libv::mat3x4d>) { return "mat3x4d"; }
	std::string_view type_string(libv::meta::identity<libv::mat4x2d>) { return "mat4x2d"; }
	std::string_view type_string(libv::meta::identity<libv::mat4x3d>) { return "mat4x3d"; }

	template <typename T>
	void print_members() {
		libv::meta::foreach_static_member<T>([&](const auto& member) {
			using M = typename std::decay_t<decltype(member)>::value_type;

			os << '\t';

			if constexpr (isUniform_v<M>) {
				if constexpr (libv::meta::is_array_v<typename M::value_type>)
					os << type_string(libv::meta::identity<typename M::value_type::value_type>{});
				else
					os << type_string(libv::meta::identity<typename M::value_type>{});

			} else if constexpr (libv::meta::is_array_v<M>) {
					os << typeid(typename M::value_type).name();

			} else {
				os << typeid(M).name();
			}

			os << ' ' << member.name;

			if constexpr (libv::meta::is_array_v<M>) {
				os << '[' << libv::meta::array_size_v<M> << "];\n";

			} else if constexpr (isUniform_v<M>) {
				if constexpr (libv::meta::is_array_v<typename M::value_type>)
					os << '[' << libv::meta::array_size_v<typename M::value_type> << "];\n";
				else
					os << ";\n";

			} else {
				os << ";\n";
			}
		});
	}

	template <typename T>
	void print_struct() {
		os << "struct " << typeid(T).name() << " {\n";
		print_members<T>();
		os << "};\n\n";
	}

	template <typename T>
	void pass_structs() {
		libv::meta::foreach_static_member<T>([&](const auto& member) {
			using M = typename std::decay_t<decltype(member)>::value_type;

			if constexpr (isUniform_v<M>) {
				// Skip non struct

			} else if constexpr (libv::meta::is_array_v<M>) {
				if (seen_types.insert(std::type_index(typeid(typename M::value_type))).second) {
					pass_structs<typename M::value_type>();
					print_struct<typename M::value_type>();
				}

			} else {
				if (seen_types.insert(std::type_index(typeid(M))).second) {
					pass_structs<M>();
					print_struct<M>();
				}
			}
		});
	}

	template <typename T>
	void pass(const std::string& name) {
		pass_structs<T>();

		os << "layout(std140) uniform " << name << " {\n";
		print_members<T>();
		os << "};\n";
	}
};

} // namespace detail ------------------------------------------------------------------------------

template <typename T>
inline std::string layout_to_string(const std::string& name = "block") {
	std::ostringstream ss;

	detail::ToStringLayouter layouter{ss, {}};
	layouter.pass<T>(name);

	return ss.str();
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
