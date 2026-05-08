# include <iostream>

int pow (int base, unsigned p) {
	int r = 1;

	while (0 != p) {
		if (p & 1) {
			r *= base;
		}
		base *= base;

		p >>= 1;
	}

	return r;
}

int pow_recursive (int base, unsigned p) {
	if (p == 0) {
		return 1;
	}
	else if (p == 1) {
		return base;
	}
	else {
		if (0 == (p & 1)) {
			// int _b = pow_recursive (base, p >> 1);
			// return _b * _b;
			return pow_recursive (base * base, p >> 1);
		}
		else {
			// int _b = pow_recursive (base, p >> 1);
			// return base * _b * _b;

			return base * pow_recursive (base * base, p >> 1);
		}
	}
}

int main ()
{
	std::cout << pow (3, 4) << std::endl;
	std::cout << pow_recursive (3, 4) << std::endl;
}
