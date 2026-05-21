# include <algorithm>
# include <functional>
# include <iostream>
# include <iterator>
# include <set>
# include <utility>

# include "26_set.h"



static void foo () {
	const std::set <int, std::greater <>> s1 {1, 2, 3, 4, 3, 2, 1};
	const set <int, std::greater <>> s2 {1, 2, 3, 4, 3, 2, 1};
	auto s3 = s2;
	decltype (s3) s4;
	s4 = std::move (s3);

	std::ranges::copy (s1, std::ostream_iterator <int> (std::cout, ", "));
	std::cout << '\n';
	std::copy (s4.cbegin (), s4.cend (), std::ostream_iterator <int> (std::cout, ", "));
	std::cout << '\n';

	// std::ranges::begin (s4);

	for (int i : s4) {
		std::cout << i << ", ";
	}
	std::cout << '\n';

	std::cout << s4 << '\n';
}

int main () {
	foo ();
}
