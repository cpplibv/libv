// File: Visitor.hpp, Created on 2014. április 11. 23:19, Author: Vader

#pragma once

// external
// vLib
// std
#include <stdexcept>
// project

namespace libv {

struct DynamicVisitorBase {
	virtual ~DynamicVisitorBase() { }
};

template<typename Type>
struct DynamicVisitor : public virtual DynamicVisitorBase {
	virtual void operator()(Type* v) = 0;
};

struct DynamicVisitableBase {
	virtual void accept(DynamicVisitorBase*) = 0;
};

template<typename Derived>
struct DynamicVisitable : public DynamicVisitableBase {
	void accept(DynamicVisitorBase* visitor) {
		if (auto ptr = dynamic_cast<DynamicVisitor<Derived>*> (visitor)) {
			(*ptr)(static_cast<Derived*> (this));
		} else {
			throw std::invalid_argument(""); //TODO P5: give it a message
		}
	}
};

} //namespace libv