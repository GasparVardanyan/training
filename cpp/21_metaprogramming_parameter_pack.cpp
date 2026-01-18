# include <algorithm>
# include <iostream>
# include <iterator>
# include <sstream>



template <typename>
static void print1 () {
	std::cout << std::endl;
}

template <typename T, T first, T ... rest>
static void print1 () {
	std::cout << first << ", ";
	print1 <T, rest ...> ();
}

static void print2 () {
	std::cout << std::endl;
}

template <typename T, typename ... TS>
static void print2 (T first, TS ... rest) {
	std::cout << first << ", ";
	print2 (rest ...);
}

template <typename T, T ... VS>
static void print3 () {
	(void) (int []) {0, ((void) (std::cout << VS << ", "), 0) ...};
	std::cout << std::endl;
}

template <typename ... TS>
static void print4 (TS ... vs) {
	(void) (int []) {0, ((void) (std::cout << vs << ", "), 0) ...};
	std::cout << std::endl;
}

template <typename T, T ... VS>
static void print5 () {
	((std::cout << VS << ", "), ...) << std::endl;
}

template <typename ... TS>
static void print6 (TS ... vs) {
	((std::cout << vs << ", "), ...) << std::endl;
}

template <typename>
static void print7 () {}

template <typename T, T first, T ... rest>
static void print7 () {
	std::cout << first << ", ";

	if (0 < sizeof ... (rest)) {
		print7 <T, rest ...> ();
	}
	else {
		std::cout << std::endl;
	}
}

static void print8 () {}

template <typename T, typename ... TS>
static void print8 (T first, TS ... rest) {
	std::cout << first << ", ";

	if (0 < sizeof ... (rest)) {
		print8 (rest ...);
	}
	else {
		std::cout << std::endl;
	}
}

template <typename T, T first, T ... rest>
static void print9 () {
	std::cout << first << ", ";

	if constexpr (0 < sizeof ... (rest)) {
		print9 <T, rest ...> ();
	}
	else {
		std::cout << std::endl;
	}
}

template <typename T, typename ... TS>
static void print10 (T first, TS ... rest) {
	std::cout << first << ", ";

	if constexpr (0 < sizeof ... (rest)) {
		print10 (rest ...);
	}
	else {
		std::cout << std::endl;
	}
}

int main () {
	print1 <int, 1, 2, 3, 4> ();
	print2 (1, 2, 3, 4, "hello");
	print3 <int, 1, 2, 3, 4> ();
	print4 (1, 2, 3, 4, "hello");
	print5 <int, 1, 2, 3, 4> ();
	print6 (1, 2, 3, 4, "hello");
	print7 <int, 1, 2, 3, 4> ();
	print8 (1, 2, 3, 4, "hello");
	print9 <int, 1, 2, 3, 4> ();
	print10 (1, 2, 3, 4, "hello");
}
