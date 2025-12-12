# include <iostream>
# include <stdexcept>
# include <type_traits>

template <typename T>
void printOut (T n);

int main ()
{
	std::cout << "Hello, World!" << std::endl;

	printOut(1234567890u);
}

template <typename T>
void _printOut (T n);

template <typename T>
void printOut (T n)
{
	static_assert (std::is_unsigned<T>::value, "n must be an unsgined integer");

	_printOut (n);

	std::cout << std::endl;
}

template <typename T>
void _printOut (T n)
{
	if (n > 9) {
		_printOut (n / 10);
	}
	std::cout << (n - n / 10 * 10);
}

