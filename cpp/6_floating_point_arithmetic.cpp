# include <cstdlib>

# include <iostream>
# include <limits>

int main ()
{
	std::cout << std::boolalpha;

	float total = 0;

	for (int i = 0; i < 200; i++) {
		total += 0.01;
	}

	std::cout << "total: " << total << ", == 2 ?: " << (total == 2) << std::endl;
	std::cout << "check2: " << (std::abs (2 - total) <= std::numeric_limits<double>::epsilon()) << std::endl;

	double a = 0.1;
	double b = 0.2;
	double c = 0.3;

	if (a + b == c) {
		std::cout << "This Computer is Magic!" << std::endl;
	}
	else {
		std::cout << "This Computer is pretty normal, all things considered." << std::endl;
	}
}
