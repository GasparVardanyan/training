# include <iostream>

class A {
public:
	int x () { return 5; }
	int y () { return 10; }
};

class B : private A {
public:
	using A::x;
};

class C : public A {
private:
	using A::y;
};

int main () {
	A a;
	std::cout << a.x () << std::endl;
	std::cout << a.y () << std::endl;

	B b;
	std::cout << b.x () << std::endl;
	// std::cout << b.y () << std::endl; // error: 'y' is a private member of 'A'

	C c;
	std::cout << c.x () << std::endl;
	// std::cout << c.y () << std::endl; // error: 'y' is a private member of 'C'

	A & ar = a;						(void) ar;
	// A & br = b; // error: Cannot cast 'B' to its private base class 'A'
	A & cr = c;						(void) cr;
}
