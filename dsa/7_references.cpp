# include <cstddef>
# include <ctime>

# include <algorithm>
# include <iostream>
#include <stdexcept>
# include <string>
# include <vector>

class C {
public:
	C () {
		std::cout << "called " << makeLabel (this) << " C () " << std::endl;
	}

	C (int x) {
		if (x) {}
		std::cout << "called " << makeLabel (this) << " C (int) " << std::endl;
	}

	C (const C & o) {
		std::cout << "called C (const C &) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
	}

	C (C && o) {
		std::cout << "called C (C &&) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
	}

	C & operator= (const C & o) {
		std::cout << "called C & operator=(const C &) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
		return *this;
	}

	C & operator= (C && o) {
		std::cout << "called C & operator=(C &&) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
		return *this;
	}

	virtual ~C () {
		std::cout << "called ~C () [" << makeLabel (this)  << ']' << std::endl;
	}

public:
	static int maxIndex () {
		return (int) s_ptrs.size () - 1;
	}

private:
	static inline std::vector <const C *> s_ptrs;
	static std::string makeLabel (const C * c) {
		if (s_ptrs.end () == std::find(s_ptrs.begin (), s_ptrs.end (), c)) {
			s_ptrs.push_back(c);
		}

		return "ID" + std::to_string (std::find (s_ptrs.begin (), s_ptrs.end (), c) - s_ptrs.begin ());
	}
};

C x () {
	return {};
}

C randomItem1 (const std::vector <C> & list) {
	return list [std::rand () % list.size ()];
}

const C & randomItem2 (const std::vector <C> & list) {
	return list [std::rand () % list.size ()];
}

std::vector <int> partialSum (const std::vector <int> & arr) {
	std::vector <int> result (arr.size ());

	result [0] = arr [0];

	for (std::size_t i = 1; i < arr.size (); i++) {
		result [i] = result [i - 1] + arr [i];
	}

	return result;
}

void swap1 (std::vector <C> & x, std::vector <C> & y) {
	std::cout << "> std::vector <C> tmp = x; [" << C::maxIndex () << ']' << std::endl;
	std::vector <C> tmp = x;
	std::cout << "> x = y [" << C::maxIndex () << ']' << std::endl;
	x = y;
	std::cout << "> y = tmp [" << C::maxIndex () << ']' << std::endl;
	y = tmp;
}

void swap2 (std::vector <C> & x, std::vector <C> & y) {
	// std::vector <C> tmp = (std::vector <C> &&) (x);
	// x = (std::vector <C> &&) (y);
	// y = (std::vector <C> &&) (tmp);
	std::vector <C> tmp = static_cast <std::vector <C> &&> (x);
	x = static_cast <std::vector <C> &&> (y);
	y = static_cast <std::vector <C> &&> (tmp);
}

void swap3 (std::vector <C> & x, std::vector <C> & y) {
	std::vector <C> tmp = std::move (x);
	x = std::move (y);
	y = std::move (tmp);
}

int main () noexcept (false)
{
	std::srand (std::time (NULL));

	std::cout << "C c = x () [" << C::maxIndex () << ']' << std::endl;

	C c = x ();
	if (& c > (C*)100) {}

	std::cout << "std::vector <C> cvec {{}, {}, {}} [" << C::maxIndex () << ']' << std::endl;

	std::vector <C> cvec {{}, {}, {}};

	std::cout << "====================" << std::endl;

	{
		std::cout << "<<<" << std::endl;
		std::cout << "C _c = randomItem1 (cvec) [" << C::maxIndex () << ']' << std::endl;
		C _c = randomItem1 (cvec);
		if (& _c > (C*)0) {}
		std::cout << ">>>" << std::endl;
	}

	{
		std::cout << "<<<" << std::endl;
		std::cout << "C _c; _c = randomItem1 (cvec) [" << C::maxIndex () << ']' << std::endl;
		C _c;
		_c = randomItem1 (cvec);
		if (& _c > (C*)0) {}
		std::cout << ">>>" << std::endl;
	}

	std::cout << "====================" << std::endl;

	{
		std::cout << "<<<" << std::endl;
		std::cout << "C _c = randomItem2 (cvec) [" << C::maxIndex () << ']' << std::endl;
		C _c = randomItem2 (cvec);
		if (& _c > (C*)0) {}
		std::cout << ">>>" << std::endl;
	}

	{
		std::cout << "<<<" << std::endl;
		std::cout << "C _c; _c = randomItem2 (cvec) [" << C::maxIndex () << ']' << std::endl;
		C _c;
		_c = randomItem2 (cvec);
		if (& _c > (C*)0) {}
		std::cout << ">>>" << std::endl;
	}

	{
		std::cout << "<<<" << std::endl;
		std::cout << "const C & _c = randomItem2 (cvec) [" << C::maxIndex () << ']' << std::endl;
		const C & _c = randomItem2 (cvec);
		if (& _c > (C*)0) {}
		std::cout << ">>>" << std::endl;
	}

	std::cout << "====================" << std::endl;

	{
		std::cout << "<<<" << std::endl;
		std::cout << "C _c; _c = 5" << std::endl;
		C _c;
		_c = 5;
		std::cout << ">>>" << std::endl;
	}

	std::cout << "====================" << std::endl;

	{
		std::cout << "<<<" << std::endl;
		std::cout << "std::vector <C> v1 {1, 2, 3}; [" << C::maxIndex () << ']' << std::endl;
		std::vector <C> v1 {1, 2, 3};
		std::cout << "std::vector <C> v2 {1, 2, 3, 4, 5}; [" << C::maxIndex () << ']' << std::endl;
		std::vector <C> v2 {1, 2, 3, 4, 5};
		std::cout << "swap1 (v1, v2); [" << C::maxIndex() << ']' << std::endl;
		swap1 (v1, v2);
		if (v1.size () != 5 || v2.size () != 3) {
			throw std::runtime_error ("error");
		}
		std::cout << "swap2 (v1, v2); [" << C::maxIndex() << ']' << std::endl;
		swap2 (v1, v2);
		if (v1.size () != 3 || v2.size () != 5) {
			throw std::runtime_error ("error");
		}
		std::cout << "swap3 (v1, v2); [" << C::maxIndex() << ']' << std::endl;
		swap3 (v1, v2);
		if (v1.size () != 5 || v2.size () != 3) {
			throw std::runtime_error ("error");
		}
		std::cout << ">>>" << std::endl;
	}

	std::cout << "====================" << std::endl;

	{
		std::cout << "<<<" << std::endl;
		std::vector <C> x = {1, 2, 3, 4}; // This uses already used and free-ed stack, so ids repeat
		std::vector <C> y = (std::vector <C> &&) x;
		std::cout << "ys: " << y.size () << std::endl;
		std::cout << "xs: " << x.size () << std::endl;
		x.push_back(10);
		std::cout << "xs: " << x.size () << std::endl;
		std::cout << ">>>" << std::endl;
	}
}
