# include <cstddef>
# include <ctime>

# include <algorithm>
# include <iostream>
# include <string>
# include <vector>

class C {
public:
	C () {
		std::cout << "called " << makeLabel (this) << " C () " << std::endl;
	}

	C(const C & o) {
		std::cout << "called C(const C &) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
	}

	C(C && o) {
		std::cout << "called C(C &&) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
	}

	C &operator=(const C & o) {
		std::cout << "called C &operator=(const C &) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
		return *this;
	}

	C &operator=(C && o) {
		std::cout << "called C &operator=(C &&) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
		return *this;
	}

public:
	static std::size_t maxIndex () {
		return s_ptrs.size () - 1;
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

int main ()
{
	std::srand (std::time (NULL));

	std::cout << "C c = x () [" << C::maxIndex () << ']' << std::endl;

	C c = x ();
	if (& c > (C*)100) {}

	std::cout << "std::vector <C> cvec {{}, {}, {}} [" << C::maxIndex () << ']' << std::endl;

	std::vector <C> cvec {{}, {}, {}};

	std::cout << "====================" << std::endl;

	{
		std::cout << "C _c = randomItem1 (cvec) [" << C::maxIndex () << ']' << std::endl;
		C _c = randomItem1 (cvec);
		if (& _c > (C*)0) {}
	}

	{
		std::cout << "C _c; _c = randomItem1 (cvec) [" << C::maxIndex () << ']' << std::endl;
		C _c;
		_c = randomItem1 (cvec);
		if (& _c > (C*)0) {}
	}

	std::cout << "====================" << std::endl;

	{
		std::cout << "C _c = randomItem2 (cvec) [" << C::maxIndex () << ']' << std::endl;
		C _c = randomItem2 (cvec);
		if (& _c > (C*)0) {}
	}

	{
		std::cout << "C _c; _c = randomItem2 (cvec) [" << C::maxIndex () << ']' << std::endl;
		C _c;
		_c = randomItem2 (cvec);
		if (& _c > (C*)0) {}
	}

	{
		std::cout << "const C & _c = randomItem2 (cvec) [" << C::maxIndex () << ']' << std::endl;
		const C & _c = randomItem2 (cvec);
		if (& _c > (C*)0) {}
	}
}
