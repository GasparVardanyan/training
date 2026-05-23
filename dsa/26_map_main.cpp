# include <algorithm>
# include <functional>
# include <iostream>
# include <map>
# include <numeric>
# include <random>
# include <stdexcept>
# include <utility>

# include "20_vector.h"

// NOLINTBEGIN(unused-includes,misc-include-cleaner)
# include "23_binary_search_tree.h"		// IWYU pragma: keep
# include "24_avl_tree.h"				// IWYU pragma: keep
# include "25_splay_tree.h"				// IWYU pragma: keep
// NOLINTEND(unused-includes,misc-include-cleaner)

# include "26_map.h"

template <typename ... Ts>
// using map_tree = binary_search_tree <Ts ...>;
using map_tree = avl_tree <Ts ...>;
// using map_tree = splay_tree <Ts ...>;

static void foo () {
	map <int, int, std::greater <>, map_tree> m; {
		vector <int> v (12);
		std::iota (v.begin (), v.end (), 1); // NOLINT(boost-use-ranges)
		std::mt19937 rng (std::random_device {} ());
		std::ranges::shuffle (v, rng);
		std::ranges::for_each (v, [& m] (int i) -> void {
			m.insert (i, i + 1);
		});
	}

	std::cout << m.contains (10) << '\n';
	m.remove (10);
	std::cout << m.contains (10) << '\n';

	std::cout << m << '\n';

	m.at (4) = 44;
	m [8] = 88;

	for (auto [k, v] : m) {
		std::cout << "key: " << k << ", value: " << v << '\n';
	}
}



template <template <typename, typename> typename Map>
static void bar_ () {
	Map <int, int> m;
	m.insert (std::pair <int, int> {1, 2});
	m.insert (std::pair <int, int> {3, 4});
	m [10] = 100;
	m.at (10) = 200;

	for (auto [k, v] : m) {
		std::cout << "key: " << k << ", value: " << v << std::endl;
	}

	try {
		std::cout << m.at (100) << std::endl;
	}
	catch (const std::out_of_range & e) {
		std::cout << "THROWN: " << e.what () << '\n';
	}
}

static void bar () {
	bar_ <std::map> ();
	std::cout << "====================" << '\n';
	bar_ <map> ();
}



int main () { // NOLINT(bugprone-exception-escape)
	// cppcheck-suppress throwInEntryPoint
	foo ();
	std::cout << "====================" << '\n';
	bar ();
}
