// File: MaterialPropertyVisitor.hpp, Created on 2015. február 13. 22:37, Author: Vader

#pragma once

// libv
#include <libv/vec.hpp>
// std
#include <iostream>

namespace libv {
namespace vm3 {
class Material;

struct MaterialPropertyVisitor {
	virtual void visit(const std::string& name, const int& value) = 0;
	virtual void visit(const std::string& name, const float& value) = 0;
	virtual void visit(const std::string& name, const double& value) = 0;
	virtual void visit(const std::string& name, const std::string& value) = 0;
	virtual void visit(const std::string& name, const ivec2& value) = 0;
	virtual void visit(const std::string& name, const ivec3& value) = 0;
	virtual void visit(const std::string& name, const ivec4& value) = 0;
	virtual void visit(const std::string& name, const vec2& value) = 0;
	virtual void visit(const std::string& name, const vec3& value) = 0;
	virtual void visit(const std::string& name, const vec4& value) = 0;
	virtual void visit(const std::string& name, const dvec2& value) = 0;
	virtual void visit(const std::string& name, const dvec3& value) = 0;
	virtual void visit(const std::string& name, const dvec4& value) = 0;
	virtual ~MaterialPropertyVisitor() { }
};

//------------------------------------------------------------------------------------------

class MaterialPropertyPrinter : public MaterialPropertyVisitor {
	std::ostream& stream;
public:
	MaterialPropertyPrinter(std::ostream& stream = std::cout);

	void visit(const std::string& name, const int& value) override;
	void visit(const std::string& name, const float& value) override;
	void visit(const std::string& name, const double& value) override;
	void visit(const std::string& name, const std::string& value) override;
	void visit(const std::string& name, const ivec2& value) override;
	void visit(const std::string& name, const ivec3& value) override;
	void visit(const std::string& name, const ivec4& value) override;
	void visit(const std::string& name, const vec2& value) override;
	void visit(const std::string& name, const vec3& value) override;
	void visit(const std::string& name, const vec4& value) override;
	void visit(const std::string& name, const dvec2& value) override;
	void visit(const std::string& name, const dvec3& value) override;
	void visit(const std::string& name, const dvec4& value) override;
};

class MaterialPropertyCounter : public MaterialPropertyVisitor {
#define VM3_PROPERTY_VISITOR_TYPE_HELPER(TYPE, NAME) \
			private: int _num##NAME = 0; \
			public: inline int num##NAME() { return _num##NAME; } \
			void visit(const std::string&, const TYPE&) override;
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
};

class MaterialPropertyCopier : public MaterialPropertyVisitor {
	Material* target;
public:
	MaterialPropertyCopier(Material& target);
private:
	void visit(const std::string& name, const int& value) override;
	void visit(const std::string& name, const float& value) override;
	void visit(const std::string& name, const double& value) override;
	void visit(const std::string& name, const std::string& value) override;
	void visit(const std::string& name, const ivec2& value) override;
	void visit(const std::string& name, const ivec3& value) override;
	void visit(const std::string& name, const ivec4& value) override;
	void visit(const std::string& name, const vec2& value) override;
	void visit(const std::string& name, const vec3& value) override;
	void visit(const std::string& name, const vec4& value) override;
	void visit(const std::string& name, const dvec2& value) override;
	void visit(const std::string& name, const dvec3& value) override;
	void visit(const std::string& name, const dvec4& value) override;
};
} //namespace vm3
} //namespace libv