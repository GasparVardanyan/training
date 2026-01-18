# include <iomanip>
# include <iostream>
# include <type_traits>
# include <utility>



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



static constexpr unsigned factorial (unsigned n) {
	if (0 == n) {
		return 1;
	}
	else {
		return n * factorial (n - 1);
	}
}



template <unsigned N>
struct factorial1 {
	static constexpr unsigned value = N * factorial1 <N - 1>::value;
};

template <>
struct factorial1 <0> {
	static constexpr unsigned value = 1;
};



template <unsigned N>
struct factorial2 : std::integral_constant <unsigned, N * factorial2 <N - 1>::value>
{};

template <>
struct factorial2 <0> : std::integral_constant <unsigned, 1> {};



template <typename T>
concept MyIntegralType = std::is_integral_v <T>;

template <typename T>
concept MyUnsignedType = std::is_unsigned_v <T>;

template <MyIntegralType T, T V>
struct MyIntegralConstant {
	static constexpr T value = V;
};

template <unsigned N>
struct factorial3 : MyIntegralConstant <unsigned, N * factorial3 <N - 1>::value> {};

template <>
struct factorial3 <0> : MyIntegralConstant <unsigned, 1> {};



template <std::size_t N, MyUnsignedType T = unsigned, typename TS = std::make_integer_sequence <T, N>>
struct factorial4;

template <std::size_t N, MyUnsignedType T, T ... TS>
struct factorial4 <N, T, std::integer_sequence <T, TS ...>> {
	static constexpr T value = ((TS + 1) * ... * static_cast <T> (1));
};



template <typename T>
struct MyIsIntegralConstantType {
	static constexpr bool value = false;
};

template <MyIntegralType T, T V>
struct MyIsIntegralConstantType <MyIntegralConstant <T, V>> {
	static constexpr bool value = true;
};

template <typename T>
concept MyIntegralConstantType = MyIsIntegralConstantType <T>::value;

template <MyIntegralConstantType ... TS>
struct MyIntegralConstantSequence {};

template <MyIntegralType T, std::size_t N, MyIntegralConstantType ... S>
struct MyMakeIntegralConstantSequenceImpl : MyMakeIntegralConstantSequenceImpl <T, N - 1, MyIntegralConstant <T, N - 1>, S ...> {};

template <MyIntegralType T, MyIntegralConstantType ... S>
struct MyMakeIntegralConstantSequenceImpl <T, 0, S ...> {
	using type = MyIntegralConstantSequence <S ...>;
};

template <MyIntegralType T, std::size_t N>
using MyMakeIntegralConstantSequence = MyMakeIntegralConstantSequenceImpl <T, N>::type;

template <std::size_t N, MyUnsignedType T = unsigned, typename S = MyMakeIntegralConstantSequence <T, N>>
struct factorial5;

template <std::size_t N, MyUnsignedType T, MyIntegralConstantType ... S>
struct factorial5 <N, T, MyIntegralConstantSequence <S ...>> {
	static constexpr T value = ((1 + S::value) * ... * 1);
};



template <MyUnsignedType T, T N>
struct MyDigitCount : MyIntegralConstant <T, [] () {
	if constexpr (N < 10) {
		return 1;
	}
	else {
		return 1 + MyDigitCount <T, N / 10>::value;
	}
} ()> {};

template <MyIntegralType T, T first, T second, T ... rest>
struct MyMaxValue : MyMaxValue <T, (first > second ? first : second), rest ...> {};

template <MyIntegralType T, T first, T second>
struct MyMaxValue <T, first, second> : MyIntegralConstant <T, (first > second ? first : second)> {};

template <MyUnsignedType T, std::size_t W, T ... VS>
static void print () {
	((std::cout << std::setw (W) << VS), ...) << std::endl;
}

template <MyUnsignedType T, T ... VS>
static void printFactorials () {
	constexpr std::size_t w = 1 + MyDigitCount <T, MyMaxValue <T, factorial1 <VS>::value ...>::value>::value;
	(
		(print <T, w,
			factorial (VS),
			factorial1 <VS>::value,
			factorial2 <VS>::value,
			factorial3 <VS>::value,
			factorial4 <VS>::value,
			factorial5 <VS>::value
		> ())
		, ...
	);
}



int main ()
{
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

	std::cout << "====================" << std::endl;

	printFactorials <unsigned, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9> ();
}
