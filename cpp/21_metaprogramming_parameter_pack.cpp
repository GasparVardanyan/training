# include <functional>
# include <iostream>
# include <type_traits>



template <std::ostream & os, typename>
static void print1 () {
	os << std::endl;
}

template <std::ostream & os, typename T, T first, T ... rest>
static void print1 () {
	os << first << ", ";
	print1 <os, T, rest ...> ();
}

static void print2 (std::ostream & os) {
	os << std::endl;
}

template <typename TFirst, typename ... TRest>
static void print2 (std::ostream & os, const TFirst & first, const TRest & ... rest) {
	os << first << ", ";
	print2 (os, rest ...);
}

template <std::ostream & os, typename T, T ... args>
static void print3 () {
	(void) (int []) {0, ((os << args << ", "), 0) ...};
	os << std::endl;
}

template <typename ... Args>
static void print4 (std::ostream & os, const Args & ... args) {
	(void) (int []) {0, ((os << args << ", "), 0) ...};
	os << std::endl;
}

template <std::ostream & os, typename T, T ... args>
static void print5 () {
	((os << args << ", "), ...) << std::endl;
}

template <typename ... Args>
static void print6 (std::ostream & os, const Args & ... args) {
	((os << args << ", "), ...) << std::endl;
}

template <std::ostream & os, typename>
static void print7 () {}

template <std::ostream & os, typename T, T first, T ... rest>
static void print7 () {
	os << first << ", ";

	if (0 == sizeof ... (rest)) {
		os << std::endl;
	}
	else {
		print7 <os, T, rest ...> ();
	}
}

static void print8 (std::ostream &) {}

template <typename TFirst, typename ... TRest>
static void print8 (std::ostream & os, const TFirst & first, const TRest & ... rest) {
	os << first << ", ";

	if (0 == sizeof ... (rest)) {
		os << std::endl;
	}
	else {
		print8 (os, rest ...);
	}
}

template <std::ostream & os, typename T, T first, T ... rest>
static void print9 () {
	os << first << ", ";

	if constexpr (0 == sizeof ... (rest)) {
		os << std::endl;
	}
	else {
		print9 <os, T, rest ...> ();
	}
}

template <typename TFirst, typename ... TRest>
static void print10 (std::ostream & os, const TFirst & first, const TRest & ... rest) {
	os << first << ", ";

	if constexpr (0 == sizeof ... (rest)) {
		os << std::endl;
	}
	else {
		print10 (os, rest ...);
	}
}

template <typename ... Args>
requires (std::is_arithmetic_v <Args> && ...)
static void printSum (std::ostream & os, const Args & ... args) {
	os << (args + ... + 0) << std::endl;
}

template <double ... coeffs, typename ... Args>
requires (std::is_arithmetic_v <Args> && ...)
static void printDotProd (std::ostream & os, const Args & ... args) {
	os << ((coeffs * args) + ... + 0) << std::endl;
}


namespace MultiplePackExpansion {
template <typename ... TS>
requires (std::is_integral_v <TS> && ...)
int sum (TS ... vs) {
	return 0 + (vs + ...);
}

template <typename I, typename ... TS>
requires std::invocable <I, TS ...>
decltype (auto) MyInvoke (I && func, TS && ... args)
	noexcept (std::is_nothrow_invocable_v <I, TS ...>)
{
	return std::forward <I> (func) (std::forward <TS> (args) ...);
}

int x = 10;
decltype (auto) test () {
	// int x = 10;
	return (x);
}

decltype (auto) test1 () {
	return test ();
}
}


int main () {
	print1 <std::cout, int, 1, 2, 3, 4, 5> ();
	print2 (std::cout, 1, "hello", 3, "world", 5);

	print3 <std::cout, int, 1, 2, 3, 4, 5> ();
	print4 (std::cout, 1, "hello", 3, "world", 5);

	print5 <std::cout, int, 1, 2, 3, 4, 5> ();
	print6 (std::cout, 1, "hello", 3, "world", 5);

	print7 <std::cout, int, 1, 2, 3, 4, 5> ();
	print8 (std::cout, 1, "hello", 3, "world", 5);

	print9 <std::cout, int, 1, 2, 3, 4, 5> ();
	print10 (std::cout, 1, "hello", 3, "world", 5);

	printSum (std::cout, 1, 2, 3, 4, 5);
	printSum (std::cout);

	printDotProd <1.1, 1.2, 1.3> (std::cout, 10, 20, 30);
	printDotProd <> (std::cout);

	{
		std::cout << "====================" << std::endl;

		using namespace MultiplePackExpansion;
		std::cout << std::invoke (sum <int, int, int>, 2, 4, 6) << std::endl;
		std::cout << MyInvoke (sum <int, int, int>, 2, 4, 6) << std::endl;

		// std::cout << std::invoke (dotProd <1, 10, 100, int, int, int>, 2, 4, 6) << std::endl;

		std::cout << "====================" << std::endl;

		std::cout << test () << std::endl;
		test1 () = 20;
		std::cout << test () << std::endl;
	}
}
