// File: DefaultRenderer.hpp, Created on 2014. április 11. 23:18, Author: Vader

#pragma once

namespace vl {
namespace ui {

class Quad;

class DefaultRenderer {
private:
	static DefaultRenderer& instance() {
		static DefaultRenderer inst;
		return inst;
	}
public:
	template<typename __Object>
	static void render(const __Object* obj) {
		instance()(obj);
	}
private:
	virtual void operator()(const Quad* v);
};

} //namespace ui
} //namespace vl