# include <gtest/gtest.h>

# include <algorithm>
# include <numeric>
# include <random>

# include "20_vector.h"
# include "22_stack.h"
# include "24_avl_tree.h"

template <typename T>
bool verify_avl_balance (const avl_tree <T> & tree) {
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
			if (lh - rh > 1) {
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

	return ok;
}

template <typename T>
bool verify_avl_order (const avl_tree <T> & tree) {
	bool ok = true;

	vector <typename avl_tree <T>::value_type> v = tree;
	auto v2 = v;

	std::sort (v2.begin (), v2.end ());

	if (v != v2) {
		ok = false;
	}

	return ok;
}

template <typename T = int>
std::vector <T> random_permutation (std::size_t n, std::mt19937 & rng) {
	std::vector <T> values (n);
	std::iota (values.begin (), values.end (), 0);
	std::ranges::shuffle (values, rng);

	return values;
}

TEST(AVL, InsertBruteForceBalance)
{
	for (int iter = 0; iter < 1000; iter++) {
		std::mt19937 rng (std::random_device {} ());
		std::size_t n = std::uniform_int_distribution <std::size_t> (80'000, 100'000) (rng);
		std::vector <int> values = random_permutation (n, rng);

		avl_tree <int> avlt;

		for (int item : values) {
			avlt.insert (item);

			EXPECT_TRUE (verify_avl_balance (avlt));
		}
	}
}

TEST(AVL, InsertBruteForceOrder)
{
	for (int iter = 0; iter < 1000; iter++) {
		std::mt19937 rng (std::random_device {} ());
		std::size_t n = std::uniform_int_distribution <std::size_t> (80'000, 100'000) (rng);
		std::vector <int> values = random_permutation (n, rng);

		avl_tree <int> avlt;

		for (int item : values) {
			avlt.insert (item);

			EXPECT_TRUE (verify_avl_order (avlt));
		}
	}
}

TEST(AVL, RemoveBruteForceBalance)
{
	for (int iter = 0; iter < 1000; iter++) {
		std::mt19937 rng (std::random_device {} ());
		std::size_t n = std::uniform_int_distribution <std::size_t> (80'000, 100'000) (rng);
		std::vector <int> values = random_permutation (n, rng);

		avl_tree <int> avlt;

		for (int item : values) {
			avlt.insert (item);
		}

		for (int i : values) {
			avlt.remove (i);

			EXPECT_TRUE (verify_avl_balance (avlt));
		}

		EXPECT_TRUE (avlt.empty ());
	}
}

TEST(AVL, RemoveBruteForceOrder)
{
	for (int iter = 0; iter < 1000; iter++) {
		std::mt19937 rng (std::random_device {} ());
		std::size_t n = std::uniform_int_distribution <std::size_t> (80'000, 100'000) (rng);
		std::vector <int> values = random_permutation (n, rng);

		avl_tree <int> avlt;

		for (int item : values) {
			avlt.insert (item);
		}

		for (int i : values) {
			avlt.remove (i);

			EXPECT_TRUE (verify_avl_order (avlt));
		}

		EXPECT_TRUE (avlt.empty ());
	}
}

TEST(AVL, SingleTreeFullInsertRemoveBalance)
{
	std::mt19937 rng (std::random_device {} ());
	std::size_t n = std::uniform_int_distribution <std::size_t> (80'000, 100'000) (rng);
	std::vector <int> values = random_permutation (n, rng);

	avl_tree <int> avlt;

	for (int item : values) {
		avlt.insert (item);

		EXPECT_TRUE (verify_avl_balance (avlt));
	}

	for (int i : values) {
		avlt.remove (i);

		EXPECT_TRUE (verify_avl_balance (avlt));
	}

	EXPECT_TRUE (avlt.empty ());
}

TEST(AVL, SingleTreeFullInsertRemoveOrder)
{
	std::mt19937 rng (std::random_device {} ());
	std::size_t n = std::uniform_int_distribution <std::size_t> (80'000, 100'000) (rng);
	std::vector <int> values = random_permutation (n, rng);

	avl_tree <int> avlt;

	for (int item : values) {
		avlt.insert (item);

		EXPECT_TRUE (verify_avl_order (avlt));
	}

	for (int i : values) {
		avlt.remove (i);

		EXPECT_TRUE (verify_avl_order (avlt));
	}

	EXPECT_TRUE (avlt.empty ());
}
