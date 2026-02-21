# include <gtest/gtest.h>
#include <limits>

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

TEST(AVL, BruteForceTest)
{
	std::srand (std::time (NULL));

	for (int iter = 0; iter < 1000; iter++) {
		int n = std::rand () % 100'000;

		std::vector <int> myVec (n);

		for (int & item : myVec) {
			item = std::rand () % std::numeric_limits<int>::max ();
		}

		avl_tree <int> avlt;
		for (int i : myVec) {
			avlt.insert (i);
		}

		std::cout << myVec.size () << " - " << avlt.size () << std::endl;

		bool test = verify_avlt (avlt);

		// print_avl (avlt);

		EXPECT_EQ (true, test);
	}
}
