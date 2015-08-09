// File: MaterialPropertyVisitor.cpp, Created on 2015. február 14. 21:24, Author: Vader

#include <vl/vm3/material.hpp>
#include <vl/vm3/material_property_visitor.hpp>

namespace vl {
	namespace vm3 {
		MaterialPropertyPrinter::MaterialPropertyPrinter(std::ostream& stream) :
			stream(stream) { }

#define VM3_PROPERTY_VISITOR_TYPE_HELPER(TYPE) \
		void MaterialPropertyPrinter::visit(const std::string& name, const TYPE& value) { \
			stream << name << ": " << value << std::endl; } 

		VM3_PROPERTY_VISITOR_TYPE_HELPER(int)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(float)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(double)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(std::string)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec4)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec4)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec4)
#undef VM3_PROPERTY_VISITOR_TYPE_HELPER

#define VM3_PROPERTY_VISITOR_TYPE_HELPER(TYPE, NAME) \
		void MaterialPropertyCounter::visit(const std::string&, const TYPE&) { ++_num##NAME; }
		VM3_PROPERTY_VISITOR_TYPE_HELPER(int, Int)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(float, Float)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(double, Double)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(std::string, String)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec2, Ivec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec3, Ivec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec4, Ivec4)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec2, Vec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec3, Vec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec4, Vec4)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec2, Dvec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec3, Dvec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec4, Dvec4)
#undef VM3_PROPERTY_VISITOR_TYPE_HELPER

		MaterialPropertyCopier::MaterialPropertyCopier(Material& target) : target(&target) { }
#define VM3_PROPERTY_VISITOR_TYPE_HELPER(TYPE) \
		void MaterialPropertyCopier::visit(const std::string& name, const TYPE& value) { target->set(name, value); }
		VM3_PROPERTY_VISITOR_TYPE_HELPER(int)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(float)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(double)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(std::string)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(ivec4)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(vec4)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec2)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec3)
		VM3_PROPERTY_VISITOR_TYPE_HELPER(dvec4)
#undef VM3_PROPERTY_VISITOR_TYPE_HELPER
	} //namespace vm3
} //namespace vl