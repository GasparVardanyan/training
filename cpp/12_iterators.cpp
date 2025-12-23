# include <iostream>
# include <iterator>
# include <map>
# include <sstream>
# include <string>
# include <vector>

std::string iterType (std::contiguous_iterator_tag) {
	return "contiguous_iterator_tag";
}

std::string iterType (std::random_access_iterator_tag) {
	return "random_access_iterator_tag";
}

std::string iterType (std::bidirectional_iterator_tag) {
	return "bidirectional_iterator_tag";
}

std::string iterType (std::forward_iterator_tag) {
	return "forward_iterator_tag";
}

std::string iterType (std::output_iterator_tag) {
	return "output_iterator_tag";
}

std::string iterType (std::input_iterator_tag) {
	return "input_iterator_tag";
}

template <typename Iter>
std::string iterType (const Iter & it = {}) {
	return iterType (typename std::iterator_traits <Iter>::iterator_category ());
}

void stream_iterators ();

int main ()
{
	std::vector <int> vec { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector <int>::iterator it1 = vec.begin ();
	* it1 = 10;

	std::cout << "iterator type: " << iterType (it1) << std::endl;
	std::cout << "iterator type: " << iterType <std::vector <int>::const_reverse_iterator> () << std::endl;

	static_assert (std::contiguous_iterator <decltype (it1)>);

	std::advance (it1, 3);
	std::cout << * it1 << std::endl;
	it1 = std::next (it1);
	std::cout << * it1 << std::endl;
	it1 = std::prev (it1);
	std::cout << * it1 << std::endl;
	std::advance (it1, -3);
	std::cout << * it1 << std::endl;

	std::cout << vec.size () << " - " << std::distance (vec.begin (), vec.end ()) << std::endl;

	for (auto it = vec.cbegin (); it < vec.cend (); it++) {
		std::cout << * it << ' ';
	}

	std::cout << std::endl;

	for (auto it = vec.cbegin (); it < vec.cend (); std::advance (it, 2)) {
		std::cout << * it << ' ';
	}

	std::cout << std::endl;

	for (auto it = std::prev (vec.cend ()); it >= vec.cbegin (); it = std::prev (it)) {
		std::cout << * it << ' ';
	}

	std::cout << std::endl;

	for (auto it = vec.crbegin (); it < vec.crend (); it = std::next (it)) {
		std::cout << * it << ' ';
	}

	std::cout << std::endl;

	std::map <int, int> myMap = {
		{1, 2},
		{2, 3},
		{3, 4},
		{4, 5},
		{5, 6},
	};

	std::cout << iterType <decltype (myMap)::iterator> () << std::endl;

	for (auto it = myMap.cbegin (); it != myMap.cend (); it++) {
		std::cout << it->first << " - " << it->second << std::endl;
	}

	int arr [10];

	std::cout << std::distance (std::begin (arr), std::end (arr)) << std::endl;

	std::cout << "=============================" << std::endl;

	stream_iterators ();
}

void stream_iterators ()
{
	std::istringstream istr ("1\t 2   5   3  4");
	std::vector <int> v;

	std::copy (std::istream_iterator <int> (istr), std::istream_iterator <int> (), std::back_inserter(v));

	for (int i : v) {
		std::cout << i << std::endl;
	}

	std::copy (v.begin (), v.end (), std::ostream_iterator<int> (std::cout, " -- "));
	std::cout << std::endl;
}
