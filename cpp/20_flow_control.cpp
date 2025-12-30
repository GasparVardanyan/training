# include <cstdio>
# include <exception>
# include <iostream>
# include <stdexcept>

struct S : public std::exception {};

bool confirm () noexcept (false) {
	char c = std::getchar ();

	bool confirmed = false;

	switch (c) {
	case 'y' :
	case 'Y' :
		confirmed = true;
		break;
	case 'n' :
	case 'N' :
		confirmed = false;
		break;
	case '-' :
		throw S {};
	default :
		throw std::runtime_error ("invalid input");
	}

	return confirmed;
}

void check_confirm () {
	try {
		bool c = confirm ();

		if (true == c) {
			std::cout << "confirmed" << std::endl;
		}
		else {
			std::cout << "not confirmed" << std::endl;
		}
	}
	catch (std::runtime_error e) {
		std::cout << "exception thrown: " << e.what () << std::endl;
	}
	catch (...) {
		std::cout << "unknown exception thrown" << std::endl;
	}
}

int main ()
{
	check_confirm ();

	if (bool b = true) {
		std::cout << "b: " << b << std::endl;
	}

	if (bool b = false; !b) {
		std::cout << "b: " << b << std::endl;
	}

a:
	goto a;
}
