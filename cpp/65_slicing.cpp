# include <iostream>

# define ENABLE_VIRTUAL 1

# if 1 == ENABLE_VIRTUAL
#	define VIRTUAL_TOGGLE virtual
# else
#	define VIRTUAL_TOGGLE
# endif

struct Base {
	int value = 10;

	Base () {}

	VIRTUAL_TOGGLE void print () const {
		std::cout << "Base\n";
	}

# if 1 == ENABLE_VIRTUAL
// 	Base ( const Base & ) = delete;
// 	Base & operator= ( const Base & ) = delete;
# endif
};

struct Derived : Base {
	int extra = 20;

	Derived () {}

	void print () const {
		std::cout << "Derived\n";
	}
};

int main () {
	Derived d;
	Base b = d;


	std::cout << "sizeof (d): " << sizeof (d) << '\n';
	std::cout << "sizeof (b): " << sizeof (b) << '\n';

	std::cout << "d.print (): ";
	d.print ();

	std::cout << "b.print (): ";
	b.print ();
}
