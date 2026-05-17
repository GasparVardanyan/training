# include <algorithm>
# include <iostream>
# include <iterator>
# include <numeric>
# include <random>

# include "20_vector.h"
# include "26_map.h"

int main () {
	map <int, int> m; {
		vector <int> v (12);
		std::iota (v.begin (), v.end (), 1);
		std::shuffle (v.begin (), v.end (), std::random_device {});
		for (int i : v) {
			m.insert (i, i + 1);
		}
	}

	std::cout << m << std::endl;

	for (auto [k, v] : m) {
		std::cout << "key: " << k << ", value: " << v << std::endl;
	}
}
