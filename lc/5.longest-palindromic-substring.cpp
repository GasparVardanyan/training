// @leet start
#include <iterator>
#include <string>
class Solution {
public:
	std::string longestPalindrome(const std::string & s) {
		std::string r;

		// tattarrattat

		for (std::size_t i = 0; i < s.size (); i++) {
			std::string c = std::string{} + s [i];
		bool ca = true;



			int p = i - 1, n = i + 1;

			while (true) {
				bool back = p >= 0;
				bool front = n < s.size ();
				if (back && front && s [p] == s [n]) {
					if (ca == true)
						if (s [p] != c [0]) ca = false;

					c = s [p] + c + s [n];
					p--;
					n++;
				}
				else if (true == ca) {
					if (back && s [p] == s [i]) {
						c = s [p] + c;
						p--;
					}
					else if (front && s [n] == s [i]) {
						c = c + s [n];
						n++;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}

			if (c.size () > r.size ()) {
				r = c;
			}
		}

		return r;
	}
};
// @leet end
