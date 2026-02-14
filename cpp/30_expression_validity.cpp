# include <functional>
# include <iostream>
# include <map>
# include <type_traits>
# include <unordered_map>
# include <vector>

namespace detail {
	template <typename T, typename = void>
	struct HaveValueTypeImpl : std::false_type {};

	template <typename T>
	struct HaveValueTypeImpl <T, std::void_t <typename T::value_type>>
		: std::true_type {};

	template <typename T, typename = void>
	struct HaveSizeImpl : std::false_type {};

	template <typename T>
	struct HaveSizeImpl <
		T,
		std::void_t <decltype (std::declval <const std::decay_t <T>> ().size ())>
	>
		: std::true_type {};
}

template <typename T>
using HaveValueType = detail::HaveValueTypeImpl <T>;

template <typename T>
inline constexpr bool HaveValueTypeV = HaveValueType <T>::value;

template <typename T>
using HaveSize = detail::HaveSizeImpl <T>;

template <typename T>
inline constexpr bool HaveSizeV = HaveSize <T>::value;



namespace detail {
template <typename T, typename = void>
struct HasHashImpl : std::false_type {};

template <typename T>
// struct HasHashImpl <T, std::void_t <decltype (std::declval <std::hash <T>> () (std::declval <T> ()))>>
struct HasHashImpl <T, std::void_t <decltype (std::hash <T> () (std::declval <T> ()))>>
	: std::true_type {};
}

template <typename T>
using HasHash = detail::HasHashImpl <T>;

template <typename T>
inline constexpr bool HasHashV = HasHash <T>::value;

template <typename T, typename ... TS>
using InvariantHash = std::conditional_t <HasHashV <T>, std::map <T, TS ...>, std::unordered_map <T, TS ...>>;



int main () {
	std::cout << HaveValueTypeV <int> << std::endl;
	std::cout << HaveValueTypeV <std::vector <int>> << std::endl;
	std::cout << HaveSizeV <int> << std::endl;
	std::cout << HaveSizeV <std::vector <int>> << std::endl;

	struct S { };
	std::cout << HasHashV <S> << std::endl;
	std::cout << HasHashV <int> << std::endl;

	std::cout << std::is_same_v <InvariantHash <int, double>, std::map <int, double>> << std::endl;
	std::cout << std::is_same_v <InvariantHash <S, double>, std::unordered_map <S, double>> << std::endl;
}
