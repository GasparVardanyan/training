# include <gtest/gtest.h>

# include <algorithm>
# include <limits>
# include <numeric>
# include <random>

# include "20_vector.h"
# include "22_stack.h"
# include "24_avl_tree.h"

inline const std::size_t nIter = 100;

template <typename T>
static bool verify_avl_balance (const avl_tree <T> & tree) {
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
static bool verify_avl_order (const avl_tree <T> & tree) {
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
static std::vector <T> random_permutation (std::size_t n, std::mt19937 & rng) {
	std::vector <T> values (n);
	std::iota (values.begin (), values.end (), 0);
	std::ranges::shuffle (values, rng);

	return values;
}

struct TestParams {
	const std::size_t minCount = 80'000;
	const std::size_t maxCount = 100'000;
	const bool test_insert_balance = true;
	const bool test_insert_order = true;
	const bool test_remove_balance = true;
	const bool test_remove_order = true;
	const std::size_t check_interval = 0;
	const std::size_t iterations = 1;
};

template <const TestParams params>
void avl_test () {
	static_assert (0 < params.iterations && params.iterations < std::numeric_limits <std::size_t>::max (), "illegal iterations count");

	for (std::size_t i = 0; i < params.iterations; i++) {
		std::mt19937 rng (std::random_device {} ());
		std::size_t n = std::uniform_int_distribution <std::size_t> (params.minCount, params.maxCount) (rng);
		std::vector <int> values = random_permutation (n, rng);

		std::size_t counter = 0;

		avl_tree <int> avlt;

		for (int item : values) {
			avlt.insert (item);

			bool check = false;

			if constexpr (0 == params.check_interval) {
				check = true;
			}
			else if (++counter == params.check_interval) {
				counter = 0;
				check = true;
			}

			if (true == check) {
				if constexpr (true == params.test_insert_balance) {
					ASSERT_TRUE (verify_avl_balance (avlt));
				}
				if constexpr (true == params.test_insert_order) {
					ASSERT_TRUE (verify_avl_order (avlt));
				}
			}
		}

		std::cout << "Tree size: " << avlt.size () << ", height: " << (static_cast <int> (avlt.root ()->data.height_plus_one) - 1) << std::endl;

		if constexpr (true == params.test_remove_balance || true == params.test_remove_order) {
			for (int item : values) {
				avlt.remove (item);

				bool check = false;

				if constexpr (0 == params.check_interval) {
					check = true;
				}
				else if (++counter == params.check_interval) {
					counter = 0;
					check = true;
				}

				if (true == check) {
					if constexpr (true == params.test_remove_balance) {
						ASSERT_TRUE (verify_avl_balance (avlt));
					}
					if constexpr (true == params.test_remove_order) {
						ASSERT_TRUE (verify_avl_order (avlt));
					}
				}
			}

			ASSERT_TRUE (avlt.empty ());
		}
	}
}

TEST(SingleTreeFullInsertRemove, SmallTree)
{
	avl_test <{
		.minCount = 8'000,
		.maxCount = 10'000,
		.test_insert_balance = true,
		.test_insert_order = true,
		.test_remove_balance = true,
		.test_remove_order = true,
		.check_interval = 0,
		.iterations = 1
	}> ();
}

TEST(SingleTreeFullInsertRemove, BigTreeSmallIntervals)
{
	avl_test <{
		.minCount = 80'000,
		.maxCount = 100'000,
		.test_insert_balance = true,
		.test_insert_order = true,
		.test_remove_balance = true,
		.test_remove_order = true,
		.check_interval = 100,
		.iterations = 1
	}> ();
}

TEST(SingleTreeFullInsertRemove, Balance)
{
	avl_test <{
		.minCount = 80'000,
		.maxCount = 100'000,
		.test_insert_balance = true,
		.test_insert_order = false,
		.test_remove_balance = true,
		.test_remove_order = false,
		.check_interval = 0,
		.iterations = 1
	}> ();
}

TEST(SingleTreeFullInsertRemove, Order)
{
	avl_test <{
		.minCount = 80'000,
		.maxCount = 100'000,
		.test_insert_balance = false,
		.test_insert_order = true,
		.test_remove_balance = false,
		.test_remove_order = true,
		.check_interval = 0,
		.iterations = 1
	}> ();
}

TEST(BruteForceInsert, Balance)
{
	avl_test <{
		.minCount = 8'000,
		.maxCount = 10'000,
		.test_insert_balance = true,
		.test_insert_order = false,
		.test_remove_balance = false,
		.test_remove_order = false,
		.check_interval = 0,
		.iterations = nIter
	}> ();
}

TEST(BruteForceInsert, Order)
{
	avl_test <{
		.minCount = 8'000,
		.maxCount = 10'000,
		.test_insert_balance = false,
		.test_insert_order = true,
		.test_remove_balance = false,
		.test_remove_order = false,
		.check_interval = 0,
		.iterations = nIter
	}> ();
}

TEST(BruteForceRemove, Balance)
{
	avl_test <{
		.minCount = 8'000,
		.maxCount = 10'000,
		.test_insert_balance = false,
		.test_insert_order = false,
		.test_remove_balance = true,
		.test_remove_order = false,
		.check_interval = 0,
		.iterations = nIter
	}> ();
}

TEST(BruteForceRemove, Order)
{
	avl_test <{
		.minCount = 8'000,
		.maxCount = 10'000,
		.test_insert_balance = false,
		.test_insert_order = false,
		.test_remove_balance = false,
		.test_remove_order = true,
		.check_interval = 0,
		.iterations = nIter
	}> ();
}

TEST(BigTreeInsert, Balance)
{
	avl_test <{
		.minCount = 80'000,
		.maxCount = 100'000,
		.test_insert_balance = true,
		.test_insert_order = false,
		.test_remove_balance = false,
		.test_remove_order = false,
		.check_interval = 0,
		.iterations = 1
	}> ();
}

TEST(BigTreeInsert, Order)
{
	avl_test <{
		.minCount = 80'000,
		.maxCount = 100'000,
		.test_insert_balance = false,
		.test_insert_order = true,
		.test_remove_balance = false,
		.test_remove_order = false,
		.check_interval = 0,
		.iterations = 1
	}> ();
}

TEST(BigTreeRemove, Balance)
{
	avl_test <{
		.minCount = 80'000,
		.maxCount = 100'000,
		.test_insert_balance = false,
		.test_insert_order = false,
		.test_remove_balance = true,
		.test_remove_order = false,
		.check_interval = 0,
		.iterations = 1
	}> ();
}

TEST(BigTreeRemove, Order)
{
	avl_test <{
		.minCount = 80'000,
		.maxCount = 100'000,
		.test_insert_balance = false,
		.test_insert_order = false,
		.test_remove_balance = false,
		.test_remove_order = true,
		.check_interval = 0,
		.iterations = 1
	}> ();
}
