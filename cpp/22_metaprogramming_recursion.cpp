# include <concepts>
# include <iomanip>
# include <iostream>
# include <ostream>
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



//   __            _             _       _   _          _
//  / _| __ _  ___| |_ ___  _ __(_) __ _| | | |__   ___| |_ __   ___ _ __ ___
// | |_ / _` |/ __| __/ _ \| '__| |/ _` | | | '_ \ / _ \ | '_ \ / _ \ '__/ __|
// |  _| (_| | (__| || (_) | |  | | (_| | | | | | |  __/ | |_) |  __/ |  \__ \
// |_|  \__,_|\___|\__\___/|_|  |_|\__,_|_| |_| |_|\___|_| .__/ \___|_|  |___/
//                                                       |_|

template <typename F>
struct y_combinator {
	using func_t = std::decay_t <F>;

	template <typename G>
	requires std::constructible_from <func_t, G>
	constexpr explicit y_combinator (G && f)
		: func_ (std::forward <G> (f)) {}

	template <typename ... Args>
	requires std::invocable <func_t, y_combinator const &, Args ...>
	constexpr std::invoke_result_t <func_t, y_combinator const &, Args ...>
	operator () (Args && ... args) const &
		noexcept (std::is_nothrow_invocable_v <func_t, y_combinator const &, Args ...>)
	{
		return func_ (* this, std::forward <Args> (args) ...);
	}

	template <typename ... Args>
	requires std::invocable <func_t, y_combinator &, Args ...>
	constexpr std::invoke_result_t <func_t, y_combinator &, Args ...>
	operator () (Args && ... args) &
		noexcept (std::is_nothrow_invocable_v <func_t, y_combinator &, Args ...>)
	{
		return func_ (* this, std::forward <Args> (args) ...);
	}

	template <typename ... Args>
	requires std::invocable <func_t, y_combinator &&, Args ...>
	constexpr std::invoke_result_t <func_t, y_combinator &&, Args ...>
	operator () (Args && ... args) &&
		noexcept (std::is_nothrow_invocable_v <func_t, y_combinator &&, Args ...>)
	{
		return func_ (std::move (* this), std::forward <Args> (args) ...);
	}

private:
	func_t func_;
};

template <typename F>
y_combinator (F &&) -> y_combinator <std::decay_t <F>>;



template <bool b>
struct BooleanConstant {
	static constexpr bool value = b;
};

struct TrueType : BooleanConstant <true> {};
struct FalseType : BooleanConstant <false> {};

template <typename T> struct RemoveCV                      { using type = T; };
template <typename T> struct RemoveCV <const T>            { using type = T; };
template <typename T> struct RemoveCV <volatile T>         { using type = T; };
template <typename T> struct RemoveCV <const volatile T>   { using type = T; };

template <typename T>
using RemoveCVT = RemoveCV <T>::type;

// NOTE: not exactly standard-conforming behaviour
template <typename T> struct _IsIntegral              : FalseType {};

template <> struct _IsIntegral <char>                 : TrueType {};
template <> struct _IsIntegral <signed char>          : TrueType {};
template <> struct _IsIntegral <unsigned char>        : TrueType {};
template <> struct _IsIntegral <bool>                 : TrueType {};
template <> struct _IsIntegral <signed short>         : TrueType {};
template <> struct _IsIntegral <unsigned short>       : TrueType {};
template <> struct _IsIntegral <signed int>           : TrueType {};
template <> struct _IsIntegral <unsigned int>         : TrueType {};
template <> struct _IsIntegral <signed long>          : TrueType {};
template <> struct _IsIntegral <unsigned long>        : TrueType {};
template <> struct _IsIntegral <long signed long>     : TrueType {};
template <> struct _IsIntegral <unsigned long long>   : TrueType {};

template <typename T>
struct IsIntegral : _IsIntegral <RemoveCVT <T>> {};

template <typename T>
constexpr inline bool IsIntegralV = IsIntegral <T>::value;


// template <typename T> struct _IsUnsignedIntegral              : FalseType {};
//
// template <> struct _IsUnsignedIntegral <unsigned char>        : TrueType {};
// template <> struct _IsUnsignedIntegral <bool>                 : TrueType {};
// template <> struct _IsUnsignedIntegral <unsigned short>       : TrueType {};
// template <> struct _IsUnsignedIntegral <unsigned int>         : TrueType {};
// template <> struct _IsUnsignedIntegral <unsigned long>        : TrueType {};
// template <> struct _IsUnsignedIntegral <unsigned long long>   : TrueType {};
//
// template <typename T>
// struct IsUnsignedIntegral : _IsUnsignedIntegral <RemoveCVT <T>> {};
//
// template <typename T>
// constexpr inline bool IsUnsignedIntegralV = IsUnsignedIntegral <T>::value;


template <typename T, T _value, bool _is_integral = IsIntegralV <T>>
struct IntegralConstantImpl;

template <typename T, T _value>
struct IntegralConstantImpl <T, _value, false> {
	static_assert (false, "T isn't integral");
};

template <typename T, T _value>
struct IntegralConstantImpl <T, _value, true> {
	static constexpr T value = _value;
};

template <typename T, T value>
using IntegralConstant = IntegralConstantImpl <T, value>;

template <typename T>
struct ValidateIntegral {
	static_assert (IsIntegralV <T>, "T is'nt integral");
};

template <typename T, T ... vs>
struct IntegralSequence : ValidateIntegral <T> {};

