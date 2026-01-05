# include "15_max_subseq_sum.h"

# include <iostream>
# include <iterator>
# include <limits>
# include <numeric>
# include <vector>

# ifdef MAX
# error conflicting macro names
# endif // MAX
# define MAX(a,b) ((a) > (b) ? (a) : (b))

void maxSubSeqSum_check_arg (const std::vector <int> & v) noexcept (false) {
	if (0 == v.size ()) {
		throw std::runtime_error ("empty argument to maxSubSeqSum3");
	}
}

int maxSubSeqSum1 (const std::vector <int> & v) noexcept (false) {
	maxSubSeqSum_check_arg (v);

	int s = std::numeric_limits <int>::min ();

	for (auto i1 = v.cbegin (); i1 != v.cend (); std::advance (i1, 1)) {
		for (auto i2 = i1; i2 < v.cend (); std::advance (i2, 1)) {
			int cs = std::accumulate (i1, std::next (i2), 0);

			// int _cs = 0;
			// for (auto i3 = i1; i3 <= i2; std::advance (i3, 1)) {
			// 	_cs += * i3;
			// }
			// if (cs != _cs) {
			// 	throw std::runtime_error ("wrong use of std::accumulate");
			// }

			if (cs > s) {
				s = cs;
			}
		}
	}

	return s;
}

int maxSubSeqSum2 (const std::vector <int> & v) {
	maxSubSeqSum_check_arg (v);

	int s = std::numeric_limits <int>::min ();

	for (auto i1 = v.cbegin (); i1 < v.cend (); std::advance (i1, 1)) {
		int cs = 0;

		for (auto i2 = i1; i2 < v.cend (); std::advance (i2, 1)) {
			cs += * i2;

			if (cs > s) {
				s = cs;
			}
		}
	}

	return s;
}

int maxSubSeqSum3 (const std::vector <int>::const_iterator & beg, const std::vector <int>::const_iterator & end) {
	auto dist = std::distance (beg, end);

	if (dist == 1) {
		return * beg;
	}
	else if (dist == 0) {
		std::cout << "))" << std::endl;
		return 0;
	}
	else {
		const auto mid = beg + dist / 2;

		# define COND ((1 == *beg) && (2 == *mid) && (3 == * std::prev (end)))

		int ls = maxSubSeqSum3 (beg, mid);
		int rs = maxSubSeqSum3 (mid, end);

		int mLMax, mRMax; {
			mLMax = mRMax = std::numeric_limits <int>::min ();
		}

		int mLMaxC = 0, mRMaxC = 0;

		bool mL = false, mR = false;

		// [beg, mid) [mid, end)

		// for (auto it = mid; it != beg;) {
		// 	std::advance (it, -1);
		//
		// 	mLMaxC += * it;
		// 	if (mLMaxC > mLMax) {
		// 		mLMax = mLMaxC;
		// 		mL = true;
		// 	}
		// }

		// as I can't imagine how these bounds behave, I need extra safety...
		{
			auto it = mid;
			if (beg != it) {
				std::advance (it, -1);

				while (true) {
					mLMaxC += * it;

					if (mLMaxC > mLMax) {
						mLMax = mLMaxC;
						mL = true;
					}

					if (beg == it) {
						break;
					}
					else {
						std::advance (it, -1);
					}
				}
			}
		}

		for (auto it = mid; it < end; std::advance (it, 1)) {
			mRMaxC += * it;
			if (mRMaxC > mRMax) {
				mRMax = mRMaxC;
				mR = true;
			}
		}

		int mMax; {
			if (mL && mR) {
				mMax = mLMax + mRMax;
			}
			else if (mL) {
				mMax = mLMax;
			}
			else if (mR) {
				mMax = mRMax;
			}
			else {
				mMax = std::numeric_limits <int>::min ();
			}
		}

		return MAX (mMax, MAX (ls, rs));
	}
}

int maxSubSeqSum3 (const std::vector <int> & v) noexcept (false) {
	maxSubSeqSum_check_arg (v);

	auto it = v.cbegin ();

	std::vector <int> v2 {* it};
	v2.reserve (v.size ());

	bool nonNeg = v2.front () >= 0;

	// NOTE: minimize the recursion depth
	while (++it != v.cend ()) {
		const int val = * it;
		const bool _nonNeg = val >= 0;

		// // if (_nonNeg == nonNeg) {	// NOTE: if the result is nonnegative, then
		// 							// this is ok, otherwise this fails
		// if (true == _nonNeg && true == nonNeg) { // NOTE: at least join the nonnegative values
		// 	v2.back () += val;
		if (_nonNeg == nonNeg) {
			v2.back () = MAX (v2.back () + val, MAX (v2.back (), val));
		}
		else {
			v2.push_back (val);
			nonNeg = _nonNeg;
		}
	}

	return maxSubSeqSum3 (v.cbegin (), v.cend ());
}

int maxSubSeqSum4 (const std::vector <int> & v) {
	maxSubSeqSum_check_arg (v);

	int s = std::numeric_limits <int>::min ();

	int cs = 0;
	for (auto it = v.cbegin (); it != v.cend (); std::advance (it, 1)) {
		cs += * it;

		if (cs > s) {
			s = cs;
		}

		if (0 > cs) {
			cs = 0;
		}
	}

	return s;
}
