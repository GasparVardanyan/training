# include <iostream>
# include <iterator>
# include <list>
# include <vector>

# include "19_iterType.h"

static void iterators ();

int main () {
	std::vector <int> v { 1, 2, 3};
	std::list <int> l { 1, 2, 3};

	std::cout << "iterators: " <<
		iterType (v.begin ()) << // random_access (and not contiguous)
		" - " <<
		iterType (l.begin ()) << // bidirectional
		std::endl
	;

	std::cout << std::boolalpha;

	std::cout << "empty: " << v.empty () << ", " << l.empty () << std::endl;
	std::cout << "size: " << v.size () << ", " << l.size () << std::endl;

	std::cout << "========== push/pop back ==========" << std::endl;
	v.push_back (4);
	l.push_back (4);

	std::cout << v.back () << " - " << l.back () << std::endl;

	v.pop_back ();
	l.pop_back ();

	std::cout << v.back () << " - " << l.back () << std::endl;

	std::cout << "========== push/pop front ==========" << std::endl;
	v.front () = l.front () = 10;

	std::cout << v.front () << " - " << l.front () << std::endl;

	// v.push_front (10); v.pop_front (); // NOTE: not available for vector
	v.insert (v.cbegin (), 5);
	l.push_front (5);
	std::cout << v.front () << " - " << l.front () << std::endl;
	v.erase (v.cbegin ());
	l.pop_front ();
	std::cout << v.front () << " - " << l.front () << std::endl;

	std::cout << "========== vector-specific ==========" << std::endl;

	std::cout << "operator[] doesn't do bounds-checking: " << v [1] << std::endl;;
	std::cout << "at() does bounds-checking: " << v.at (1) << std::endl;
	std::cout << "capacity(): " << v.capacity () << std::endl;
	v.reserve (10);
	std::cout << "\tafter reserve(): " << v.capacity () << std::endl;


	std::cout << "========== clear/empty ==========" << std::endl;
	v.clear ();
	l.clear ();

	std::cout << "empty: " << v.empty () << ", " << l.empty () << std::endl;

	iterators ();
}

template <typename Container>
void print (const Container & cnt) {
	// Container::const_iterator itr = std::begin (cnt); // TODO: learn about this "dependent-type" stuff later
	typename Container::const_iterator itr = std::begin (cnt); // NOTE: use "auto" instead for portability

	while (itr != std::end (cnt)) {
		std::cout << * itr << ' ';
		std::advance (itr, 1);
	}
	std::cout << std::endl;
}

void iterators () {
	std::cout << "\n===============================" << std::endl;
	std::cout << "========== ITERATORS ==========" << std::endl;
	std::cout << "===============================\n" << std::endl;

	std::vector <int> vec {1, 2, 3};

	for (std::size_t i = 0; i < vec.size (); i++) {
		std::cout << vec [i] << ' ';
	}
	std::cout << std::endl;

	for (decltype (vec)::const_iterator it = vec.cbegin (); it != vec.cend (); it++) {
		std::cout << * it << ' ';
	}
	std::cout << std::endl;

	for (const auto & val : vec) {
		std::cout << val << ' ';
	}
	std::cout << std::endl;

	{
		auto it = vec.cbegin ();

		while (vec.end () != it) { // compare const_iterator with iterator
			std::cout << * it ++ << ' ';
		}
		std::cout << std::endl;
	}

	vec.insert (vec.erase (std::next (vec.cbegin ())), 5);
	vec.insert (vec.begin (), {10, 20, 30});

	for (const auto & val : vec) {
		std::cout << val << ' ';
	}
	std::cout << std::endl;

	vec.erase (std::next (vec.cbegin ()), std::prev (vec.cend ()));

	for (const auto & val : vec) {
		std::cout << val << ' ';
	}
	std::cout << std::endl;

	std::list <int> lst { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, };

	{
		std::list <int>::const_iterator it = lst.cbegin ();

		while (it != lst.cend ()) {
			it = lst.erase (it);
			if (lst.cend () == it) {
				break; // avoid advancing "cend"
			}
			std::advance (it, 1);
		}
	}

	std::cout << "**********" << std::endl;
	print (lst);
	// for (const auto & val : lst) {
	// 	std::cout << val << ' ';
	// }
	// std::cout << std::endl;
}
