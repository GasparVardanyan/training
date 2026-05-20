# include <iostream>

int main () {
	// A most-derived object contains only one A subobject if and only if
	// either A appears exactly once in the inheritance hierarchy, virtual or
	// non virtual, or every inheritance path leading to A is virtual with
	// respect to A.
	//
	// Furthermore consider the complete inheritance graph of a class AA.
	// For any base type AX, conceptually flatten the graph so that every
	// inheritance path to AX becomes a direct occurrence in a list.
	// Then:
	// * every non-virtual occurrence of AX remains a distinct AX subobject;
	// * all occurrences of AX reached through virtual inheritance are merged
	//   into a single shared AX subobject.
	// The resulting set exactly describes how many AX subobjects physically
	// reside inside an object of type AA.
	//
	// Directly virtually inherited classes are initialized only in the most
	// derived class.

	{
		struct A { int a; A (int param = 0) : a (param) {} };
		struct B1 : A { int b; B1 (int param) : A (param), b (param) {} };
		struct B2 : virtual A { int b; B2 (int param) : A (param), b (param) {} };
		struct C1 : B1 { int c; C1 (int param) : B1 (param), c (param) {} };
		struct C2 : B2 { int c; C2 (int param) : B2 (param), c (param) {} };

		C1 c1 (42);
		std::cout << c1.c << " " << c1.b << " " << c1.a << std::endl;
		C2 c2 (42);
		// since A is virtually inherited, it must be initialized in the
		// most derived class: C2
		// C2 doesn't pass the value to A's constructor. Even though
		// C2 passes it B2's constructor, and B2's constructor have
		// "A (param)" in it's initializer list, B2 doesn't call A's
		// constructor and doesn't pass param to A since A is virtually
		// inherited in B2 and the most derived class isn't B2
		std::cout << c2.c << " " << c2.b << " " << c2.a << std::endl;
	}

	std::cout << "====================" << std::endl;

	{
		struct A { int a = 1; };
		struct X : A {};
		struct Y : A {};
		struct AA : X, Y {};

		AA aa; // { AA, X, A, Y, A }
		// aa.A::a = 0; // error1 : Ambiguous conversion from derived class 'AA' to base class 'A':
						//          struct AA -> X -> A
						//          struct AA -> Y -> A
		aa.X::a = 1;
		aa.Y::a = 2;
		// aa.a = 3;	// error2 :  Non-static member 'a' found in multiple base-class subobjects of type 'A':
						//           struct AA -> X -> A
						//           struct AA -> Y -> A
		// aa.X::A::a = 4; // error1

		std::cout << aa.X::a << aa.Y::a << std::endl;
	}

	std::cout << "====================" << std::endl;

	{
		struct A { int a = 1; };
		struct X : virtual A {};
		struct Y : A {};
		struct AA : X, Y {};

		AA aa; // { AA, X, virtual A, Y, A }
		// aa.A::a = 0; // error1
		aa.X::a = 1;
		aa.Y::a = 2;
		// aa.a = 3; // error2

		std::cout << aa.X::a << aa.Y::a << std::endl;
	}

	std::cout << "====================" << std::endl;

	{
		struct A { int a = 10; };
		struct X : virtual A {};
		struct Y : virtual A {};
		struct AA : X, Y {};

		AA aa; // { AA, X, virtual A, Y }
		std::cout << aa.A::a << aa.X::a << aa.Y::a << aa.AA::a << aa.a << std::endl;
		aa.A::a = 0;
		std::cout << aa.A::a << aa.X::a << aa.Y::a << aa.AA::a << aa.a << std::endl;
		aa.X::a = 1;
		std::cout << aa.A::a << aa.X::a << aa.Y::a << aa.AA::a << aa.a << std::endl;
		aa.Y::a = 2;
		std::cout << aa.A::a << aa.X::a << aa.Y::a << aa.AA::a << aa.a << std::endl;
		aa.a = 3;
		std::cout << aa.A::a << aa.X::a << aa.Y::a << aa.AA::a << aa.a << std::endl;
	}
}
