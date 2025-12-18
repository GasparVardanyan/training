# pragma once

# include <cstdlib>
# include <ctime>
# include <stdexcept>
# include <vector>

template <typename T, typename Comparator>
T & findNthMax (std::vector<T> & items, std::size_t n, Comparator isLessThan) noexcept (false)
{
	if (items.size () < n) {
		throw std::invalid_argument ("items should contain at least n values");
	}

	std::vector <std::size_t> max_k_i {0};

	bool clean = false;

	for (std::size_t i = 1; i < items.size (); i++) {
		decltype (max_k_i.rbegin ()) place_it = max_k_i.rend ();
		for (auto rit = max_k_i.rbegin (); rit != max_k_i.rend (); rit++) {
			if (isLessThan (items [* rit], items [i])) {
				place_it = rit;
			}
		}

		if (max_k_i.rend () != place_it) {
			max_k_i.insert (place_it.base () - 1, i);
			if (false == clean) {
				if (max_k_i.size () == n) {
					clean = true;
				}
			}
			else {
				max_k_i.erase (max_k_i.end () - 1);
			}
		}
		else {
			if (false == clean) {
				max_k_i.push_back (i);
			}
		}
	}

	return items [max_k_i.at (n - 1)];
}
