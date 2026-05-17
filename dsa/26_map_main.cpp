# include <algorithm>
# include <iostream>
# include <numeric>
# include <random>
# include <type_traits>

# include "20_vector.h"
# include "26_map.h"
# include "23_binary_search_tree.h"
# include "24_avl_tree.h"
# include "25_splay_tree.h"

# include <concepts>
# include <type_traits>

template <typename ... Ts>
// using map_tree = binary_search_tree <Ts ...>;
using map_tree = splay_tree <Ts ...>;
// using map_tree = avl_tree <Ts ...>;

int main () {
	map <int, int, std::less <int>, map_tree> m; {
		vector <int> v (12);
		std::iota (v.begin (), v.end (), 1);
		std::shuffle (v.begin (), v.end (), std::random_device {});
		for (int i : v) {
			m.insert (i, i + 1);
		}
	}
	std::cout << m.contains (10) << std::endl;
	m.remove (10);
	std::cout << m.contains (10) << std::endl;

	std::cout << m << std::endl;

	for (auto [k, v] : m) {
		std::cout << "key: " << k << ", value: " << v << std::endl;
	}
}
