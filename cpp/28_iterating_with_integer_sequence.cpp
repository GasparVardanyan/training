# include <iostream>
# include <ostream>
# include <tuple>
# include <type_traits>
# include <utility>

template <std::size_t c, typename ... TS>
struct GetTypeCountImpl;

template <std::size_t c>
struct GetTypeCountImpl <c> : std::integral_constant <std::size_t, c> {};

template <std::size_t c, typename TFirst, typename ... TRest>
struct GetTypeCountImpl <c, TFirst, TRest ...>
	: GetTypeCountImpl <c + 1, TRest ...> {};

template <typename ... TS>
using GetTypeCount = GetTypeCountImpl <0, TS ...>;

template <typename T>
struct GetTupleTypeCount {
	static_assert (false, "T is not a tuple");
};

template <typename ... TS>
struct GetTupleTypeCount <std::tuple <TS ...>>
	: GetTypeCount <TS ...> {};

template <typename Tuple, typename = std::make_index_sequence <GetTupleTypeCount <Tuple>::value>>
struct TupleHelper;

template <typename Tuple, std::size_t ... indices>
struct TupleHelper <Tuple, std::index_sequence <indices ...>> {
	using tuple_t = std::decay_t <Tuple>;

	template <typename ApplyTuple>
	requires std::constructible_from <tuple_t, ApplyTuple>
	static std::ostream & print (std::ostream & os, ApplyTuple && tuple) {
		((os << std::get <indices> (std::forward <ApplyTuple> (tuple)) << ", "), ...);
		return os;
	}

	template <typename F, typename ApplyTuple>
	requires
		   std::constructible_from <tuple_t, ApplyTuple>
		&& std::invocable <F, decltype (std::get <indices> (std::declval <ApplyTuple> ())) ...>
	static inline decltype (auto) apply (F && f, ApplyTuple && tuple) {
		return std::forward <F> (f) (std::get <indices> (std::forward <ApplyTuple> (tuple)) ...);
	}
};

template <typename ... TS>
std::ostream & operator << (std::ostream & os, const std::tuple <TS ...> & tpl) {
	TupleHelper <std::tuple <TS ...>>::print (os, tpl);

	return os;
}

namespace detail {
template <typename T>
struct VerifyTuple {
	static_assert (false, "T is not a tuple");
};

template <typename ... TS>
struct VerifyTuple <std::tuple <TS ...>> {};

template <typename F, typename Tuple, std::size_t ... idx>
decltype (auto) apply (F && f, Tuple && tuple, std::index_sequence <idx ...> &&) {
	(void) VerifyTuple <std::decay_t <Tuple>> {};
	return std::forward <F> (f) (std::get <idx> (std::forward <Tuple> (tuple)) ...);
}
}

template <typename F, typename Tuple>
decltype (auto) MyApply (F && f, Tuple && tuple) {
	return detail::apply (std::forward <F> (f), std::forward <Tuple> (tuple), std::make_index_sequence <std::tuple_size_v <std::decay_t <Tuple>>> {});
}

void func (const char * s, double d, int i, char c) {
	std::cout << "\tstr: " << s << std::endl;
	std::cout << "\tdouble: " << d << std::endl;
	std::cout << "\tint: " << i << std::endl;
	std::cout << "\tchar: " << c << std::endl;
}

int main () {
	auto bar = std::make_tuple ("test", 3.1, 14, 'y');
	apply (func, bar); // ADL find std::apply
	std::cout << "TC: " << GetTupleTypeCount <decltype (bar)>::value << std::endl;
	std::cout << bar << std::endl;
	TupleHelper <decltype (bar)>::apply (func, bar);
	std::cout << "====================" << std::endl;
	MyApply (func, bar);

	return 0;
}
