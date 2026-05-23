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
	// NOLINTNEXTLINE
	std::copy (s4.cbegin (), s4.cend (), std::ostream_iterator <int> (std::cout, ", "));
	std::cout << '\n';

	// std::ranges::begin (s4);

	for (int i : s4) { // NOLINT
		std::cout << i << ", ";
	}
	std::cout << '\n';

	std::cout << s4 << '\n';
}



static void bar () {
	std::set <int> s;

	auto print = [&s] () -> void {
		for (const int i : s) {
			std::cout << i << ", ";
		}
		std::cout << '\n';
	};

	for (int i = 0; i < 10; i++) {
		s.insert (s.end (), i);
	}

	s.erase (5);

	print ();

	auto i1 = s.begin (), i2 = s.end ();
	++i1, ++i1, --i2, --i2;

	s.erase (i1, i2);

	for (const int i : s) {
		std::cout << i << ", ";
	}
	std::cout << '\n';

	s.erase (s.find (8));

	print ();

	for (int i = 0; i < 10; i++) {
		s.insert (s.end (), i);
	}

	print ();

	auto it = s.erase (s.find (5));
	print ();
	std::cout << * it << '\n';
}



int main () {
	foo ();
	bar ();
}
