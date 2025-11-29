# include <cstdint>
# include <type_traits>

template <typename T>
auto popcount8R (const T & val) -> int
{
	static_assert (std::is_integral <T> :: value && ! std::is_signed <T> :: value);

	if (val == 0)
		return 0;

	const unsigned char m1 = 0b01010101;
	const unsigned char m2 = 0b00110011;
	const unsigned char m4 = 0b00001111;

	const unsigned char r = val & 0b11111111;
	const unsigned char a = (r & m1) + ((r >> 1) & m1);
	const unsigned char b = (a & m2) + ((a >> 2) & m2);
	const unsigned char c = (b & m4) + ((b >> 4) & m4);

	return c + popcount8R ((T) (val >> 8));
}

auto popcount64 (const uint64_t & val) -> int
{
	const uint64_t m1 = 0b0101010101010101010101010101010101010101010101010101010101010101;
	const uint64_t m2 = 0b0011001100110011001100110011001100110011001100110011001100110011;
	const uint64_t m4 = 0b0000111100001111000011110000111100001111000011110000111100001111;

	const uint64_t a = val - ((val >> 1) & m1);			// bit a, bit b, (a << 1) + b - a = 2a + b - a = a + b
	const uint64_t b = (a & m2) + ((a >> 2) & m2);		// again apply the mask first, because
														//                          the sum of two 2 bit integers can take more than 2 bits
														//                          (max: 3)
	uint64_t c = (b + (b >> 4)) & m4;					// take the advantage, that the sum of two 3 bit integers can't take
														//                     more than 4 bits (max: 4)

# ifndef __FAST_MUL_OR_SOMETHING_LIKE_THIS
	c += c >> 8;			// take the advantage, that the sum of two 4 bit integers can't take more than 8 bits (max 5)
	c += c >> 16;			// take the advantage, that the sum of two 5 bit integers can't take more than 8 bits (max 6)
	c += c >> 32;			// take the advantage, that the sum of two 6 bit integers can't take more than 8 bits (max 7)
	c &= (1ull << 7) - 1;	// take the rightmost 7 bits (the sum of all 64 bits)
# else
	const uint64_t h01 = 0b0000000100000001000000010000000100000001000000010000000100000001;
	c = (c * h01) >> (7 * 8);		// combine the sum in the leftmost 8 bits and fix the position:
	/*
	   c * h01 = c * (1 + (1 << 8) + (1 << 16) + (1 << 24) + (1 << 32) + (1 << 40) + (1 << 48) + (1 << 56)) =
			   = c + (c << 8) + (c << 16) + (c << 32) + (c << 40) + (c << 48) + (c << 56)
	*/
								// NOTE: overflow happens there, I'm not sure how the multiplication is implemented,
								//       so this can give a wrong value.
# endif

	return c;
}

int main ()
{

}
