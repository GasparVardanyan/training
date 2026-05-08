# include <array>
# include <concepts>
# include <cstdio>
# include <iostream>
# include <iterator>
# include <map>
# include <string>
# include <tuple>
# include <type_traits>
# include <vector>
# include <algorithm>
# include <utility>



namespace using_tuple                      { void run ();   }
namespace using_structured_bindings        { void run ();   }
namespace tuple_like                       { void run ();   }
namespace using_struct                     { void run ();   }
namespace using_output_parameters          { void run ();   }
namespace using_function_object_consumer   { void run ();   }
namespace using_pair                       { void run ();   }
namespace using_array                      { void run ();   }
namespace using_output_iterator            { void run ();   }
namespace using_vector                     { void run ();   }



int main () {
	using_tuple::run ();
	using_structured_bindings::run ();
	tuple_like::run ();
	using_struct::run ();
	using_output_parameters::run ();
	using_function_object_consumer::run ();
	using_pair::run ();
	using_array::run ();
	using_output_iterator::run ();
	using_vector::run ();
}



namespace using_vector {
std::vector <int> foo (int a, int b) {
	return { a + b, a - b, a * b, a / b };
}

void run () {
	auto v = foo (50, 60);

	std::copy (v.cbegin (), v.cend (), std::ostream_iterator <int> (std::cout, ", "));
	std::cout << std::endl;
}
}

namespace using_output_iterator {
void foo (int a, int b, std::output_iterator <int> auto it) {
	* it = a + b, ++it;
	* it = a - b, ++it;
	* it = a * b, ++it;
	* it = a / b, ++it;
}

void run () {
	foo (10, 20, std::ostream_iterator <int> (std::cout, ", "));
	std::cout << std::endl;
}
}

namespace using_array {
std::array <int, 4> foo (int a, int b) {
	return { a + b, a - b, a * b, a / b };
}

void run () {
	auto [a, b, c, d] = foo (20, 30);

	std::cout << a << " - " << b << " - " << c << " - " << d << std::endl;
}
}

namespace using_pair {
std::pair <int, int> foo (int a, int b) {
	return { a + b, a - b };
}

void run () {
	auto [a, b] = foo (10, 20);

	std::cout << a << " - " << b << std::endl;
}
}

namespace using_function_object_consumer {
template <std::invocable <int, int, int, int> F>
void foo (int a, int b, F && f) {
	std::forward <F> (f) (a + b, a - b, a * b, a / b);
}

template <typename Tuple>
struct continuation {
	Tuple t;

	template <typename F>
	decltype (auto) operator->* (F && f) && {
		return std::apply (std::forward <F> (f), std::move (t));
	}

	template <typename F>
	decltype (auto) operator () (F && f) && {
		return std::apply (std::forward <F> (f), std::move (t));
	}
};

std::tuple <int, int, int, int> bar (int a, int b) {
	return { a + b, a - b, a * b, a / b };
}

void run () {
	int a, b, c, d;
	foo (10, 20, [&] (int add, int sub, int mul, int div) -> void {
		a = add, b = sub, c = mul, d = div;
	});
	std::cout << a << " - " << b << " - " << c << " - " << d << std::endl;

	std::cout << continuation (bar (10, 20))->* [] (int sum, auto && ...) -> int {
		return sum;
	} << std::endl;

	std::cout << continuation (bar (10, 20)) ([] (int sum, auto && ...) -> int {
		return sum;
	}) << std::endl;
}
}

namespace using_output_parameters {
void foo (int a, int b, int & add, int & sub, int & mul, int & div) {
	add = a + b;
	sub = a - b;
	mul = a * b;
	div = a / b;
}

void run () {
	int a, b, c, d;
	foo (10, 20, a, b, c, d);
	std::cout << a << " - " << b << " - " << c << " - " << d << std::endl;
}
}

namespace using_struct {
struct foo_result {
	int add, sub, mul, div;
};

foo_result foo (int a, int b) {
	return {
		.add = a + b,
		.sub = a - b,
		.mul = a * b,
		.div = a / b
	};
}

struct foo_calculator : foo_result {
	foo_calculator (int a, int b) {
		add = a + b;
		sub = a - b;
		mul = a * b;
		div = a / b;
	}
};

void run () {
	{
		auto [a, b, c, d] = foo (10, 20);
		std::cout << a << " - " << b << " - " << c << " - " << d << std::endl;
	}
	{
		auto [a, b, c, d] = foo_calculator (10, 20);
		std::cout << a << " - " << b << " - " << c << " - " << d << std::endl;
	}
}
}

namespace tuple_like {
class TupleLike {
public:
	TupleLike (int i, double d, const std::string & s)
		: m_i (i), m_d (d), m_s (s)
	{}

