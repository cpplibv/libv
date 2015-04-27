// File: Material.hpp, Created on 2015. január 17. 7:50, Author: Vader

#pragma once

// std
#include <map>
#include <memory>
// pro
#include "material_property_proxy.hpp"
#include "material_property_visitor.hpp"

namespace vl {
	namespace vm3 {
		struct MaterialPropertyBase {
			virtual void accept(MaterialPropertyVisitor& visitor, const std::string& name) const = 0;
			virtual ~MaterialPropertyBase();
		};

		template<typename T>
		struct MaterialProperty : public MaterialPropertyBase {
			T data;
			void accept(MaterialPropertyVisitor& visitor, const std::string& name) const {
				visitor.visit(name, data);
			}
			template<typename... Args>
			MaterialProperty(Args&&... args) : data(std::forward<Args>(args)...) { }
		};
		
		//------------------------------------------------------------------------------------------

		class MaterialSerialization;
		class Material {
			friend class MaterialSerialization;
			
			std::string name;
			std::string shader;
			std::map<std::string, std::unique_ptr<MaterialPropertyBase>> properties;

		public:
			Material();
			Material(const std::string& name, const std::string& shader);
			Material(const Material& orig);
			//TODO P5: operator=

			void setName(const std::string& name);
			void setShader(const std::string& shader);
			const std::string& getName() const;
			const std::string& getShader() const;
			void remove(const std::string& name) {
				properties.erase(name);
			}
			void accept(MaterialPropertyVisitor& visitor) const;
			template<typename T>
			void set(const std::string& name, const T& data) {
				if (auto it = get<T>(name)) {
					it = data;
				} else {
					std::unique_ptr<MaterialPropertyBase> ptr = std::make_unique<MaterialProperty<T>> (data);
					properties.emplace(name, std::move(ptr));
				}
			}
			template<typename T>
			bool get(const std::string& name, T& target) const {
				auto it = properties.find(name);
				if (it != properties.end()) {
					if (auto result = dynamic_cast<MaterialProperty<T>*> (it->second.get())) {
						target = result->data;
						return true;
					}
				}
				return false;
			}
			template<typename T>
			MaterialPropertyProxy<T> get(const std::string& name) {
				auto it = properties.find(name);
				if (it != properties.end()) {
					if (auto result = dynamic_cast<MaterialProperty<T>*> (it->second.get())) {
						return &result->data;
					}
				}
				return nullptr;
			}
			template<typename T>
			inline const MaterialPropertyProxy<T> get(const std::string& name) const {
				return get<T>(name);
			}
		};
	} //namespace vm3
} //namespace vl