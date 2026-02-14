# include <iostream>
#include <utility>

typedef unsigned long long ull;

constexpr ull calcPower1 (const ull & b, const ull & p) {
	ull r = 1;

	for (ull i = 0; i < p; i++) {
		r *= b;
	}

	return r;
}

constexpr ull calcPower2 (const ull & b, const ull & p) {
	if (0 == p) {
		return 1;
	}
	else {
		return b * calcPower2 (b, p - 1);
	}
}

template <unsigned base, unsigned pow, typename = std::make_index_sequence <pow>>
struct CalcPower;

template <unsigned base, unsigned pow, std::size_t ... indices>
struct CalcPower <base, pow, std::index_sequence <indices ...>>
	: std::integral_constant <unsigned, (((void) indices, base) * ... * 1)> {};

int main () {
	std::cout << calcPower1 (2, 6) << std::endl; // calculated compile time
	std::cout << calcPower2 (2, 6) << std::endl; // calculated compile time

	int p; {
		std::cout << "pow: ";
		std::cin >> p;
	}

	std::cout << calcPower1 (2, p) << std::endl; // calculated run time
	std::cout << calcPower2 (2, p) << std::endl; // calculated run time

	std::cout << CalcPower <2, 6>::value << std::endl;
	// std::cout << CalcPower <2, p>::value << std::endl; // works compile time only
}
