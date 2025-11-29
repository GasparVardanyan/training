# include <bitset>
# include <cstddef>
# include <exception>
# include <iostream>
# include <stdexcept>
# include <type_traits>

template <typename T>
void assert_unsigned ()
{
	static_assert (
		std::is_integral<T>::value &&
			std::is_unsigned<T>::value,
		"T isn't unsigned"
	);
}

template <typename T>
T unsetTheRightmostSetBit (const T & n)
{
	assert_unsigned <T> ();

	return n & (n - 1);
}

template <typename T>
T setAllBits (const T & n)
{
	assert_unsigned <T> ();

	return -1;
	return n | ~n;
}

template <typename T>
T toggleNthBit (const T & n, std::size_t index) noexcept (false)
{
	assert_unsigned<T>();
	if (index >= 8 * sizeof (T)) {
		throw std::invalid_argument ("index is invalid");
	}

	return n ^ ((T) 1 << index);
}

template <typename T>
int getNthBit (const T & n, std::size_t index) noexcept (false)
{
	assert_unsigned<T>();
	if (index >= 8 * sizeof (T)) {
		throw std::invalid_argument ("index is out of range");
	}

	return (n & (T) 1 << index) >> index & 1;
}

template <typename T>
bool isPowerOf2 (const T & val)
{
	assert_unsigned<T>();

	if (0 == (val & (val - 1))) {
		if (0 != val) {
			return true;
		}
	}

	return false;
}

template <typename T>
void setNthBit (T & val, std::size_t index) noexcept (false)
{
	assert_unsigned <T> ();

	if (8 * sizeof (T) <= index) {
		throw std::invalid_argument ("index is out of range");
	}

	val |= ((T) 1 << index);
}

template <typename T>
void clearNthBit (T & val, std::size_t index) noexcept (false)
{
	// T v = ~val;
	// setNthBit (v, index);
	// val = ~v;

	assert_unsigned <T> ();

	if (8 * sizeof (T) <= index) {
		throw std::invalid_argument ("index is out of range");
	}

	val &= ~((T) 1 << index);
}

template <typename T>
void setNthBit (T & val, std::size_t index, bool state) noexcept (false)
{
	assert_unsigned <T> ();

	if (8 * sizeof (T) <= index) {
		throw std::invalid_argument ("index is out of range");
	}

	if (state || val){}

	static_assert (1, "do this later");
}

int main () noexcept (false)
{
	unsigned int i = 0b1010001100;
	const unsigned int _i = i;
	std::cout << std::bitset <8 * sizeof (i)> (i) << std::endl;
	i = unsetTheRightmostSetBit (i);
	std::cout << std::bitset <8 * sizeof (i)> (i) << std::endl;

	i = setAllBits (i);
	std::cout << std::bitset <8 * sizeof (i)> (i) << std::endl;

	for (std::size_t j = 0; j < 8 * sizeof (i); j++) {
		if (1 != ((i & (decltype (i)) 1 << j) >> j)) {
			throw std::runtime_error ("Something is wrong !!!");
		}
	}

	i = _i;
	std::cout << std::bitset <8 * sizeof (i)> (i) << std::endl;

	for (std::size_t j = 0; j < 8 * sizeof (i); j++) {
		i = toggleNthBit (_i, j);

		if ((i ^ _i) != (decltype (i)) 1u << j) {
			throw std::runtime_error ("Something is wrong !!!");
		}
	}

	i = _i;
	decltype (i) j = 0;

	for (std::size_t index = 0; index < 8 * sizeof (i); index++) {
		j |= getNthBit (i, index) << index;
	}

	if (j != i) {
		throw std::runtime_error ("Something is wrong !!!");
	}
}
