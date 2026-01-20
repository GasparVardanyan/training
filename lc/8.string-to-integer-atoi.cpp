// @leet start
#include <iostream>
#include <iterator>
#include <limits>
# include <string>

enum class Phase : int {
	Whitespace,
	Sign,
	Value
};

class Solution {
public:
	int myAtoi(std::string s) {
		unsigned r = 0;
		Phase p = Phase::Whitespace;
		bool positive = true;

		bool terminate = false;

		for (auto it = s.cbegin (); it != s.cend (); std::advance(it, 1)) {
			char c = * it;
			switch (p) {
			case Phase::Whitespace :
				if (' ' == c) {
						continue;
					}
				else {
						p = Phase::Sign;
					}
			case Phase::Sign :
				p = Phase::Value;
				if ('+' == c) {
						continue;
					}
				else if ('-' == c) {
						positive = false;
						continue;
					}
			case Phase::Value :
				if ('0' <= c && c <= '9') {
						int m = c - '0';
						unsigned _r = r * 10 + m;
						if ((_r - m) / 10 != r) {
							if (positive) {
								return std::numeric_limits<int>::max ();
							}
							else {
								return std::numeric_limits<int>::min ();
							}
						}
						else {
							r = _r;
						}
					}
				else {
						terminate = true;
						break;
					}
			}
			if (true == terminate) {
				break;
			}
		}

		if (positive) {
			if (r <= std::numeric_limits<int>::max ()) {
				return r;
			}
			else {
				return std::numeric_limits<int>::max ();
			}
		}
		else {
			std::cout << "NEG: " << r << std::endl;
			std::cout << -r << std::endl;
			if (-(long long) r >= std::numeric_limits<int>::min ()) {
				std::cout << "hello" << std::endl;
				return -r;
			}
			else {
				std::cout << "world" << std::endl;
				return std::numeric_limits<int>::min ();
			}
		}
	}
};
// @leet end
