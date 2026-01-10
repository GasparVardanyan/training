# include <iostream>
# include <ostream>
# include <type_traits>



template <std::size_t N>
struct factorial {
	enum {
		value = N * factorial <N -1>::value
	};
};

template <>
struct factorial <0> {
	enum {
		value = 1
	};
};



template <long long N>
struct factorial2 : std::integral_constant <long long, N * factorial2 <N - 1>::value> {};

template <>
struct factorial2 <0> : std::integral_constant <long long, 1> {};



template <typename T>
concept MyIntegerType = std::is_integral_v <T>;

template <MyIntegerType T, T N>
struct MyIntegralConstant {
	static constexpr T value = N;
};

template <long long N>
struct factorial3 : MyIntegralConstant <long long, N * factorial3 <N - 1>::value> {};

template <>
struct factorial3 <0> : MyIntegralConstant <long long, 1> {};



constexpr long long factorial4 (long long n) {
	if (0 == n) {
		return 1;
	}
	else {
		return n * factorial4 (n - 1);
	}
}



template <typename T, T N, typename I = std::make_integer_sequence <T, N>>
struct factorial5;

template <typename T, T N, T ... Is>
struct factorial5 <T, N, std::integer_sequence <T, Is ...>> {
	static constexpr T value = ((Is + 1) * ... * (T) 1);
};



template <MyIntegerType T, T... Is>
struct MyIntegerSequence {};

template <std::size_t N>
using MyIndex = MyIntegralConstant <std::size_t, N>;

template <MyIntegerType T, typename Index, T... Is>
struct MakeMyIntegerSequenceImpl;

template <MyIntegerType T, std::size_t N, T... Is>
struct MakeMyIntegerSequenceImpl <T, MyIndex <N>, Is...>
	: MakeMyIntegerSequenceImpl <T, MyIndex <N - 1>, (T) (N - 1), Is...>
{};

template <MyIntegerType T, T... Is>
struct MakeMyIntegerSequenceImpl <T, MyIndex <0>, Is...> {
	using type = MyIntegerSequence <T, Is...>;
};

template <MyIntegerType T, T N>
using MakeMyIntegerSequence = MakeMyIntegerSequenceImpl <T, MyIndex <N>>::type;

template <typename T, T N, typename I = MakeMyIntegerSequence <T, N>>
struct factorial6;

template <typename T, T N, T ... Is>
struct factorial6 <T, N, MyIntegerSequence <T, Is ...>> {
	static constexpr T value = ((Is + 1) * ... * (T) 1);
};



template <unsigned int n>
struct fib {
	enum {
		value = int (fib <n - 1>::value) + int (fib <n - 2>::value)
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



template <std::size_t N>
struct fib2 {
	using prev = fib2 <N - 1>;
	static constexpr std::size_t value = prev::value + prev::prev_value;
	static constexpr std::size_t prev_value = prev::value;
};

template <>
struct fib2 <0> {
	static constexpr std::size_t value = 0;
	static constexpr std::size_t prev_value = 1;
};



int main () {
	std::cout << factorial <0>::value << std::endl;
	std::cout << factorial <1>::value << std::endl;
	std::cout << factorial <2>::value << std::endl;
	std::cout << factorial <3>::value << std::endl;
	std::cout << factorial <4>::value << std::endl;
	std::cout << factorial <5>::value << std::endl;
	std::cout << factorial <6>::value << std::endl;
	std::cout << factorial <7>::value << std::endl;
	std::cout << factorial <8>::value << std::endl;
	std::cout << factorial <9>::value << std::endl;
	std::cout << factorial2 <0>::value << std::endl;
	std::cout << factorial2 <1>::value << std::endl;
	std::cout << factorial2 <2>::value << std::endl;
	std::cout << factorial2 <3>::value << std::endl;
	std::cout << factorial2 <4>::value << std::endl;
	std::cout << factorial2 <5>::value << std::endl;
	std::cout << factorial2 <6>::value << std::endl;
	std::cout << factorial2 <7>::value << std::endl;
	std::cout << factorial2 <8>::value << std::endl;
	std::cout << factorial2 <9>::value << std::endl;
	std::cout << factorial3 <0>::value << std::endl;
	std::cout << factorial3 <1>::value << std::endl;
	std::cout << factorial3 <2>::value << std::endl;
	std::cout << factorial3 <3>::value << std::endl;
	std::cout << factorial3 <4>::value << std::endl;
	std::cout << factorial3 <5>::value << std::endl;
	std::cout << factorial3 <6>::value << std::endl;
	std::cout << factorial3 <7>::value << std::endl;
	std::cout << factorial3 <8>::value << std::endl;
	std::cout << factorial3 <9>::value << std::endl;
	std::cout << factorial4 (0) << std::endl;
	std::cout << factorial4 (1) << std::endl;
	std::cout << factorial4 (2) << std::endl;
	std::cout << factorial4 (3) << std::endl;
	std::cout << factorial4 (4) << std::endl;
	std::cout << factorial4 (5) << std::endl;
	std::cout << factorial4 (6) << std::endl;
	std::cout << factorial4 (7) << std::endl;
	std::cout << factorial4 (8) << std::endl;
	std::cout << factorial4 (9) << std::endl;
	std::cout << factorial5 <int, 0>::value << std::endl;
	std::cout << factorial5 <int, 1>::value << std::endl;
	std::cout << factorial5 <int, 2>::value << std::endl;
	std::cout << factorial5 <int, 3>::value << std::endl;
	std::cout << factorial5 <int, 4>::value << std::endl;
	std::cout << factorial5 <int, 5>::value << std::endl;
	std::cout << factorial5 <int, 6>::value << std::endl;
	std::cout << factorial5 <int, 7>::value << std::endl;
	std::cout << factorial5 <int, 8>::value << std::endl;
	std::cout << factorial5 <int, 9>::value << std::endl;
	std::cout << factorial6 <int, 0>::value << std::endl;
	std::cout << factorial6 <int, 1>::value << std::endl;
	std::cout << factorial6 <int, 2>::value << std::endl;
	std::cout << factorial6 <int, 3>::value << std::endl;
	std::cout << factorial6 <int, 4>::value << std::endl;
	std::cout << factorial6 <int, 5>::value << std::endl;
	std::cout << factorial6 <int, 6>::value << std::endl;
	std::cout << factorial6 <int, 7>::value << std::endl;
	std::cout << factorial6 <int, 8>::value << std::endl;
	std::cout << factorial6 <int, 9>::value << std::endl;
	std::cout << "====================" << std::endl;

	std::cout << fib <0>::value << std::endl;
	std::cout << fib <1>::value << std::endl;
	std::cout << fib <2>::value << std::endl;
	std::cout << fib <3>::value << std::endl;
	std::cout << fib <4>::value << std::endl;
	std::cout << fib <5>::value << std::endl;
	std::cout << "====================" << std::endl;

	std::cout << fib2 <0>::value << std::endl;
	std::cout << fib2 <1>::value << std::endl;
	std::cout << fib2 <2>::value << std::endl;
	std::cout << fib2 <3>::value << std::endl;
	std::cout << fib2 <4>::value << std::endl;
	std::cout << fib2 <5>::value << std::endl;
}
