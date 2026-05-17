# include <algorithm>
# include <iostream>
# include <iterator>
# include <set>
# include <utility>

# include "26_set.h"

int main () {
	std::set <int> s1 {1, 2, 3, 4, 3, 2, 1};
	set <int> s2 {1, 2, 3, 4, 3, 2, 1};
	auto s3 = s2;
	decltype (s2) s4;
	s4 = std::move (s3);

	std::copy (s1.cbegin (), s1.cend (), std::ostream_iterator <int> (std::cout, ", "));
	std::cout << std::endl;
	std::copy (s4.cbegin (), s4.cend (), std::ostream_iterator <int> (std::cout, ", "));
	std::cout << std::endl;

	for (int i : s4) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
}
