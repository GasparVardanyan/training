# ifndef MAX_SUBSEQ_SUM_H_15
# define MAX_SUBSEQ_SUM_H_15

# include <vector>

int maxSubSeqSum1(const std::vector<int> &v) noexcept(false);

int maxSubSeqSum2(const std::vector<int> &v);

int maxSubSeqSum3(const std::vector<int>::const_iterator &beg,
				  const std::vector<int>::const_iterator &end);

int maxSubSeqSum3(const std::vector<int> &v) noexcept(false);

int maxSubSeqSum4(const std::vector<int> &v);

# endif // MAX_SUBSEQ_SUM_H_15
