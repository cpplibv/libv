// File: Material.hpp, Created on 2015. március 10. 1:50, Author: Vader

#pragma once

// hpp
#include <vl/vm3/material.hpp>
// config
#include <vl/vm3/config.hpp>
// external
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_free.hpp>
// vl
#include <vl/serialization/vec.hpp>
#include <vl/serialization/glm.hpp>
// std
// pro

namespace vl {
	namespace vm3 {

		template<typename Archive>
		class MaterialPropertySerializer : public MaterialPropertyVisitor {
			Archive& ar;
		public:
			MaterialPropertySerializer(Archive& ar) : ar(ar) { }

#define VM3_PROPERTY_VISITOR_TYPE_HELPER(TYPE, ID) \
		void visit(const std::string& id, const TYPE& value) override { \
			uint16_t type = ID; \
			ar & VL_NVP(type); \
			ar & VL_NVP(id); \
			ar & VL_NVP(value);	}
			VM3_PROPERTY_VISITOR_TYPE_HELPER(int, 0x0001)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(float, 0x0002)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(double, 0x0003)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(std::string, 0x0004)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec2, 0x0010)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec3, 0x0011)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec4, 0x0012)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(vec2, 0x0020)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(vec3, 0x0021)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(vec4, 0x0022)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec2, 0x0030)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec3, 0x0031)
			VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec4, 0x0032)
#undef VM3_PROPERTY_VISITOR_TYPE_HELPER
		};
		template<typename Archive>
		std::unique_ptr<MaterialPropertyBase> deserializeMaterialProperty(uint16_t type, Archive& ar) {
			switch (type) {
#define VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(TYPE, ID) case ID: { TYPE value; ar & VL_NVP(value); \
					return std::make_unique<MaterialProperty<TYPE>>(value); break; }
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(int, 0x0001)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(float, 0x0002)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(double, 0x0003)
				default:
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(std::string, 0x0004)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(ivec2, 0x0010)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(ivec3, 0x0011)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(ivec4, 0x0012)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(vec2, 0x0020)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(vec3, 0x0021)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(vec4, 0x0022)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(dvec2, 0x0030)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(dvec3, 0x0031)
					VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(dvec4, 0x0032)
#undef VM3_PROPERTY_DESERIALIZE_TYPE_HELPER
			}
		}

		struct MaterialSerialization {
			template<class Archive>
			static inline void save(Archive& ar, const ::vl::vm3::Material& var, const unsigned int) {
				ar & VL_NVP_NAMED("name", var.name);
				ar & VL_NVP_NAMED("shader", var.shader);
				size_t size = var.properties.size();
				ar & VL_NVP_NAMED("count", size);

				MaterialPropertySerializer<Archive> mps(ar);
				for (auto& item : var.properties) {
					item.second->accept(mps, item.first);
				}
			}
			template<class Archive>
			static inline void load(Archive& ar, ::vl::vm3::Material& var, const unsigned int) {
				ar & VL_NVP_NAMED("name", var.name);
				ar & VL_NVP_NAMED("shader", var.shader);
				size_t size = 0;
				ar & VL_NVP_NAMED("count", size);
				var.properties.clear();
				//properties.reserve(size);
				while (size-- > 0) {
					uint16_t type = 0;
					std::string id;
					ar & VL_NVP(type);
					ar & VL_NVP(id);
					var.properties.emplace(id, deserializeMaterialProperty(type, ar));
				}
			}
		};
	} //namespace vm3
} //namespace vl

BOOST_SERIALIZATION_SPLIT_FREE(::vl::vm3::Material);

namespace boost {
	namespace serialization {
		template<class Archive>
		static void save(Archive& ar, const ::vl::vm3::Material& var, const unsigned int ver) {
			::vl::vm3::MaterialSerialization::save(ar, var, ver);
		}
		template<class Archive>
		static void load(Archive& ar, ::vl::vm3::Material& var, const unsigned int ver) {
			::vl::vm3::MaterialSerialization::load(ar, var, ver);
		}
	} //namespace boost
} //namespace serialization