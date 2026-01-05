# include "15_max_subseq_sum.h"

# include <cstdlib>
# include <ctime>
# include <gtest/gtest.h>

TEST (MaxSubSeqSum, RandomizedComparisonWithO3) {
	std::srand (std::time (NULL));

	for (int iter = 0; iter < 100; iter++) {
		int n = std::rand () % 500 + 1;

		std::vector <int> v (n);

		for (int & item : v) {
			item = std::rand () % 10000;
			if (0 == std::rand () % 2) {
				item *= -1;
			}
		}

		int s1 = maxSubSeqSum1 (v);
		int s2 = maxSubSeqSum2 (v);
		int s3 = maxSubSeqSum3 (v);
		int s4 = maxSubSeqSum4 (v);

		ASSERT_EQ (s1, s2);
		ASSERT_EQ (s1, s3);
		ASSERT_EQ (s1, s4);
	}
}
