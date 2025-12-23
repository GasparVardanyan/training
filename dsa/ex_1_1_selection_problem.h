# pragma once

# include <cstdlib>
# include <ctime>
# include <stdexcept>
# include <vector>

template <typename T, typename Comparator>
T & findNthMax (std::vector<T> & items, std::size_t n, Comparator isLessThan) noexcept (false)
{
	if (0 == items.size () || items.size () < n) {
		throw std::invalid_argument ("items should contain at least n values");
	}
	std::vector <std::size_t> sInd {0};

	bool clean = false;

	for (std::size_t i = 1; i < items.size (); i++) {
		decltype (sInd)::const_reverse_iterator pIt = sInd.crend ();

		for (auto rIt = sInd.crbegin (); rIt != sInd.crend (); rIt = std::next (rIt)) {
			if (isLessThan (items [* rIt], items [i])) {
				pIt = rIt;
			}
		}

		if (sInd.crend () != pIt) {
			sInd.insert (std::next (pIt).base (), i);

			if (false == clean) {
				if (sInd.size () == n) {
					clean = true;
				}
			}
			else {
				sInd.erase (std::prev (sInd.cend ()));
			}
		}
		else {
			if (false == clean) {
				sInd.push_back (i);
			}
		}
	}

	return items [sInd.at (n - 1)];
}
