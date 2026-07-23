# include <concepts>
# include <iostream>

# define ENABLE_VIRTUAL 0

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

	template <std::derived_from <Base> D>
	Base ( const D & ) = delete;
	template <std::derived_from <Base> D>
	Base & operator= ( const D & ) = delete;
};

struct Derived : Base {
	int extra = 20;

	Derived () {}

	void print () const {
		std::cout << "Derived\n";
	}
};

void func(Base) { }

int main () {
	Derived d;
	// Base b = d;
	Base b = {};
	Base b2 = {};
	b2 = b;
	Derived d2;
	d2 = d;
	// func (d); // 1. Call to deleted constructor of 'Base' [ovl_deleted_init]
	func (b);


	std::cout << "sizeof (d): " << sizeof (d) << '\n';
	std::cout << "sizeof (b): " << sizeof (b) << '\n';

	std::cout << "d.print (): ";
	d.print ();

	std::cout << "b.print (): ";
	b.print ();
}
