# include <iostream>
# include <utility>

int gcd (int m, int n) {
	while (n != 0) {
		m %= n;
		std::swap (m, n);
	}

	return m;
}

int main ()
{
	std::cout << gcd (1989, 1590) << std::endl;
}
