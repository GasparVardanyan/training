# include "21_list.h"

# include <iostream>
# include <vector>

int main () {
	std::vector<int> vec {1, 2, 3};
	// vec.insert (vec.cend (), 10);
	list <int> lst (vec.begin (), vec.end ());
	list <int> lst2 (lst);
	// std::copy (vec.begin (), vec.end (), std::back_inserter (lst));
	// std::vector<int> v {1, 2, 3};
	// v.insert (v.cbegin (), 3);
	for (int i : lst) {
		std::cout << i << std::endl;
	}

	for (list<int>::const_iterator it = lst.begin (); it != lst.end (); it++) {
		// * it *= 10;
		std::cout << * it << std::endl;
	}

	return 0;

	auto it1 = lst.begin ();
	auto it2 = lst.end ();
	if (it1 != it2) {
		std::cout << * it1 << std::endl;
	}
	// auto it2 = lst.end ();
	// decltype (lst)::iterator it2 = lst.cbegin ();
	// std::cout << * it2 << std::endl;


	std::cout << lst.size () << std::endl;
	// lst.insert (lst.cbegin (), 10);
	// std::cout << lst.size () << std::endl;
	// lst.erase (lst.cbegin ());
	//
	// {
	// 	auto it1 = lst.cbegin (); it1++;
	// 	auto it2 = lst.cend (); it2--;
	//
	// 	lst.erase (it1);
	// }
	//
	// lst.erase (++lst.begin (), --lst.end ());
	// std::cout << lst.size () << std::endl;

	for (auto it = lst.begin (); it != lst.end (); ++it) {
		// * it *= 10;
		std::cout << * it << std::endl;
	}
}
