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
struct TuplePrintHelper;

template <typename Tuple, std::size_t ... indices>
struct TuplePrintHelper <Tuple, std::index_sequence <indices ...>> {
	static std::ostream & print (std::ostream & os, const Tuple & tuple) {
		((os << std::get <indices> (tuple) << ", "), ...);
		return os;
	}
};

template <typename ... TS>
std::ostream & operator << (std::ostream & os, const std::tuple <TS ...> & tpl) {
	TuplePrintHelper <std::tuple <TS ...>>::print (os, tpl);

	return os;
}

int main () {
	std::cout << GetTypeCount <>::value << std::endl;
	std::cout << GetTypeCount <int>::value << std::endl;
	std::cout << GetTypeCount <int, float>::value << std::endl;
	std::tuple <int, char> foo (10, 'x');
	auto bar = std::make_tuple ("test", 3.1, 14, 'y');

	std::cout << "TC: " << GetTupleTypeCount <decltype (bar)>::value << std::endl;
	std::cout << bar << std::endl;

	std::get <2> (bar) = 122;

	int myint; char mychar;
	std::tie (myint, mychar) = foo;

	std::cout << myint << " - " << mychar << std::endl;

	std::tie (std::ignore, std::ignore, myint, mychar) = bar;

	std::cout << myint << " - " << mychar << std::endl;

	mychar = std::get <3> (bar);
	std::get <0> (foo) = std::get <2> (bar);
	std::get <1> (foo) = std::get <3> (bar);

	std::cout << std::get <0> (foo) << " - " << std::get <1> (foo) << std::endl;

	std::cout << foo << std::endl;
}
