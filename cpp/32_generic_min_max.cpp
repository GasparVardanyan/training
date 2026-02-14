# include <iostream>
# include <type_traits>

template <typename TFirst, typename TSecond>
constexpr auto genericMin (TFirst && first, TSecond && second)
	-> std::common_type_t <TFirst, TSecond>
{
	if (first < second) {
		return std::forward <TFirst> (first);
	}
	else {
		return std::forward <TSecond> (second);
	}
}

template <typename TFirst, typename TSecond, typename ... Rest>
constexpr auto genericMin (TFirst && first, TSecond && second, Rest && ... rest)
	-> std::common_type_t <TFirst, TSecond, Rest ...>
{
	return genericMin (
		genericMin (std::forward <TFirst> (first), std::forward <TSecond> (second)),
		std::forward <Rest> (rest) ...
	);
}

int main () {
	std::cout << genericMin (10, 8, 29) << std::endl;
	std::cout << std::is_same_v <decltype (genericMin (10.1f, 2)), float> << std::endl;
}
