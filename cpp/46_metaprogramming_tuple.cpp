# include <concepts>
# include <cstddef>
# include <iostream>
# include <string>
# include <utility>



template <typename ...>
struct Tuple;

template <>
struct Tuple <> {
	template <std::size_t N, typename T, typename ... Ts>
	friend constexpr decltype (auto) get (Tuple <T, Ts ...> & tuple) {
		if constexpr (0 == N) {
			return (tuple.value);
		}
		else {
			return get <N - 1> (static_cast <Tuple <Ts ...> &> (tuple));
		}
	}

	template <std::size_t N, typename T, typename ... Ts>
	friend constexpr decltype (auto) get (const Tuple <T, Ts ...> & tuple) {
		if constexpr (0 == N) {
			return tuple.value;
		}
		else {
			return get <N - 1> (static_cast <const Tuple <Ts ...> &> (tuple));
		}
	}
};

template <typename T, typename ... Ts>
struct Tuple <T, Ts ...> : Tuple <Ts ...> {
	T value;

	template <typename T_, typename ... Ts_>
	requires std::constructible_from <T, T_> && (std::constructible_from <Ts, Ts_> && ...)
	constexpr Tuple (T_ && v, Ts_ && ... vs)
		: Tuple <Ts ...> (std::forward <Ts_> (vs) ...)
		, value (std::forward <T_> (v))
	{}
};

template <typename ... Ts>
Tuple (Ts && ...) -> Tuple <std::decay_t <Ts> ...>;



namespace std {

template <typename ... Ts>
struct tuple_size <Tuple <Ts ...>>
	: std::integral_constant <std::size_t, sizeof ... (Ts)> {};

template <std::size_t N, typename T, typename ... Ts>
struct tuple_element <N, Tuple <T, Ts ...>>
	: tuple_element <N - 1, Tuple <Ts ...>> {};

template <typename T, typename ... Ts>
struct tuple_element <0, Tuple <T, Ts ...>> {
	using type = T;
};

}



int main () {
	int x = 22;
	Tuple <int, std::string, double, int &> t (10, "xyz", 2.2, x);
	std::cout << get <0> (t) << std::endl;
	get <1> (t) = "hhh";
	std::cout << get <1> (t) << std::endl;
	std::cout << get <2> (t) << std::endl;
	x = 33;
	std::cout << get <3> (t) << std::endl;
	auto [a, b, c, d] = t;
	std::cout << a << " - " << b << " - " << c << " - " << d << std::endl;
	get <3> (t) = 44;

	std::cout << x << std::endl;

	Tuple t1 (x, 1.1, 'a');
	// auto [a1, b1, c1] = t1;
	x = 55;
	std::cout << get <0> (t1) << std::endl;
	std::cout << get <1> (t1) << std::endl;
	std::cout << get <2> (t1) << std::endl;
}
