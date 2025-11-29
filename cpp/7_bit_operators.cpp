# include <bitset>
# include <iostream>
#include <stdexcept>

void doXORSwap (int & a, int & b) noexcept (false)
{
	if (& a != & b) {
		a ^= b;
		b ^= a;
		a ^= b;
	}
	else {
		throw std::invalid_argument ("a and b have to be distinct objects");
	}
}

int main ()
{
	int a = 5;
	int b = 12;
	int c = a | b;
	int d = a ^ b;
	int e = a & b;

	std::cout << a << " = " << std::bitset<sizeof (a) * 8> (a) << std::endl;
	std::cout << b << " = " << std::bitset<sizeof (b) * 8> (b) << std::endl;
	std::cout << c << " = " << std::bitset<sizeof (c) * 8> (c) << std::endl;
	std::cout << d << " = " << std::bitset<sizeof (d) * 8> (d) << std::endl;
	std::cout << e << " = " << std::bitset<sizeof (e) * 8> (e) << std::endl;

	std::cout << "a: " << a << std::endl;
	std::cout << "b: " << b << std::endl;

	a ^= b; // a = a ^ b
	b ^= a; // b = b ^ a = b ^ (a ^ b) = (b ^ b) ^ a = a
	a ^= b; // a = a ^ b = a ^ b ^ a = (a ^ a) ^ b = b

	try {
		doXORSwap(a, a);
	}
	catch (const std::invalid_argument & e) {
		std::cout << "ERROR: " << e.what () << std::endl;
	}

	std::cout << "a: " << a << std::endl;
	std::cout << "b: " << b << std::endl;

	doXORSwap(a, b);

	b = 0b1010101;
	std::cout << b << ": " << std::bitset<sizeof (b) * 8> (b) << std::endl;

	std::cout << std::bitset <sizeof (b) * 8> (b) << std::endl;
	b <<= 2;
	std::cout << std::bitset <sizeof (b) * 8> (b) << std::endl;
	b >>= 4;
	std::cout << std::bitset <sizeof (b) * 8> (b) << std::endl;

	std::cout << "b: " << b << std::endl;

	b = ~b;
	std::cout << std::bitset<sizeof (b) * 8> (b) << std::endl;

	std::cout << "b: " << b << std::endl;
}
