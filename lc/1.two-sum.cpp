#include <iostream>
// @leet start
# include <iterator>
# include <vector>
# include <algorithm>
# include <iterator>
using namespace std;

class Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		std::vector <int> sv (nums.size ());
		std::copy (nums.cbegin (), nums.cend (), sv.begin ());

		std::sort (nums.begin (), nums.end ());
		auto beg = nums.cbegin (), end = std::prev (nums.cend ());
		while (beg < end) {
			int s = * beg + * end;
			if (s == target) {
				break;
			}
			else if (s > target) {
				std::advance (end, -1);
			}
			else {
				std::advance (beg, 1);
			}
		}

		return {
			static_cast <int> (std::distance (sv.cbegin (), std::find (sv.cbegin (), sv.cend (), *beg))),
			static_cast <int> (std::distance (sv.cbegin (), std::prev (std::find (sv.crbegin (), sv.crend (), *end).base ())))
		};
	}
};
// @leet end

int main () {
	Solution s;
	auto inp = std::vector <int> {2,7,11,15};
	auto res = s.twoSum (inp, 9);
	for (auto i : res) {
		std::cout << i << std::endl;
	}
}
