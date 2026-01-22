// @leet start
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <stack>
#include <utility>
#include <vector>
class Solution {
public:
	bool isMatch1(std::string s, std::string pattern) {
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

	bool isMatch(const std::string & s, const std::string & pattern) {
		enum class token_type : int {
			regular, dot, asterisk, none
		};

		std::map <token_type, std::string> tnames = {
			{ token_type::regular, "regular" },
			{ token_type::dot, "dot" },
			{ token_type::asterisk, "asterisk" },
			{ token_type::none, "none" }
		};

		struct token {
			token_type type;
			char value;

			token (token_type type, char value) : type (type), value (value) {}
		};

		std::queue <token> tokens; {
			char curr_c = 0;
			token_type curr_t;

			auto get_token_type = [] (char c) -> token_type {
				if ('.' == c) {
					return token_type::dot;
				}
				else if ('*' == c) {
					return token_type::asterisk;
				}
				else {
					return token_type::regular;
				}
			};

			for (char c : pattern) {
				token_type t = get_token_type (c);

				if (token_type::asterisk != t) {
					if (0 != curr_c) {
						tokens.push (token (curr_t, curr_c));
					}
					curr_c = c;
					curr_t = t;
				}
				else {
					bool push = true;
					if (false == tokens.empty ()) {
						auto b = tokens.back ();
						if (b.type == token_type::asterisk && b.value == curr_c) {
							push = false;
						}
					}
					if (true == push) {
						tokens.push (token (token_type::asterisk, curr_c));
					}
					curr_c = 0;
				}
			}

			if (0 != curr_c) {
				tokens.push (token (curr_t, curr_c));
			}
		}

		std::queue <std::vector <std::pair <int, int>>> cmatches;
		auto add_cmatch = [&cmatches] (const std::vector <std::pair <int, int>> & ranges) {
			cmatches.push ({});

			for (const auto & range : ranges) {
				for (int s = range.first; s <= range.second; s++) {
					cmatches.back ().push_back ({std::pair <int, int> (range.first, s)});
				}
			}
		};

		int sBegin = 0; // curr chunk begin
		int sMin = 0; // next match begin
		int sMax = 1; // next match end
		// checking [sMin, sMax)

		auto process_cmatches = [&] () -> bool {
			// cmatches must be a "division" of [sMin, sMax)

			std::stack <std::vector <int>> lastPos;
			std::stack <decltype (cmatches)::value_type> s;

			lastPos.push ({sBegin});

			std::vector <int> lll {sBegin};

			while (false == cmatches.empty ()) {
				auto c = cmatches.front ();
				cmatches.pop ();

				std::cout << "\t";
				for (const auto & p : c) {
					std::cout << "[" << p.first << ", " << p.second << "], ";
				}
				std::cout << std::endl;

				// std::vector <int> lps = lastPos.top ();
				// lastPos.pop ();

				std::vector <int> nps;

				for (int lp : lll) {
					for (std::pair <int, int> np : c) {
						if (np.first == lp) {
							nps.push_back (np.second + 1);
						}
					}
				}

				std::cout << "lll: ";
				for (int i : lll) {
					std::cout << i << ", ";
				}
				std::cout << std::endl;

				std::cout << "nps: ";
				for (int i : nps) {
					std::cout << i << ", ";
				}
				std::cout << std::endl;

				lll = nps;
			}

			sBegin = sMax - 1;

			if (lll.cend () != std::find (lll.cbegin (), lll.cend (), sMax - 1)) {
				std::cout << "FOUND" << std::endl;
				return true;
			}
			else {
				std::cout << "NOT FOUND" << std::endl;
				return false;
			}

			return true;
		};

		// bool last_asterisk = false;
		// char last_asterisk_c;


		while (false == tokens.empty ()) {
			auto t = tokens.front ();
			tokens.pop ();

			auto tt = t.type;
			auto tc = t.value;

			std::cout << "processing token: " << tc << " - " << tnames.at (tt) << std::endl;
			std::cout << "s: " << std::string (s.cbegin () + sMin, s.cbegin () + sMax) << ", [" << sMin << ", " << sMax << ")" << std::endl;

			std::vector <std::pair <int, int>> pp;

			int m;

			if (token_type::asterisk != tt) {
				if (sMin >= s.size ()) {
					std::cout << "F" << std::endl;
					return false;
				}
			}

			for (m = sMin; m < sMax; m++) {
				if (token_type::asterisk == tt) {
					// TODO: handle .*
					int n = m;
					while (s [n] == tc) {
						n++;
						if (n == s.size ()) {
							break;
						}
					}
					// n - one after last match

					bool matchExists = n > m;

					n--; // last pos

					if (true == matchExists) {
						pp.push_back ({m, n});
						m = n;
					}
				}
				else {
					if (token_type::dot == tt) {
						std::cout << "\t\t\t>>> dot" << std::endl;
						pp.push_back ({m, m});
					}
					else if (token_type::regular == tt) {
						if (s [m] == tc) {
							std::cout << "\t\t\t>>> " << tc << std::endl;
							pp.push_back ({m, m});
						}
						else {
							return false;
						}
					}
				}
			}
			if (true == pp.empty ()) {
				if (token_type::asterisk != tt) {
					std::cout << "F1" << std::endl;
					return false;
				}
			}
			else {
				add_cmatch (pp);
				sMax = m + 1;
					sMin = m;
					if (token_type::asterisk != tt) {
						if (false == process_cmatches ()) {
							return false;
						}
					}
			}
		}

		if (false == cmatches.empty ()) {
			std::cout << "LP" << std::endl;
			if (false == process_cmatches ()) {
				return false;
			}
		}

		if (s.size () != sBegin) {
			return false;
		}
		else {
			return true;
		}
	}
};
// @leet end

int main () {
	Solution s;
	// std::cout << s.isMatch("aaabcccxt", "a*b*e*c*xt") << std::endl;
	std::cout << s.isMatch("aaaYYYaaaxdfffgxxxxx", "a*Y*a*xdf*gx*") << std::endl;
}
