# include <iostream>
# include <numeric>

# include "15_max_subseq_sum.h"

int main ()
{
	std::vector <int> v {
		1,
		2,
		3,
	};

	int s1 = maxSubSeqSum1 (v);
	int s2 = maxSubSeqSum2 (v);
	int s3 = maxSubSeqSum3 (v);
	int s4 = maxSubSeqSum4 (v);

	std::cout << "SUM: " << std::accumulate (v.begin (), v.end (), 0) << std::endl;

	std::cout << "s1: " << s1 << std::endl;
	std::cout << "s2: " << s2 << std::endl;
	std::cout << "s3: " << s3 << std::endl;
	std::cout << "s4: " << s4 << std::endl;
}
