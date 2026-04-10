# include <concepts>
# include <iostream>
# include <map>
# include <string>
# include <tuple>
# include <type_traits>

namespace with_tuple {

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

template <typename T1, typename T2>
std::tuple <
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>,
	SignedCommonNumberT <T1, T2>
> foo (const T1 & v1, const T2 & v2) {
	return { v1 + v2, v1 - v2, v1 * v2, v1 / v2 };
}

}

template <typename ...>
struct TupleElemList;

template <typename T>
struct TupleElemList <T> {
	using type = T;
	using next_type = void;
	template <typename T_>
	requires std::constructible_from <T, T_>
	TupleElemList (T_ && v)
		: data (std::forward <T_> (v))
	{
	}

	TupleElemList(const TupleElemList &) = delete;
	TupleElemList(TupleElemList &&) = delete;
	TupleElemList &operator=(const TupleElemList &) = delete;
	TupleElemList &operator=(TupleElemList &&) = delete;

	T data;
};

template <typename T1, typename T2, typename ... Ts>
struct TupleElemList <T1, T2, Ts ...> {
	using type = T1;

	template <typename T1_, typename T2_, typename ... Ts_>
	requires std::constructible_from <T1, T1_> && std::constructible_from <T2, T2_> && (std::constructible_from <Ts, Ts_> && ...)
	TupleElemList (T1_ && v1, T2_ && v2, Ts_ && ... vs)
		: data (std::forward <T1_> (v1))
		, next (TupleElemList <T2, Ts ...> (std::forward <T2_> (v2), std::forward <Ts_> (vs) ...))
	{}

	TupleElemList(const TupleElemList &) = delete;
	TupleElemList(TupleElemList &&) = delete;
	TupleElemList &operator=(const TupleElemList &) = delete;
	TupleElemList &operator=(TupleElemList &&) = delete;

	T1 data;
	TupleElemList <T2, Ts...> next;
};

template <typename ... Ts>
struct Tuple {
	template <typename ... Ts_>
	requires (std::constructible_from<Ts, Ts_> && ...)
	Tuple (Ts_ && ... vs)
		: elems (TupleElemList <Ts ...> (std::forward <Ts_> (vs) ...))
	{
	}

	Tuple(const Tuple &) = delete;
	Tuple(Tuple &&) = delete;
	Tuple &operator=(const Tuple &) = delete;
	Tuple &operator=(Tuple &&) = delete;

	TupleElemList <Ts ...> elems;
};

template <std::size_t N, typename ... Ts>
decltype (auto) Get (Tuple <Ts ...> & tuple) {
	return Get <N> (tuple.elems);
}

template <std::size_t N, typename ... Ts>
decltype (auto) Get (TupleElemList <Ts ...> & tel)
{
	if constexpr (0 == N) {
		return (tel.data);
	}
	else {
		return Get <N - 1> (tel.next);
	}
}


int main () {

	Tuple <int, std::string, double> t (10, "xyz", 2.2);
	std::cout << Get <0> (t) << std::endl;
	Get <1> (t) = "hhh";
	std::cout << Get <1> (t) << std::endl;
	std::cout << Get <2> (t) << std::endl;

	{
		using namespace with_tuple;

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



	std::tuple <int, float, std::string> tpl (10, 20.30, "abc");
	std::cout << std::get <0> (tpl) << std::endl;
	std::cout << std::get <1> (tpl) << std::endl;
	std::cout << std::get <2> (tpl) << std::endl;


}
