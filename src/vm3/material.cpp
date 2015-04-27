// File: Material.cpp, Created on 2015. január 17. 7:50, Author: Vader

#include "vl/vm3/material.hpp"

namespace vl {
	namespace vm3 {

		MaterialPropertyBase::~MaterialPropertyBase() { }

		//------------------------------------------------------------------------------------------
		
		Material::Material() :
			name("--unnamed--"), shader("--unknown--") { }

		Material::Material(const std::string& name, const std::string& shader) :
			name(name), shader(shader) { }

		Material::Material(const Material& orig) :
			name(orig.name),
			shader(orig.shader) {
			MaterialPropertyCopier mpc(*this);
			orig.accept(mpc);
		}

		void Material::accept(MaterialPropertyVisitor& visitor) const {
			for (auto& property : properties) {
				property.second->accept(visitor, property.first);
			}
		}

		void Material::setName(const std::string& name) {
			this->name = name;
		}

		void Material::setShader(const std::string& shader) {
			this->shader = shader;
		}

		const std::string& Material::getName() const {
			return name;
		}

		const std::string& Material::getShader() const {
			return shader;
		}
	} //namespace vm3
} //namespace vl
