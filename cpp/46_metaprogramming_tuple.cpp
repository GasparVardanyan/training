# include <cstddef>
# include <iostream>
# include <utility>
# include <type_traits>

template <typename ...>
struct Tuple;

template <std::size_t N, typename T, typename ... Ts>
struct NthType : NthType <N - 1, Ts ...> {};

template <typename T, typename ... Ts>
struct NthType <0, T, Ts ...> {
	using type = T;
};

template <std::size_t N, typename T, typename ... Ts>
struct NthInheritanceLevelTuple : NthInheritanceLevelTuple <N - 1, Ts ...> {};

template <typename T, typename ... Ts>
struct NthInheritanceLevelTuple <0, T, Ts ...> {
	using type = Tuple <T, Ts ...>;
};

template <typename T, typename ... Ts>
struct Tuple <T, Ts ...> : Tuple <Ts ...> {
	template <typename U, typename ... Us>
	Tuple (U && u, Us && ... us)
		requires (false == std::is_same_v <std::remove_cvref_t <U>, std::remove_cvref_t <Tuple>>)
		: Tuple <Ts ...> (std::forward <Us> (us) ...)
		, value (std::forward <U> (u))
	{}

	template <std::size_t N, typename U, typename ... Us>
	constexpr friend NthType <N, U, Us ...>::type & get (Tuple <U, Us ...> & tuple) noexcept;

	template <std::size_t N, typename U, typename ... Us>
	constexpr friend const NthType <N, U, Us ...>::type & get (const Tuple <U, Us ...> & tuple) noexcept;

	template <std::size_t N, typename U, typename ... Us>
	constexpr friend NthType <N, U, Us ...>::type && get (Tuple <U, Us ...> && tuple) noexcept;

private:
	T value;
};

template <> struct Tuple <> {
	template <std::size_t N, typename U, typename ... Us>
	constexpr friend NthType <N, U, Us ...>::type & get (Tuple <U, Us ...> & tuple) noexcept {
		return static_cast <NthInheritanceLevelTuple <N, U, Us ...>::type &> (tuple).value;
	}

	template <std::size_t N, typename U, typename ... Us>
	constexpr friend const NthType <N, U, Us ...>::type & get (const Tuple <U, Us ...> & tuple) noexcept {
		return static_cast <const NthInheritanceLevelTuple <N, U, Us ...>::type &> (tuple).value;
	}

	template <std::size_t N, typename U, typename ... Us>
	constexpr friend NthType <N, U, Us ...>::type && get (Tuple <U, Us ...> && tuple) noexcept {
		return static_cast <NthInheritanceLevelTuple <N, U, Us ...>::type &&> (tuple).value;
	}
};

template <typename ... Ts>
Tuple (Ts ...) -> Tuple <Ts ...>;

namespace std {
template <typename ... Ts>
struct tuple_size <Tuple <Ts ...>>
	: std::integral_constant <std::size_t, sizeof ... (Ts)> {};

template <std::size_t N, typename ... Ts>
struct tuple_element <N, Tuple <Ts ...>> {
	using type = NthType <N, Ts ...>::type;
};
}

int main () {
	Tuple t (1, 2.3, "def");

	{
		auto & [a, b, c] = t;
		std::cout << a << ", " << b << ", " << c << std::endl;
		b = 222;
	}

	{
		auto [a, b, c] = t;

		std::cout << a << ", " << b << ", " << c << std::endl;
	}
}

