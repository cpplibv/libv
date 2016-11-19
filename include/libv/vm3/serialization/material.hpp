// File: Material.hpp, Created on 2015. március 10. 1:50, Author: Vader

#pragma once

// hpp
#include <libv/vm3/material.hpp>
// cfgig
#include <libv/vm3/config.hpp>
// external
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_free.hpp>
// libv
#include <libv/serialization/vec.hpp>
#include <libv/serialization/glm.hpp>
// std
// pro

namespace libv {
	namespace vm3 {

		template<typename Archive>
		class MaterialPropertySerializer : public MaterialPropertyVisitor {
			Archive& ar;
		public:
			MaterialPropertySerializer(Archive& ar) : ar(ar) { }

#define VM3_PROPERTY_VISITOR_TYPE_HELPER(TYPE, ID) \
		void visit(const std::string& id, const TYPE& value) override { \
			uint16_t type = ID; \
			ar & LIBV_NVP(type); \
			ar & LIBV_NVP(id); \
			ar & LIBV_NVP(value);	}
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
#define VM3_PROPERTY_DESERIALIZE_TYPE_HELPER(TYPE, ID) case ID: { TYPE value; ar & LIBV_NVP(value); \
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
			static inline void save(Archive& ar, const ::libv::vm3::Material& var, const unsigned int) {
				ar & LIBV_NVP_NAMED("name", var.name);
				ar & LIBV_NVP_NAMED("shader", var.shader);
				size_t size = var.properties.size();
				ar & LIBV_NVP_NAMED("count", size);

				MaterialPropertySerializer<Archive> mps(ar);
				for (auto& item : var.properties) {
					item.second->accept(mps, item.first);
				}
			}
			template<class Archive>
			static inline void load(Archive& ar, ::libv::vm3::Material& var, const unsigned int) {
				ar & LIBV_NVP_NAMED("name", var.name);
				ar & LIBV_NVP_NAMED("shader", var.shader);
				size_t size = 0;
				ar & LIBV_NVP_NAMED("count", size);
				var.properties.clear();
				//properties.reserve(size);
				while (size-- > 0) {
					uint16_t type = 0;
					std::string id;
					ar & LIBV_NVP(type);
					ar & LIBV_NVP(id);
					var.properties.emplace(id, deserializeMaterialProperty(type, ar));
				}
			}
		};
	} //namespace vm3
} //namespace libv

BOOST_SERIALIZATION_SPLIT_FREE(::libv::vm3::Material)

namespace boost {
	namespace serialization {
		template<class Archive>
		static void save(Archive& ar, const ::libv::vm3::Material& var, const unsigned int ver) {
			::libv::vm3::MaterialSerialization::save(ar, var, ver);
		}
		template<class Archive>
		static void load(Archive& ar, ::libv::vm3::Material& var, const unsigned int ver) {
			::libv::vm3::MaterialSerialization::load(ar, var, ver);
		}
	} //namespace boost
} //namespace serialization