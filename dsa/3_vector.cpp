# include <cstddef>

# include <iostream>
# include <vector>

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
}
