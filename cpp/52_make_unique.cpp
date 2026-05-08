# include <cstddef>
# include <iostream>
# include <memory>
# include <utility>

template <typename T>
struct RemoveExtent {
	using type = T;
};

template <typename T>
struct RemoveExtent <T []> {
	using type = T;
};

template <typename T, std::size_t S>
struct RemoveExtent <T [S]> {
	using type = T;
};

template <typename T>
using RemoveExtentT = RemoveExtent <T>::type;

template <typename T>
struct MakeUniqueHelper {
	struct single_type {
		using type = std::unique_ptr <T>;
	};
};

template <typename T>
struct MakeUniqueHelper <T []> {
	struct array_type {
		using type = std::unique_ptr <T []>;
	};
};

template <typename T, std::size_t size>
struct MakeUniqueHelper <T [size]> {
	struct invalid_type {};
};

template <typename T, typename ... Args>
typename MakeUniqueHelper <T>::single_type::type MakeUnique (Args && ... args) {
	return std::unique_ptr <T> (new T (std::forward <Args> (args) ...));
}

template <typename T>
typename MakeUniqueHelper <T>::array_type::type MakeUnique (std::size_t size) {
	return std::unique_ptr <T> (new RemoveExtentT <T> [size] ());
}

template <typename T, typename Args>
typename MakeUniqueHelper <T>::invalid_type MakeUnique (Args && ...) = delete;

int main () {
	std::unique_ptr <int> p1 = MakeUnique <int> (10);
	std::cout << * p1 << std::endl;
	std::unique_ptr <int[]> p2 = MakeUnique <int[]> (10);
	p2 [3] = 3;
	std::cout << p2 [3] << std::endl;
	// auto a = MakeUnique <int [2]> (2);
}
