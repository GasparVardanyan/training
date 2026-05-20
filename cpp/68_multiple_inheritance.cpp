# include <iostream>

namespace {

struct A {};
struct B {};
struct C : A, B {};

struct Base1 {
	inline void func () {
		std::cout << "Base1::func ()\n";
	}
	inline void func2 () {
		std::cout << "Base1::func2 ()\n";
	}
};

struct Base2 {
	inline void func () {
		std::cout << "Base2::func ()\n";
	}
	inline void func2 () {
		std::cout << "Base2::func2 ()\n";
	}
};

struct Derived : Base1, Base2 {
	using Base1::func2;
};

} // end anonymous namespace

int main () {
	Derived obj;
	// obj.func (); // error
	obj.func2 ();

	obj.Base1::func ();
	obj.Base2::func ();

	static_cast <Base1 &> (obj).func ();
	static_cast <Base2 &> (obj).func ();
}
