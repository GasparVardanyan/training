// @leet start
#include <string>
#include <vector>
class Solution {
public:
	std::string convert(const std::string & s, int numRows) {
		if (1 == numRows) return s;

		std::vector <std::string> v (numRows);
		bool f = true;
		int ri = 0;
		for (char c : s) {
			if (true == f) {
				v [ri] += c;
				ri++;
				if (ri == numRows) {
					ri--;
					f = false;
				}
			}
			else {
				ri--;
				v [ri] += c;
				if (0 == ri) {
					ri++;
					f = true;
				}
			}
		}

		std::string res;

		for (const std::string & r : v) {
			res += r;
		}

		return res;
	}
};
// @leet end
