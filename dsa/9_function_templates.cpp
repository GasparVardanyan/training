# include <cstddef>
# include <iostream>
# include <vector>

const int & findMax (const std::vector <int> & a) {
	std::cout << "int findMax" << std::endl;

	std::size_t maxIndex = 0;

	for (std::size_t i = 1; i < a.size (); i++) {
		if (a [maxIndex] < a [i]) {
			maxIndex = i;
		}
	}

	return a [maxIndex];
}

template <typename Comparable>
const Comparable & findMax (const std::vector <Comparable> & a) {
	std::cout << "Comparable findMax" << std::endl;

	std::size_t maxIndex = 0;

	for (std::size_t i = 1; i < a.size (); i++) {
		if (a [maxIndex] < a [i]) {
			maxIndex = i;
		}
	}

	return a [maxIndex];
}

struct IntHolder {
	int val;
	bool operator< (const IntHolder & o) const {
		return val < o.val;
	}
};

int main ()
{
	std::vector <int> a {12, 22, 1};
	std::vector <double> b {12, 22, 1};
	std::vector <IntHolder> c {{12}, {22}, {1}};

	(void) findMax (a);
	(void) findMax (b);
	(void) findMax (c);
}
