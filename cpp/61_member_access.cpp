# include <iostream>

struct SomeStruct {
	int a;
	int b;
	void foo () {}

	static inline int c;
	static void bar () {}
};

int main () {
	SomeStruct var;
	std::cout << var.a << std::endl;
	var.b = 20;
	var.foo ();

	SomeStruct * p = & var;
	std::cout << p->a << std::endl;
	(* p).b = 30;
	p->foo ();
	(* p).foo ();

	std::cout << SomeStruct::c << std::endl;
	SomeStruct::bar ();

	std::cout << var.c << std::endl;
	p->bar ();
}
