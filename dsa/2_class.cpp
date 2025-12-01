# include <iostream>

# include "2_IntCell.h"

class A {
public:
	A (int x) : m_x (x) { (void) m_x; }
private:
	int m_x;
};

class B {
public:
	explicit B (int x) : m_x (x) { (void) m_x; }
private:
	int m_x;
};

int main ()
{
	IntCell ic;
	ic.write (5);
	std::cout << ic.read () << std::endl;

	{ A a = 10; }
	// { B b = 20; }

	{ A a (10); }
	{ B b (20); }

	{ A a { 10 }; }
	{ B b { 20 }; }
}