	int & i () & {
		return m_i;
	}

	const int & i () const & {
		return m_i;
	}

	int && i () && {
		return std::move (m_i);
	}

	double & d () & {
		return m_d;
	}

	const double & d () const & {
		return m_d;
	}

	double && d () && {
		return std::move (m_d);
	}

	std::string & s () & {
		return m_s;
	}

	const std::string & s () const & {
		return m_s;
	}

	std::string && s () && {
		return std::move (m_s);
	}


private:
	int m_i;
	double m_d;
	std::string m_s;
};
}

namespace std {
template <>
struct tuple_size <tuple_like::TupleLike> : std::integral_constant <std::size_t, 3> {};

template <>
struct tuple_element <0, tuple_like::TupleLike> { using type = int; };
template <>
struct tuple_element <1, tuple_like::TupleLike> { using type = double; };
template <>
struct tuple_element <2, tuple_like::TupleLike> { using type = std::string; };
}

namespace tuple_like {
template <std::size_t N>
std::tuple_element_t <N, TupleLike> & get (TupleLike & t) {
	if constexpr (0 == N) {
		return t.i ();
	}
	else if constexpr (1 == N) {
		return t.d ();
	}
	else if constexpr (2 == N) {
		return t.s ();
	}
	else {
		static_assert (false, "index out of range");
	}
}

template <std::size_t N>
const std::tuple_element_t <N, TupleLike> & get (const TupleLike & t) {
	if constexpr (0 == N) {
		return t.i ();
	}
	else if constexpr (1 == N) {
		return t.d ();
	}
	else if constexpr (2 == N) {
		return t.s ();
	}
	else {
		static_assert (false, "index out of range");
	}
}

template <std::size_t N>
std::tuple_element_t <N, TupleLike> && get (TupleLike && t) {
	if constexpr (0 == N) {
		return static_cast <TupleLike &&> (t).i ();
	}
	else if constexpr (1 == N) {
		return static_cast <TupleLike &&> (t).d ();
	}
	else if constexpr (2 == N) {
		return static_cast <TupleLike &&> (t).s ();
	}
	else {
		static_assert (false, "index out of range");
	}
}

void run () {
	std::cout << "X" << std::endl;

	std::cout << get <0> (TupleLike (1, 2.3, "xxx")) << std::endl;
	std::cout << get <1> (TupleLike (1, 2.3, "xxx")) << std::endl;
	std::cout << get <2> (TupleLike (1, 2.3, "xxx")) << std::endl;

	TupleLike t (4, 5.6, "yyy");

	auto & [a, b, c] = t;
	a = 10;
	b = 20;
	c = "zzz";

	auto [d, e, f] = t;
	std::cout << d << " - " << e << " - " << f << std::endl;
}
}

namespace using_structured_bindings {
void run () {
	{
		std::map <std::string, int> m;
		auto [it, success] = m.insert ({"Hello", 42});

		if (true == success) {
			std::cout << "inserted: " << m.at ("Hello") << std::endl;
		}

		for (const auto & [key, value] : m) {
			std::cout << "key: " << key << ", value: " << value << std::endl;
		}
	}

	{

		struct B { int x; int y; };

		auto [a, b] = B { 10, 20 };

		std::cout << "A: " << a << "B: " << b << std::endl;
	}
}
}

namespace using_tuple {
std::tuple <int, int, int, int> foo (int a, int b) {
	return std::make_tuple (a + b, a - b, a * b, a / b);
	// return { a + b, a - b, a * b, a / b };
}

std::tuple <int &, int &> minmax (int & a, int & b) {
	return a < b ? std::tie (a, b) : std::tie (b, a);
}

void run () {
	auto mrvs = foo (100, 50);

	{
		auto add = std::get <0> (mrvs);
		auto sub = std::get <1> (mrvs);
		auto mul = std::get <2> (mrvs);
		auto div = std::get <3> (mrvs);

		std::printf ("%d - %d - %d - %d\n", add, sub, mul, div);
	}

	{
		int add, sub, div;
		std::tie (add, sub, std::ignore, div) = mrvs;

		std::printf ("%d - %d - %d\n", add, sub, div);
	}

	{
		auto [add, sub, mul, div] = mrvs;

		std::printf ("%d - %d - %d - %d\n", add, sub, mul, div);
	}

	{
		int a = 20;
		int b = 30;

		auto [min, max] = minmax (a, b);

		a *= 10;
		b *= 10;

		std::cout << min << " - " << max << std::endl;

		std::get <1> (minmax (a, b)) += 1;

		std::cout << max << std::endl;
	}
}
}
