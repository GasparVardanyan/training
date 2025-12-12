# include <iostream>
# include <string>
# include <vector>

void print_count (const std::vector <int> & x) {
	std::cout << "print_count1: " << x.size () << std::endl;
}

void print_count (std::vector <int> && x) {
	std::cout << "print_count2: " << x.size () << std::endl;
}

int main ()
{
	std::string str = "hell";
	std::string & rstr = str;
	rstr += 'o';

	std::cout << "peq: " << (& str == & rstr) << std::endl;

	// std::string & bad1 = "hello";
	// std::string & bad2 = str + "";
	// std::string & sub = str.substr (0, 4);

	std::string && bad1 = "hello";
	std::string && bad2 = str + "";
	std::string && sub = str.substr (0, 4);
	// std::string && rvref = str;
	// int && y = x;


	std::cout << bad1 << std::endl;
	std::cout << bad2 << std::endl;
	std::cout << sub << std::endl;

	std::vector <int> x = {1, 2, 3};

	print_count (x);
	print_count ({1, 2, 3});
}
