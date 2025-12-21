# include <gtest/gtest.h>

# include <algorithm>
# include <cstdlib>
# include <ctime>
# include <functional>
# include <iostream>
# include <vector>

# include "ex_1_1_selection_problem.h"

struct IntComparator {
	bool operator() (int a, int b) {
		return a < b;
	}
};

TEST(FindNthMax, RandomizedComparisonWithSort)
{
	std::srand (std::time (NULL));

	for (int iter = 0; iter < 100; iter++) {
		int n = std::rand () % 500;
		int k = 2 * n;

		std::vector <int> myVec (k);

		for (int & item : myVec) {
			item = std::rand () % 1000;
		}

		int v1 = findNthMax (myVec, n, IntComparator {});

		std::sort (myVec.begin (), myVec.end (), std::less <int> ());

		int v2 = myVec [n];

		EXPECT_EQ (v1, v2);
	}
}
