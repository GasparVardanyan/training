# include <algorithm>
# include <cstdlib>
# include <ctime>
# include <iostream>

# include <iterator>
# include <sstream>
# include <string>

struct IntWrapper {
	int val;

	IntWrapper (int v) : val {v} {}; // defines the cast operator

	friend std::istream & operator>> (std::istream & is, IntWrapper & iw) {
		is >> iw.val;
		return is;
	}

	friend std::ostream & operator<< (std::ostream & os, const IntWrapper & iw) {
		os << (std::string) iw;
		return os;
	}

	operator std::string () const {
		return "INT(" + std::to_string (val) + ")";
	}
};

int main ()
{
	std::ostringstream oss;

	oss << "hello, " << "world!" << std::endl << 123;

	std::istringstream iss (oss.str ());

	std::cout << oss.str () << std::endl;

	std::copy (std::istream_iterator <std::string> {iss}, std::istream_iterator <std::string> {}, std::ostream_iterator <std::string> (std::cout, " - "));
	std::cout << std::endl;

	iss = std::istringstream ("1 2 3 4");

	std::transform (
		std::istream_iterator <int> {iss},
		std::istream_iterator <int> {},
		std::ostream_iterator <IntWrapper> (std::cout, ", "),
		[] (const int & i) -> int { return i * i; }
	);
	std::cout << std::endl;

	iss.clear ();
	iss.seekg (0, std::ios::beg);

	std::srand (std::time (NULL));
	std::generate_n (std::ostream_iterator<int> (std::cout, ", "), 10, std::rand);
	std::cout << std::endl;
}
