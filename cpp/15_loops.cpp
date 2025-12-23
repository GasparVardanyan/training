# include <algorithm>
# include <iostream>
# include <iterator>
# include <vector>

template <class Iterator, class Sentinel=Iterator>
struct range_t {
	Iterator b;
	Sentinel e;

	Iterator begin () const { return b; }
	Sentinel end () const { return e; }

	bool empty () const { return b == e; }

	range_t without_front (std::size_t count = 1) const {
		if (std::is_same <std::random_access_iterator_tag, typename std::iterator_traits <Iterator>::iterator_category> ()) {
			count = std::min ((std::size_t) std::distance (b, e), count);
		}
		return {std::next (b, count), e};
	}

	range_t without_back (std::size_t count = 1) const {
		if (std::is_same <std::random_access_iterator_tag, typename std::iterator_traits <Iterator>::iterator_category> ()) {
			count = std::min ((std::size_t) std::distance (b, e), count);
		}
		return {b, std::prev (e, count)};
	}
};

template <class Iterator, class Sentinel = Iterator>
range_t <Iterator, Sentinel> range (Iterator b, Sentinel e) {
	return { .b = b, .e = e };
}

template <class Iterable>
auto range (const Iterable & it) {
	return range (std::begin (it), std::end (it));
}

int main ()
{
	std::vector <int> intVec { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	auto r = range (intVec).without_front (2).without_back (3);

	for (int i : r) {
		std::cout << i << std::endl;
	}
}
