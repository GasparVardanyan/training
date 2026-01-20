// @leet start
#include <iostream>
#include <iterator>
#include <stack>
#include <vector>
class Solution {
public:
	bool isMatch(std::string s, std::string pattern) {
		if (s == "") {
			if (pattern == "") {
				return true;
			}
			else {
				return false;
			}
		}
		else if (pattern == "") {
			return false;
		}

		auto sIt = s.cbegin ();
		char prev;

		bool matchedAny = false;
		char matchedAnyChar;
		bool matchedAnyCanRecover = false;
		std::vector <char> matchedAnyChars;

		for (auto pit = pattern.cbegin (); pit != pattern.cend (); std::advance(pit, 1)) {
			bool ma = false;
			bool macr = false;
			char p = * pit;
			if (p == '.') {
				std::advance (sIt, 1);
				if (s.cend () == sIt) {
					std::cout << "WWW" << std::endl;
					if (pattern.cend () != std::next (pit)) {
						return false;
					}
					else {
						return true;
					}
				}
			}
			else {
				if ('*' == p) {
					ma = true;
					if ('.' == prev) {
						if (s.cend () != sIt) {
							matchedAnyChar = * sIt;
							matchedAnyChars.push_back(matchedAnyChar);
						}
						auto pN = std::next (pit);
						if (pN == pattern.cend ()) {
							return true;
						}
						else {
							while (true) {
								if (s.cend () == sIt) {
									return false;
								}
								else {
									sIt = std::next (sIt);
									if (* pN == * sIt) {
										break;
									}
								}
							}
						}
					}
					else {
						bool found = false;
						matchedAnyChar = prev;
						matchedAnyChars.push_back (matchedAnyChar);
						while (true) {
							if (prev == * sIt) {
								sIt = std::next (sIt);
								found = true;
								if (s.cend () == sIt) {
									break;
								}
							}
							else {
								break;
							}
						}
						if (false == found && matchedAnyCanRecover) {
							std::cout << "MAXR: " << matchedAnyChar << std::endl;
							matchedAny = true;
							matchedAnyChars.pop_back();
							matchedAnyChar = matchedAnyChars.back();
						}
					}
				}
				else {
					if (true == matchedAny) {
						macr = true;

						while (true) {
							if (* pit == matchedAnyChar) {
								pit = std::next (pit);
								if (pattern.cend () == pit) {
									std::cout << "WWW" << std::endl;
									return s.cend () == sIt;
								}
							}
							else {
								break;
							}
						}
					}
					if (p == * sIt) {
						std::advance (sIt, 1);
						if (s.cend () == sIt) {
							std::cout << "XXX" << std::endl;
							if (pattern.cend() == std::next (pit)) {
								std::cout << "a1" << std::endl;
								return true;
							}
							else {
								while (true) {
									std::advance (pit, 1);
									if (pattern.cend () != pit) {
										std::advance (pit, 1);
											if (pattern.cend () != pit && '*' == * pit) {
												continue;
											}
											else {
								std::cout << "a2" << std::endl;
												return true;
											}
									}
									else {
								std::cout << "a3" << std::endl;
										return true;
									}
								}
								if (pattern.cend () == pit) {
								std::cout << "a4" << std::endl;
									return true;
								}
								else {
									return false;
								}
							}
						}
					}
					else {
						auto pN = std::next (pit);
						std::cout << "finishing " << * pit << std::endl;
						if (pattern.cend () != pN && '*' == * pN) {} else {
							return false;
						}
					}
				}
			}
			matchedAny = ma;
			matchedAnyCanRecover = macr;
			prev = p;
		}

		if (s.cend () == sIt) {
			std::cout << "ZZZ" << std::endl;
			return true;
		}
		else {
			return false;
		}
	}
};
// @leet end
