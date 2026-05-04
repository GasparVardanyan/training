# include "23_binary_search_tree.h"
# include "23_binary_tree_node.h"
# include "25_splay_tree.h"
# include "26_set.h"
# include "26_map.h"

int main () {
	map <int, int> m;
	m.insert (1, 2);
	m.insert (2, 3);
	m.insert (3, 4);
	std::cout << m << std::endl;

	for (const auto [k, v] : m) {
		std::cout << "key: " << k << std::endl;
		std::cout << "value: " << v << std::endl;
	}

	std::cout << is_tree_v <binary_search_tree <int>> << std::endl;
	std::cout << is_tree_v <binary_search_tree <int, std::less <int>, true>> << std::endl;
	std::cout << m.contains (1) << std::endl;
	return 0;


	// set <int, binary_search_tree <int>> s;
	set <int> s;
	// static_assert (is_tree_v <binary_search_tree <int>>, "XXX");
	s.insert (10);
	s.insert (11);
	s.insert (12);
	s.insert (12);
	s.insert (12);
	s.insert (12);
	std::cout << s.contains (9) << std::endl;
	std::cout << s.contains (10) << std::endl;
	std::cout << s.contains (11) << std::endl;
	std::cout << s.contains (12) << std::endl;
	std::cout << s.contains (13) << std::endl;

	std::cout << "====================" << std::endl;

	for (int i : s) {
		std::cout << i << std::endl;
	}

	std::cout << "====================" << std::endl;

	s.remove (20);
	s.remove (12);

	for (int i : s) {
		std::cout << i << std::endl;
	}

	std::cout << "====================" << std::endl;

	const auto & r = * s.root ();
	auto it = r.begin ();
	for (; it != r.end (); it++) {
		std::cout << * it << std::endl;
	}
	auto itt = r.end ();
	std::cout << "====================" << std::endl;

	std::cout << * --r.end () << std::endl;


	// for (int i : s) {
	// 	std::cout << i << std::endl;
	// }
}
