// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

#include <iostream>

struct A {
};

struct B {
	friend A;
//private:
//	int x;
};

struct D : A {
	void foo(B& /*v*/){
//		v.x++;
	}
};

int main(int, char **) {

	D d;
	B b;

	d.foo(b);





	return 0;
}
