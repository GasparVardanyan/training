# include <iostream>
# include <iterator>
# include <list>
# include <string>
# include <vector>



// ../dsa/19_iterType.h:

inline std::string iterType (std::contiguous_iterator_tag) {
	return "contiguous_iterator_tag";
}

inline std::string iterType (std::random_access_iterator_tag) {
	return "random_access_iterator_tag";
}

inline std::string iterType (std::bidirectional_iterator_tag) {
	return "bidirectional_iterator_tag";
}

inline std::string iterType (std::forward_iterator_tag) {
	return "forward_iterator_tag";
}

inline std::string iterType (std::output_iterator_tag) {
	return "output_iterator_tag";
}

inline std::string iterType (std::input_iterator_tag) {
	return "input_iterator_tag";
}

template <typename Iter>
std::string iterType (const Iter & = {}) {
	return iterType (typename std::iterator_traits <Iter>::iterator_category ());
}



int main () {
	std::cout << iterType (std::vector <int>::iterator {}) << std::endl;
	std::cout << iterType (std::list <int>::iterator {}) << std::endl;
}
