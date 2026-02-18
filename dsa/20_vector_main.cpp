# include "20_vector.h"
# include "verbose_class.h"

# include <iostream>
# include <iterator>

int main () {
	{
		std::vector <C> std_vec { {}, {}, {}};
		std::cout << "====================" << std::endl;
		vector <C> vec (std_vec.cbegin (), std_vec.cend ());

		std::cout << vec.size () << std::endl;
	}

	std::cout << "====================" << std::endl;

	vector <int> vec {1, 2, 3}; std::cout << vec.size () << " - " << vec.capacity () << std::endl;
	std::cout << "\t[" << vec.back () << ']' << std::endl;
	vec.push_back (4); std::cout << vec.size () << " - " << vec.capacity () << std::endl;
	std::cout << "\t[" << vec.back () << ']' << std::endl;
	vec.push_back (5); std::cout << vec.size () << " - " << vec.capacity () << std::endl;
	std::cout << "\t[" << vec.back () << ']' << std::endl;
	vec.push_back (6); std::cout << vec.size () << " - " << vec.capacity () << std::endl;
	std::cout << "\t[" << vec.back () << ']' << std::endl;
	vec.push_back (7); std::cout << vec.size () << " - " << vec.capacity () << std::endl;
	std::cout << "\t[" << vec.back () << ']' << std::endl;
	vec.push_back (8); std::cout << vec.size () << " - " << vec.capacity () << std::endl;
	std::cout << "\t[" << vec.back () << ']' << std::endl;
	vec.push_back (9); std::cout << vec.size () << " - " << vec.capacity () << std::endl;
	std::cout << "\t[" << vec.back () << ']' << std::endl;

	vector <int> vec2 (vec);

	// vec2.shrink_to_contain (5); std::cout << vec2.size () << " - " << vec2.capacity () << std::endl;
	vec2.reserve (20); std::cout << vec2.size () << " - " << vec2.capacity () << std::endl;
	for (const auto & v : vec2) {
		std::cout << v << ' ';
	}
	std::cout << std::endl;
	vec2.resize (10); std::cout << vec2.size () << " - " << vec2.capacity () << std::endl;

	if (std::is_same_v <std::input_iterator_tag, std::forward_iterator_tag>) {
		std::cout << "fjksdjfklsfjlks" << std::endl;
	}

	// std::vector <int> v (vec2.cbegin (), vec2.cend ());
	std::vector <int> v (vec2.size ());
	std::transform (vec2.cbegin (), vec2.cend (), v.begin (), [] (int v) -> int {
		return v * 10;
	});

	for (int i : v) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
}
