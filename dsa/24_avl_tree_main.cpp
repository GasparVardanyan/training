# include <iostream>
# include <iterator>
# include <sstream>

# include "20_vector.h"
# include "22_stack.h"
# include "24_avl_tree.h"

template <typename T>
bool verify_avlt (const avl_tree <T> & tree) {
	if (0 == tree.size ()) {
		return true;
	}

	stack <const typename avl_tree <T>::node *> node_stack;
	node_stack.push (tree.root ());

	bool ok = true;

	while (false == node_stack.empty ()) {
		auto n = node_stack.top ();
		node_stack.pop ();

		std::size_t lh, rh;

		if (nullptr != n->left) {
			lh = n->left->data.height_plus_one;
			node_stack.push (n->left);
		}
		else {
			lh = 0;
		}

		if (nullptr != n->right) {
			rh = n->right->data.height_plus_one;
			node_stack.push (n->right);
		}
		else {
			rh = 0;
		}

		if (lh > rh) {
			if (n->data.height_plus_one != lh + 1) {
				ok = false;
				break;
			}
			if (lh - rh > 1) { // probably unneccessary
				ok = false;
				break;
			}
		}
		else if (rh > lh) {
			if (n->data.height_plus_one != rh + 1) {
				ok = false;
				break;
			}
			if (rh - lh > 1) {
				ok = false;
				break;
			}
		}
		else {
			if (n->data.height_plus_one != lh + 1) {
				ok = false;
				break;
			}
		}
	}

	if (true == ok) {
		vector <typename avl_tree <T>::value_type> v = tree;
		auto v2 = v;

		std::sort (v2.begin (), v2.end ());

		if (v != v2) {
			ok = false;
		}
	}

	return ok;
}

template <typename T>
void print_avl (const avl_tree <T> & tree) {
	tree.root ()->preorder_traverse (
		[path = stack <const typename std::decay_t <decltype (tree)>::node *> ()]
		(auto * node, auto * parent) mutable -> void {
			if (nullptr != parent) {
				while (path.top () != parent) {
					path.pop ();
				}
			}
			path.push (node);

			std::size_t depth = path.size () - 1;

			for (std::size_t i = 0; i < depth; i++) {
				std::cout << "  ";
			}

			if (nullptr != parent) {
				std::cout << "|- ";
			}

			std::cout << node->data;
			std::cout << " (" << ((int) node->data.height_plus_one - 1) << ")";

			if (nullptr != parent) {
				if (nullptr == parent->left || nullptr == parent->right) {
					if (node == parent->left) {
						std::cout << " (L)";
					}
					else {
						std::cout << " (R)";
					}
				}
			}

			std::cout << std::endl;
		}
	);
}

int main () {
	avl_tree <int> avlt;
	avlt.insert (50);
	avlt.insert (25);
	avlt.insert (75);
	avlt.insert (20);
	avlt.insert (30);
	avlt.insert (100);
	avlt.insert (27);
	decltype (avlt) avlt2 = avlt;
	avlt.insert (28);

	std::cout << avlt << std::endl;
	std::cout << (avlt == avlt2) << std::endl;

	std::cout << "====================" << std::endl;

	print_avl (avlt);
	std::cout << "V: " << verify_avlt (avlt) << std::endl;
	// return 0;
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

	// avlt.remove (5); // 10, 20, 25, 15, 5
	std::stringstream ss;
	ss << "TREE:\n" << avlt << std::endl;
	std::cout << ss.str () << std::endl;
	print_avl (avlt);
	std::cout << "V: " << verify_avlt (avlt) << std::endl;
	for (int i : (vector <int>) avlt) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	if (false == avlt.empty ()) {
		std::cout << avlt.findMin ()->data << " - " << avlt.findMax ()->data << std::endl;
	}

	std::vector <int> v;
	avlt.dumpSorted (std::back_inserter(v));
	for (int i : v) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	std::cout << avlt.at (20)->data << std::endl;
}
