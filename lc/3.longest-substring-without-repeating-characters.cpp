// @leet start
# include <string>
# include <algorithm>
# include <iterator>
using namespace std;
class Solution {
public:
	static int lengthOfLongestSubstring(const std::string & s) {
		std::string s_max = "";
		std::string s_curr = "";

		for (char c : s) {

			auto f = std::find (s_curr.cbegin (), s_curr.cend (), c);

			if (s_curr.cend () != f) {
				if (s_curr.size () > s_max.size ()) {
					s_max = s_curr;
				}
				s_curr = s_curr.substr (1 + std::distance (s_curr.cbegin (), f));
			}

			s_curr += c;
		}


		if (s_curr.size () > s_max.size ()) {
			s_max = s_curr;
		}

		return s_max.size ();
	}
};
// @leet end
