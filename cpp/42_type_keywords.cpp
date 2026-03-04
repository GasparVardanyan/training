# include <cstdint>
# include <iostream>
# include <cstdint>

void f (char c) { std::cout << "char: " << c << std::endl; }
void f (signed char c) { std::cout << "signed char: " << c << std::endl; }
void f (unsigned char c) { std::cout << "unsigned char: " << c << std::endl; }

void g (int i) { std::cout << i << std::endl; }
// void g (signed int i) { std::cout << i << std::endl; } // error
void g (unsigned int i) { std::cout << i << std::endl; }

void h (signed char c) { std::cout << "signed char: " << c << std::endl; }
void h (unsigned char c) { std::cout << "unsigned char: " << c << std::endl; }

int main () {
	// h ('c'); // ambiguous
	h ((signed char) 'c');
	h ((unsigned char) 'c');
	f ('c');
	std::cout << (char) 'c' << std::endl;
	std::cout << (signed char) 'c' << std::endl;
	std::cout << (unsigned char) 'c' << std::endl;

	static_assert (false == std::is_same_v <char16_t, std::uint_least16_t>, "ERR");
	static_assert (
			   sizeof (char16_t) == sizeof (std::uint_least16_t)
			&& alignof (char16_t) == alignof (std::uint_least16_t)
		, "ERR"
	);
	static_assert (
			   sizeof (char32_t) == sizeof (std::uint_least32_t)
			&& alignof (char32_t) == alignof (std::uint_least32_t)
		, "ERR"
	);
	// see 42_type_keywords_c.c
}
