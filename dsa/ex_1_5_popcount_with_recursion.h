# pragma once

# include <stdexcept>
# include <type_traits>

template <typename T>
std::size_t popcount (T v) noexcept (false)
{
	if (false == std::is_unsigned<T>::value) {
		throw std::invalid_argument ("T must be an unsigned integer");
	}

	if (0 == v) {
		return 0;
	}

	return (v & 1) + popcount (v >> 1);
}
