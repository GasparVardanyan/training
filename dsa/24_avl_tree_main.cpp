# include <iostream>
# include <iterator>
# include <sstream>

# include "24_avl_tree.h"

int main () {
	avl_tree <int> bst;
	int x = 10;
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (x);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (20);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (5);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (25);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (15);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (5);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (2);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (7);
	bst.insert (1);
	bst.insert (3);

	bst.remove (5); // 10, 20, 25, 15, 5
	std::stringstream ss;
	ss << "TREE:\n" << bst << std::endl;
	std::cout << ss.str () << std::endl;
	for (int i : (vector <int>) bst) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	if (false == bst.empty ()) {
		std::cout << bst.findMin ()->data << " - " << bst.findMax ()->data << std::endl;
	}

	std::vector <int> v;
	bst.dump (std::back_inserter(v));
	for (int i : v) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	std::cout << bst.at (20)->data << std::endl;
}
