// @leet start
#include <vector>
class Solution {
public:
	bool isPalindrome(int x) {
		std::vector<int> d;
		if (x == 0) {
			return true;
		}
		if (x < 0 || 0 == x % 10) {
			return false;
		}
		else {
			while (x) {
				d.push_back(x % 10);
				x /= 10;
			}
		}

		std::vector <int> dr (d.size ());

		std::copy (d.crbegin (), d.crend (), dr.begin ());

		if (dr == d) {
			return true;
		}
		else {
			return false;
		}
	}
};
// @leet end
