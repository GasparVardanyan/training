# include <iostream>
# include <vector>



namespace n1 {
class foo {
	int x = 0;

public:
	int get_x () { return x; }
	void set_x (int new_x) { x = new_x; }
};
}



namespace n2 {
class foo;
class bar {
public:
	bar (foo & f) {
		(void) f;
	}
	bar () {}
};

void baz ();
class baz;

class foo {
	bar b;
	friend class baz;
	bar b2;

public:
	foo ()
		: b (bar (* this))
	{}
};
}



namespace ns3 {
template <class T>
const T & min (const T & x, const T & y) {
	return x <= y ? x : y;
}
}



namespace ns4 {
template <template <class _> class C>
struct foo {
	C <int> intCon;
	C <double> doubleCon;
};
}



namespace ns5 {
template <class T>
class foo {
public:
	T * t;
};

class bar;
void bar ();

foo <class bar> x;

void bar () {
	std::cout << "void bar ()" << std::endl;
}

class bar {
public:
	int x = 10;
};
}



namespace ns6 {
enum E1 {
	E1V1, E1V2, E1V3
};

E1 e1 = E1V2;

enum class E2 {
	V1, V2, V3
};

E2 e2 = E2::V2;

enum class E3 : unsigned {
	V1, V2, V3
};

E3 e3 = E3::V2;
}



int main () {
	{
		using namespace n2;

		foo f;
		bar b (f);
	}
	{
		using namespace ns3;

		int x = 10, y = 20;
		int z = min (x, y);
		(void) z;
	}
	{
		using namespace ns4;
		foo <std::vector> f;
		f.intCon.push_back (10);
		f.intCon.push_back (20);
		f.doubleCon.push_back (1.1);
		f.doubleCon.push_back (2.2);
	}
	{
		using namespace ns5;
		class bar b;
		x.t = & b;
		std::cout << x.t->x << std::endl;
		bar ();
		x.t = nullptr;
	}
}
