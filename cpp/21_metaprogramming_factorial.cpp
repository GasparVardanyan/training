# include <iostream>
# include <iterator>
# include <type_traits>

template <unsigned int n>
struct factorial {
	enum {
		value = n * factorial <n-1>::value
	};
};

template <>
struct factorial <0> {
	enum {
		value = 1
	};
};



template <unsigned int n>
struct fib {
	enum {
		value = int (fib <n - 1>::value) + int (fib <n - 2>::value) // VERY INEFFICIENT
	};
};

template <>
struct fib <0> {
	enum {
		value = 0
	};
};

template <>
struct fib <1> {
	enum {
		value = 1
	};
};



template <unsigned long long n>
struct factorial2 : std::integral_constant <unsigned long long, n * factorial2 <n - 1>::value> {};

template <>
struct factorial2 <0ULL> : std::integral_constant <unsigned long long, 1ULL> {};

constexpr unsigned factorial3 (unsigned i) {
	if (0 == i) {
		return 1;
	}
	else {
		return i * factorial3 (i - 1);
	}
}



// template <class T, T N, class I = std::make_integer_sequence <T, N>>
// struct factorial4;
//
// template <class T, T N, T... Is>
// struct factorial4 <T, N, std::index_sequence <T, Is...>> {
// 	static constexpr T value = (static_cast <T> (1) * ... * (Is + 1));
// }


int main () {
	std::cout << factorial <6>::value << std::endl;
	std::cout << factorial2 <6>::value << std::endl;

	const int arr [factorial3 (6)] = {0};
	std::cout << std::size (arr) << std::endl;

	std::cout << fib <0>::value << std::endl;
	std::cout << fib <1>::value << std::endl;
	std::cout << fib <2>::value << std::endl;
	std::cout << fib <3>::value << std::endl;
	std::cout << fib <4>::value << std::endl;
	std::cout << fib <5>::value << std::endl;
}
