# include <concepts>
# include <iostream>
# include <map>
# include <string>
# include <tuple>
# include <type_traits>
# include <utility>



template <typename T>
requires std::integral <T> || std::floating_point <T>
struct SignedNumber;

template <std::integral T>
struct SignedNumber <T> : std::make_signed <T> {};

template <std::floating_point T>
struct SignedNumber <T> {
	using type = T;
};

template <typename T>
using SignedNumberT = SignedNumber <T>::type;

template <typename ... Ts>
struct SignedCommonNumber : SignedNumber <std::common_type_t <Ts ...>> {};

template <typename ... Ts>
using SignedCommonNumberT = SignedCommonNumber <Ts ...>::type;



namespace structured_bindings {

void run () {
	int x = 10;
	std::get <0> (std::tie (x)) = 20;
	std::cout << x << std::endl;

	std::map <std::string, int> m;
	{
		auto [iterator, success] = m.insert ({"Hello", 42});
		std::cout << success << std::endl;
	}
	{
		auto [iterator, success] = m.insert ({"Hello", 42});
		std::cout << success << std::endl;
	}
	{
		auto [iterator, success] = m.insert ({"Hello", 43});
		std::cout << success << std::endl;
	}

	m.insert ({"hello", 0});
	m.insert ({"world", 1});

	for (const auto & [key, value] : m) {
		std::cout << key << ": " << value << std::endl;
	}

	{
		struct S {
			int x;
			int y;
		};

		S s = { .x = 10, .y = 20 };

		auto [x, y] = s;
		std::cout << x << ", " << y << std::endl;
	}
}

}



namespace using_tuple {

template <typename T1, typename T2>
std::tuple <
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>
> foo (const T1 & v1, const T2 & v2) {
	return { v1 + v2, v1 - v2, v1 * v2, v1 / v2 };
}


void run () {
	auto [v1, v2, v3, v4] = foo (10.f, 20.l);
	std::cout << v1 << ", " << v2 << ", " << v3 << ", " << v4 << std::endl;

	std::tie (v1, v2, std::ignore, v4) = foo (20, 30.l);
	std::cout << v1 << ", " << v2 << ", " << v3 << ", " << v4 << std::endl;

	auto vs = foo (30, 40);
	std::cout << std::get <0> (vs) << std::endl;
	std::cout << std::get <1> (vs) << std::endl;
	std::cout << std::get <2> (vs) << std::endl;
	std::cout << std::get <3> (vs) << std::endl;
}

}



namespace tuple_like {

struct S {
public:
	int get_int () const { return _int; }
	std::string get_string () const { return _string; }
	double get_double () const { return _double; }

private:
	int _int = 10;
	std::string _string = "ABC";
	double _double = 1.1;
};

template <std::size_t N>
decltype (auto) get (const tuple_like::S & s) {
	if constexpr (0 == N) {
		return (s.get_int ());
	}
	else if constexpr (1 == N) {
		return (s.get_string ());
	}
	else if constexpr (2 == N) {
		return (s.get_double ());
	}
}

}

namespace std {

template <>
struct tuple_size <tuple_like::S> : std::integral_constant <std::size_t, 3> {};


template <>
struct tuple_element <0, tuple_like::S> { using type = int; };
template <>
struct tuple_element <1, tuple_like::S> { using type = std::string; };
template <>
struct tuple_element <2, tuple_like::S> { using type = double; };

}

namespace tuple_like {

void run () {
	S s;

	auto [v1, v2, v3] = s;
	std::cout << v1 << std::endl;
	std::cout << v2 << std::endl;
	std::cout << v3 << std::endl;
}

}



namespace using_struct {

template <typename T1, typename T2>
struct foo_return_type {
	SignedCommonNumberT <T1, T2> add;
	SignedCommonNumberT <T1, T2> sub;
	SignedCommonNumberT <T1, T2> mul;
	SignedCommonNumberT <T1, T2> div;
};

template <typename T1, typename T2>
foo_return_type <T1, T2> foo (T1 a, T2 b) {
	return {
		.add = a + b,
		.sub = a - b,
		.mul = a * b,
		.div = a / b
	};
}

template <typename T1, typename T2>
struct bar {
	SignedCommonNumberT <T1, T2> add;
	SignedCommonNumberT <T1, T2> sub;
	SignedCommonNumberT <T1, T2> mul;
	SignedCommonNumberT <T1, T2> div;

	bar (T1 a, T2 b)
		: add (a + b)
		, sub (a - b)
		, mul (a * b)
		, div (a / b)
	{}
};

void run () {
	auto [a, s, m, d] = foo (20, 30.);
	std::cout << a << " - " << s << " - " << m << " - " << d << std::endl;
	auto [a1, s1, m1, d1] = bar (10, 25.);
	std::cout << a1 << " - " << s1 << " - " << m1 << " - " << d1 << std::endl;
}

}



namespace using_output_parameters {

template <typename T1, typename T2, typename ... Ts>
requires
	   (4 == sizeof ... (Ts))
	&& (std::constructible_from <std::decay_t <Ts>, SignedCommonNumberT <T1, T2>> && ...)
void foo (T1 a, T2 b, std::decay_t <Ts> & ... vs) {
	[&] <std::size_t ... Is> (std::index_sequence <Is ...>) constexpr -> void {
		((
			0 == Is ? vs = a + b :
			1 == Is ? vs = a - b :
			2 == Is ? vs = a * b :
			vs = a / b
		), ...);
	} (std::make_index_sequence <4> {});

	// std::size_t i = 0;
	//
	// ((
	// 	0 == i ? vs = a + b :
	// 	1 == i ? vs = a - b :
	// 	2 == i ? vs = a * b :
	// 	vs = a / b, ++i
	// ), ...);
}

void run () {
	float a, b, c, d;

	foo <float, float, float, float, float, float> (10.f, 20, a, b, c, d);
	std::cout << a << " - " << b << " - " << c << " - " << d << std::endl;
}

}



namespace using_function_object_consumer {

template <typename T1, typename T2, typename C>
requires std::invocable <
	C,
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>
>
void foo (T1 a, T2 b, const C & consumer) {
	consumer (a + b, a - b, a * b, a / b);
}

void run () {
	foo (100, 20.0, [] (auto a, auto s, auto m, auto d) -> void {
		std::cout << a << " - " << s << " - " << m << " - " << d << std::endl;
	});
}

}



int main () {
	structured_bindings::run ();
	using_tuple::run ();
	tuple_like::run ();
	using_struct::run ();
	using_output_parameters::run ();
	using_function_object_consumer::run ();
}
