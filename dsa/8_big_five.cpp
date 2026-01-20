# include <iostream>
# include <utility>

# include "verbose_class.h"

C return_lvalue () {
	const C c {10}; // const prevents move
	return c;
}

C return_xvalue () {
	C c {10};
	return c;
}

class IntCell
{
public:
	explicit IntCell (int initialValue = 0)
		: m_storedValue (new int {initialValue})
	{}

	~IntCell () {
		delete m_storedValue;
	}

	IntCell (const IntCell & o)
		: m_storedValue (new int {* o.m_storedValue})
	{}

	IntCell (IntCell && o)
		: m_storedValue (o.m_storedValue)
	{
		o.m_storedValue = nullptr;
	}

	IntCell & operator= (const IntCell & o) {
		IntCell copy = o;
		std::swap (* this, copy);
		return * this;
	}

	IntCell & operator= (IntCell && o) {
		// std::swap (* this, o);	// will go infinite recursion if swap is
									// implemented with 3 move assignments
		std::swap (m_storedValue, o.m_storedValue);
		return * this;
	}

	int read () const {
		return * m_storedValue;
	}

	void write (int x) {
		* m_storedValue = x;
	}

private:
	int * m_storedValue;
};

int f ()
{
	IntCell a {2};
	IntCell b = a;
	// b = std::move (a);
	// std::cout << b.read () << std::endl;
	IntCell c;

	c = b;
	a.write(4);

	std::cout << a.read () << '\n' << b.read () << '\n' << c.read () << std::endl;

	return 0;
}

int main ()
{
	C c1 = return_lvalue ();
	std::cout << "====================" << std::endl;
	C c2 = return_xvalue ();
	std::cout << "====================" << std::endl;

	f ();
	std::cout << "====================" << std::endl;
}