template <typename T, std::size_t N, T ... vs>
struct MakeIntegralSequenceImpl
	: virtual ValidateIntegral <T>, MakeIntegralSequenceImpl <T, N - 1, vs ..., N - 1>
{};

template <typename T, T ... vs>
struct MakeIntegralSequenceImpl <T, (std::size_t) 0, vs ...> {
	using type = IntegralSequence <T, vs ...>;
};

template <typename T, std::size_t N>
using MakeIntegralSequence
	= MakeIntegralSequenceImpl <T, N>::type;



//   __            _             _       _
//  / _| __ _  ___| |_ ___  _ __(_) __ _| |___
// | |_ / _` |/ __| __/ _ \| '__| |/ _` | / __|
// |  _| (_| | (__| || (_) | |  | | (_| | \__ \
// |_|  \__,_|\___|\__\___/|_|  |_|\__,_|_|___/
//

template <unsigned N>
struct factorial1 {
	static constexpr unsigned value = N * factorial1 <N - 1>::value;
};

template <>
struct factorial1 <0> {
	static constexpr unsigned value = 1;
};

template <unsigned N>
struct factorial2
	: std::integral_constant <unsigned, N * factorial2 <N - 1>::value> {};

template <>
struct factorial2 <0> : std::integral_constant <unsigned, 1> {};

template <unsigned N>
struct factorial3 : std::integral_constant <unsigned, [] () constexpr -> unsigned {
	unsigned n = 1;

	for (unsigned i = 2; i <= N; i++) {
		n *= i;
	}

	return n;
} ()> {};

template <unsigned N, typename = std::make_integer_sequence <unsigned, N>>
struct factorial4;

template <unsigned N, unsigned ... NS>
struct factorial4 <N, std::integer_sequence <unsigned, NS ...>>
	: std::integral_constant <unsigned, ((NS + 1) * ... * 1)> {};

template <unsigned N>
struct factorial5 : std::integral_constant <unsigned, y_combinator ([] <typename Y> (Y && self, unsigned n) -> unsigned {
	if (1 >= n) {
		return 1;
	}
	else {
		return n * self (n - 1);
	}
}) (N)> {};

template <unsigned N, typename = MakeIntegralSequence <unsigned, N>>
struct factorial6;

template <unsigned N, unsigned ... ns>
struct factorial6 <N, IntegralSequence <unsigned, ns ...>>
	: IntegralConstant <unsigned, ((1 + ns) * ... * 1)> {};



//             _       _     _          _
//  _ __  _ __(_)_ __ | |_  | |__   ___| |_ __   ___ _ __ ___
// | '_ \| '__| | '_ \| __| | '_ \ / _ \ | '_ \ / _ \ '__/ __|
// | |_) | |  | | | | | |_  | | | |  __/ | |_) |  __/ |  \__ \
// | .__/|_|  |_|_| |_|\__| |_| |_|\___|_| .__/ \___|_|  |___/
// |_|                                   |_|

template <std::integral T, T ... values>
struct MaxValue;

template <std::integral T, T value>
struct MaxValue <T, value>
	: std::integral_constant <T, value> {};

template <std::integral T, T first, T second, T ... rest>
struct MaxValue <T, first, second, rest ...>
	: MaxValue <T, (first > second ? first : second), rest ...> {};


template <std::unsigned_integral T, T value, std::size_t count = 0, typename = void>
struct UnsignedDigitCount : UnsignedDigitCount <T, value / 10, 1 + count> {};

template <std::unsigned_integral T, T value, std::size_t count>
struct UnsignedDigitCount <T, value, count, std::enable_if_t <0 == value && 0 != count>>
	: std::integral_constant <std::size_t, count> {};


template <std::signed_integral T, T value, typename = void>
struct SignedDigitCount
	: UnsignedDigitCount <std::make_unsigned_t <T>, value> {};

template <std::signed_integral T, T value>
struct SignedDigitCount <T, value, std::enable_if_t <(0 > value)>>
	: UnsignedDigitCount <std::make_unsigned_t <T>, -value> {};


template <std::integral T, T value, typename = void>
struct DigitCount
	: UnsignedDigitCount <T, value> {};

template <std::integral T, T value>
struct DigitCount <T, value, std::void_t <std::enable_if_t <std::is_signed_v <T>>>>
	: SignedDigitCount <T, value> {};


template <std::integral T, T value, typename = void>
struct IntegerCharacterCount
	: DigitCount <T, value> {};

template <std::integral T, T value>
struct IntegerCharacterCount <T, value, std::enable_if_t <std::is_signed_v <T> && (0 > value)>>
	: std::integral_constant <std::size_t, 1 + DigitCount <T, value>::value> {};


template <std::ostream & os, std::size_t w, typename T, T ... args>
static void printw () {
	((os << std::setw (w) << args << ' '), ...) << std::endl;
}

template <std::ostream & os, std::unsigned_integral T, T ... ns>
void printFactorials () {
	constexpr std::size_t maxNumber = MaxValue <T, ns ...>::value;
	constexpr std::size_t wFactorials = IntegerCharacterCount <T, factorial1 <maxNumber>::value>::value;
	constexpr std::size_t wNumbers = IntegerCharacterCount <std::size_t, maxNumber>::value;

	((
		(os << std::setw (wNumbers) << ns << ": "),
		printw <os, wFactorials, T,
			factorial1 <ns>::value,
			factorial2 <ns>::value,
			factorial3 <ns>::value,
			factorial4 <ns>::value,
			factorial5 <ns>::value,
			factorial6 <ns>::value
		> ()
	), ...);
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

	printFactorials <std::cout, unsigned, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10> ();
}
