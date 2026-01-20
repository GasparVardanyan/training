// @leet start
#include <cstddef>
# include <vector>
using namespace std;
class Solution {
public:
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
		std::size_t c = 0;
		auto i1 = nums1.cbegin ();
		auto i2 = nums2.cbegin ();

		double s = 0;
		std::size_t cs = nums1.size () + nums2.size ();
		bool two = cs % 2 == 0;
		if (0 != cs && 0 == cs % 2) cs--;
		std::size_t mi = cs / 2;
		bool _two = two;



		while (true) {
			bool h1 = nums1.cend () != i1;
			bool h2 = nums2.cend () != i2;
			int cv;

			if (h1 && h2) {
				if (* i1 < * i2) {
					cv = * i1;
					i1 ++;
				}
				else {
					cv = * i2;
					i2 ++;
				}
			}
			else if (h1) {
				cv = * i1;
				i1++;
			}
			else if (h2) {
				cv = * i2;
				i2++;
			}


			if (c == mi) {
				s += cv;
				if (true == two) {
					two = false;
					mi++;
				}
				else {
					break;
				}
			}

			c++;
		}

		if (_two) s /= 2;
		return s;
	}
};
// @leet end
