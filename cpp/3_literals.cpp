#include <cstddef>
# include <iostream>

struct S {
	int x;
	S & operator= (const S & o) {
		this->x = o.x;

		return * this;
	}

	void print () const {
		std::cout << this->x << std::endl;
		* const_cast <int *> (& this->x) = 300;
	}
};

int * f (int * p) {return p;}

template <class T>
T * g (T * p) {return p;}

std::nullptr_t h (std::nullptr_t p) {return p;}

int main ()
{
	S s1 = { .x = 10 };
	S s2 = { .x = 20 };

	std::cout << s1.x << std::endl;
	std::cout << (s1 = s2).x << std::endl;
	std::cout << s1.x << std::endl;

	s1.print ();
	s1.print ();

	unsigned long long l1 = 8888888888888888888ull; // C++11
	unsigned long long l2 = 8'88888'8'8'88888'8uLL; // C++14
	unsigned long long l3 = 888'888'888'888'888LLU;

	int * x = nullptr;
	std::nullptr_t np;

	if (l1 || l2 || l3 || x || np != nullptr) {
	}

	f (nullptr);
	g <std::nullptr_t> (nullptr);
	h (nullptr);
}
