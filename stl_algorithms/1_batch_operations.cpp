# include <algorithm>
# include <array>
# include <execution>
# include <functional>
# include <iostream>
# include <syncstream>

# include <boost/range/algorithm_ext/iota.hpp>

int main () {
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
	std::array <int, 10> arr; {
		boost::iota (arr, 0);
	}



	std::cout << "for_each:\n\t";
	// NOLINTNEXTLINE(boost-use-ranges)
	std::for_each (arr.cbegin (), arr.cend (), [] (const int i) -> void {
		std::cout << i << ", ";
	});
	std::cout << '\n';



	std::cout << "for_each utilizing return value:\n\t";
	// NOLINTNEXTLINE(boost-use-ranges)
	std::for_each (arr.cbegin (), arr.cend (), [sum = 0] (const int i, bool b = false) mutable -> void {
		if (false == b) {
			sum += i;
			std::cout << i << ", ";
		}
		else {
			std::cout << "\n\tsum: " << sum << '\n';
		}
	}) (0, true);



	// TODO: dive deep into execution policies and forward progress guarantees
	// https://www.youtube.com/watch?v=g9Rgu6YEuqY
	std::cout << "for_each with execution policy:\n\t";
	std::for_each (std::execution::par, arr.cbegin (), arr.cend (), [] (const int i) -> void {
		std::osyncstream (std::cout) << i << ", ";
	});
	std::cout << '\n';



	std::cout << "ragnes::for_each:\n\t";
	std::ranges::for_each (arr, [] (const int i) -> void {
		std::cout << i << ", ";
	});
	std::cout << '\n';



	std::cout << "ranges::for_each with projection: \n\t";
	std::ranges::for_each (arr.cbegin (), arr.cend (), [] (char c) -> void {
		std::cout << c << ", ";
	}, [] (int i) -> char {
		return static_cast <char> ('a' + i);
	});
	std::cout << '\n';



	std::cout << "ranges::for_each with projection utilizing return value: \n\t";
	struct S {
		int i = 0;
		char c = 'a';
		S & operator++ () {
			i++;
			c++;
			return *this;
		}
	};

	std::array <S, 10> s_arr; { // NOLINT (cppcoreguidelines-avoid-magic-numbers)
		boost::iota (s_arr, S {});
	}

	struct F {
		int sum;

		F & operator() (char c) {
			std::cout << c << ", ";
			sum += static_cast <int> (c);
			return *this;
		}
	};

	int sum = std::ranges::for_each (s_arr, F {}, & S::c).fun.sum;

	sum -= s_arr.size () * 'a';

	std::cout << "\n\tsum: " << sum << '\n';

	static_assert ('G' == std::invoke (& S::c, S {.c = 'G'}));



	std::cout << "for_each_n:\n\t";

	std::for_each_n (arr.cbegin (), arr.size (), [] (int i) -> void {
		std::cout << i << ", ";
	});
	std::cout << '\n';



	std::cout << "for_each_n with execution policy:\n\t";
	std::for_each_n (std::execution::seq, arr.cbegin (), arr.size (), [] (int i) -> void {
		std::cout << i << ", ";
	});
	std::cout << '\n';



	std::cout << "ranges::for_each_n:\n\t";
	std::ranges::for_each_n (arr.cbegin (), arr.size (), [sum = 0] (int i, bool b = false) mutable -> void {
		if (false == b) {
			sum += i;
			std::cout << i << ' ';
		}
		else {
			std::cout << "\n\tsum: " << sum << '\n';
		}
	}).fun (0, true);
}
