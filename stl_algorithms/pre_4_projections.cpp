# include <algorithm>
# include <array>
# include <boost/range/algorithm_ext/iota.hpp>
# include <iostream>

struct S {
	int x = 0;
	char c = 'a';

	S & operator++ () {
		x++;
		c++;
		return * this;
	}
};

int main () {
	std::array <S, 10> s;
	boost::iota (s, S {});

	std::ranges::for_each (s, [] (int i) -> void {
		std::cout << i << ", ";
	}, [] (const S & s) -> int {
		return s.x;
	});
	std::cout << '\n';

	std::cout << "HELLO\n";
}
