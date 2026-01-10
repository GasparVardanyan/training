# include <algorithm>
# include <iostream>
# include <iterator>
# include <sstream>



void print_all (std::ostream & os) {
	os << std::endl;
}

template <typename T, typename... Ts>
void print_all (std::ostream & os, const T & first, const Ts & ... rest) {
	os << first << ' ';
	print_all (os, rest ...);
}



template <typename... Ts>
void print_all2 (std::ostream & os, const Ts & ... args) {
	(void) (int []) { 0, ((void) (os << args << ", "), 0) ...};
	os << std::endl;
}



template <typename... Ts>
void print_all3 (std::ostream & os, const Ts & ... args) {
	((os << args << ", "), ...) << std::endl;
}



template <typename T, typename... Ts>
void print_all4 (std::ostream & os, const T & first, const Ts & ... rest) {
	os << first << ", ";

	if constexpr (sizeof ... (rest) > 0) {
		print_all4 (os, rest ...);
	}
	else {
		os << std::endl;
	}
}



template <typename T, T... Ns>
void print_all5_single_type (std::ostream & os) {
	((os << Ns << ", "), ...) << std::endl;
}

int main () {
	std::stringstream ss;

	print_all (ss, 1, 2, 3, "4 5 6", 7, 8, 9);

	std::copy (
		std::istream_iterator <int> (ss),
		std::istream_iterator <int> (),
		std::ostream_iterator <int> (std::cout, ", ")
	);
	std::cout << std::endl;

	print_all2 (std::cout, 1, "2", 3, 4, 5);
	print_all3 (std::cout, 1, "2", 3, 4, 5);
	print_all4 (std::cout, 1, "2", 3, 4, 5);
	print_all5_single_type <int, 1, 2, 3, 4, 5> (std::cout);

	// print_all5_single_type <int, 1, "2", 3, 4, 5> (std::cout); // error
}
