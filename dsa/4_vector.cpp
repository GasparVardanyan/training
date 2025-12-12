# include <cstddef>

#include <initializer_list>
# include <iostream>
# include <vector>

class MyClass {
public:
	explicit MyClass (int) {
		std::cout << "MyClass (int)" << std::endl;
	}

	MyClass (std::initializer_list<int> l) {
		if (1 == l.size ()) {
			std::cout << "MyClass {int}" << std::endl;
		}
	}

	explicit MyClass (std::initializer_list<double> l) {
		if (1 == l.size ()) {
			std::cout << "MyClass {double}" << std::endl;
		}
	}
};

int main ()
{
	std::vector <int> squares (100);

	for (std::size_t i = 0; i < squares.size (); i++) {
		squares [i] = i * i;
	}

	for (std::size_t i = 0; i < squares.size (); i++) {
		std::cout << i << " " << squares [i] << std::endl;
	}

	std::vector <int> v1 (10);
	std::vector <int> v2 {10};

	std::cout << v1.size () << " - " << v1 [0] << std::endl;
	std::cout << v2.size () << " - " << v2 [0] << std::endl;

	int sum = 0;
	for (std::size_t i = 0; i < squares.size (); i++) {
		sum += squares [i];
	}

	std::cout << "sum: " << sum << std::endl;

	sum = 0;

	for (auto x : squares) {
		sum += x;
	}

	std::cout << "sum: " << sum << std::endl;

	std::cout << "====================" << std::endl;

	MyClass c1 (5);
	MyClass c2 {5};
	MyClass c3 = {5};
	MyClass c4 {5.0};
	// MyClass c5 = {5.0}; // error, the constructor is explicit
}
