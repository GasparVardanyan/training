// @leet start
#include <iostream>
#include <limits>
class Solution {
public:
	int reverse(int x) {
		const bool pos = x >= 0;
		unsigned xx;
		if (false == pos) {
			xx = -(unsigned)x;
		}
		else {
			xx = x;
		}
		unsigned r = 0;
		while (xx) {
			unsigned rem = xx % 10;
			unsigned _r = r * 10 + rem;
			if ((_r - rem) / 10 != r) {
				return 0;
			}
			xx /= 10;
			r = _r;
		}
		if (true == pos) {
			if (r <= std::numeric_limits<int>::max ()) {
				std::cout << r << '\n' << std::numeric_limits <int>::max () << std::endl;
				return r;
			}
			else {
				return 0;
			}
		}
		else {
			if (-r >= std::numeric_limits <int>::min ()) {
				return -r;
			}
			else {
				return 0;
			}
		}
	}
};
// @leet end
