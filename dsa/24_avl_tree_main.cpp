# include <iostream>
# include <iterator>
# include <sstream>

# include "24_avl_tree.h"

int main () {
	avl_tree <int> avlt;
	avlt.insert (50);
	avlt.insert (25);
	avlt.insert (75);
	avlt.insert (20);
	avlt.insert (30);
	avlt.insert (100);
	avlt.insert (27);
	avlt.insert (28);

	// return 0;
	std::cout << avlt << std::endl;
	return 0;
	stack <const decltype (avlt)::node *> path;

	// avlt.root ()->preorder_traverse (
	// 	[&path]
	// 	(auto * node, auto * parent) -> void {
	// 		if (nullptr != parent) {
	// 			while (path.top () != parent) {
	// 				path.pop ();
	// 			}
	// 		}
	// 		path.push (node);
	//
	// 		std::size_t depth = path.size ();
	//
	// 		for (std::size_t i = 0; i < depth; i++) {
	// 			std::cout << "  ";
	// 		}
	//
	// 		if (nullptr != parent) {
	// 			std::cout << "|- ";
	// 		}
	//
	// 		std::cout << node->data;
	// 		std::cout << " (" << node->data.height << ")";
	//
	// 		if (nullptr != parent) {
	// 			if (nullptr == parent->left || nullptr == parent->right) {
	// 				if (node == parent->left) {
	// 					std::cout << " (L)";
	// 				}
	// 				else {
	// 					std::cout << " (R)";
	// 				}
	// 			}
	// 		}
	//
	// 		std::cout << std::endl;
	// 	}
	// );


	return 0;
	int x = 10;
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (x);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (20);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (5);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (25);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (15);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (5);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (2);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (7);
	avlt.insert (1);
	avlt.insert (3);

	avlt.remove (5); // 10, 20, 25, 15, 5
	std::stringstream ss;
	ss << "TREE:\n" << avlt << std::endl;
	std::cout << ss.str () << std::endl;
	for (int i : (vector <int>) avlt) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	if (false == avlt.empty ()) {
		std::cout << avlt.findMin ()->data << " - " << avlt.findMax ()->data << std::endl;
	}

	std::vector <int> v;
	avlt.dump (std::back_inserter(v));
	for (int i : v) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	std::cout << avlt.at (20)->data << std::endl;
}
