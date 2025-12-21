# include <bit>
# include <cstddef>
# include <cstdlib>
# include <ctime>
# include <gtest/gtest.h>
# include "ex_1_5_popcount_with_recursion.h"

TEST (Popcount, RandomizationWithStdPopcount)
{
	std::srand (std::time (NULL));

	for (std::size_t i = 0; i < 100; i++) {
		unsigned u = std::rand () % 9999999;
		std::size_t v1 = popcount (u);
		std::size_t v2 = std::popcount (u);
		EXPECT_EQ (v1, v2);
	}
}
