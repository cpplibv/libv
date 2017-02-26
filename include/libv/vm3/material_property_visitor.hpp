// File: MaterialPropertyVisitor.hpp, Created on 2015. február 13. 22:37, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
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
	virtual void visit(const std::string& name, const vec2i& value) = 0;
	virtual void visit(const std::string& name, const vec3i& value) = 0;
	virtual void visit(const std::string& name, const vec4i& value) = 0;
	virtual void visit(const std::string& name, const vec2f& value) = 0;
	virtual void visit(const std::string& name, const vec3f& value) = 0;
	virtual void visit(const std::string& name, const vec4f& value) = 0;
	virtual void visit(const std::string& name, const vec2d& value) = 0;
	virtual void visit(const std::string& name, const vec3d& value) = 0;
	virtual void visit(const std::string& name, const vec4d& value) = 0;
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
	void visit(const std::string& name, const vec2i& value) override;
	void visit(const std::string& name, const vec3i& value) override;
	void visit(const std::string& name, const vec4i& value) override;
	void visit(const std::string& name, const vec2f& value) override;
	void visit(const std::string& name, const vec3f& value) override;
	void visit(const std::string& name, const vec4f& value) override;
	void visit(const std::string& name, const vec2d& value) override;
	void visit(const std::string& name, const vec3d& value) override;
	void visit(const std::string& name, const vec4d& value) override;
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
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec2i, vec2I)
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec3i, vec3I)
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec4i, vec4I)
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec2f, Vec2)
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec3f, Vec3)
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec4f, Vec4)
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec2d, vec2D)
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec3d, vec3D)
	VM3_PROPERTY_VISITOR_TYPE_HELPER(vec4d, vec4D)
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
	void visit(const std::string& name, const vec2i& value) override;
	void visit(const std::string& name, const vec3i& value) override;
	void visit(const std::string& name, const vec4i& value) override;
	void visit(const std::string& name, const vec2f& value) override;
	void visit(const std::string& name, const vec3f& value) override;
	void visit(const std::string& name, const vec4f& value) override;
	void visit(const std::string& name, const vec2d& value) override;
	void visit(const std::string& name, const vec3d& value) override;
	void visit(const std::string& name, const vec4d& value) override;
};
} // namespace vm3
} // namespace libv