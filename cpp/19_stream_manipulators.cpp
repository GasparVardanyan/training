# include <cmath>
# include <cstdio>
# include <iomanip>
# include <ios>
# include <iostream>
# include <limits>
# include <locale>
# include <sstream>
# include <string>

int main ()
{
	std::cout << true << std::endl;
	std::cout << std::boolalpha << true << std::endl;
	std::cout << std::noboolalpha << true << std::endl;

	std::cout << "====================" << std::endl;

	// bool b;
	// std::cin >> std::boolalpha >> b;
	// std::cout << b << std::endl;

	std::cout << std::hex;
	std::cout << 0xFF << std::endl;
	std::cout << std::showbase << 0xFF << std::endl;
	std::oct (std::cout);
	std::cout << 0xFF << std::endl;
	std::cout << std::noshowbase << 0xFF << std::endl;
	std::cout << std::dec;

	std::cout << "====================" << std::endl;

	std::stringstream ss;// ("0xFF");
	ss << "0xFF";

	int n;

	ss >> n;
	std::cout << n << std::endl;

	ss.clear ();
	ss.seekg (0, std::ios::beg);
	ss >> std::hex >> n;
	std::cout << n << std::endl;
	std::cout << std::hex << std::showbase << std::uppercase << n << std::nouppercase << std::noshowbase << std::dec << std::endl;

	std::cout << "====================" << std::endl;

	std::cout << std::setw (3) << 1 << std::setw (3) << 2 << std::setw (3) << "hello" << std::endl;

	std::cout << std::setfill ('*') << std::setw (5) << 10 << std::endl;
	std::cout << std::setfill ('-') << std::right << std::setw (5) << 20 << std::endl;
	std::cout.fill (' ');

	ss = std::stringstream {"hello"};

	std::string s;
	ss >> std::setw (3) >> s;

	std::cout << s << std::endl;

	std::cout << "====================" << std::endl;

	ss = {};
	ss.imbue (std::locale ("en_US.UTF8"));

	ss << std::fixed << std::setprecision (5);
	ss << std::setfill ('*') <<
		std::left <<
		"flt: " << std::setw (15) << -9.87654 << '\n' <<
		std::right <<
		"hex: " << std::setw (15) << 42 << '\n' <<
		std::internal <<
		"  $: " << std::setw (15) << std::showbase << std::put_money (1234, true) << '\n' <<
		"usd: " << std::setw (15) << std::showbase << std::put_money (1234, false) <<
		std::setw (0) << std::setfill (' ') << std::internal << std::endl
	;

	ss.unsetf (std::ios_base::fixed);
	ss << std::setprecision (std::cout.precision ());

	std::cout << ss.str ();

	std::cout << std::hex << std::showbase << (int) 0x1P5 << std::noshowbase << std::dec << std::endl;

	std::cout <<
		std::fixed << 0.01 << " = " <<
		std::scientific << 0.01 << " = " <<
		std::hexfloat << 0.01 << " = " <<
		std::defaultfloat << 0.01 <<
		std::endl
	;

	std::cout << std::showpos;
	std::cout << 1.0  << " = " << std::showpoint << 1.0 << " = " << std::noshowpoint << 1.0 << std::endl;
	std::cout << std::noshowpos;

	std::cout << std::setbase (8) << 342423 << std::endl;
	std::cout << std::setbase (16) << 342423 << std::endl;
	std::cout << std::setbase (10) << 342423 << std::endl;

	std::cout << "====================" << std::endl;

	long double pi = std::acos (-1);

	std::cout <<
		pi << '\n' <<
		10 * pi << '\n' <<
		std::setprecision (4) << 10 * pi << '\n' <<
		10000 * pi << '\n' <<
		std::fixed << 10000 * pi << std::defaultfloat << '\n' <<
		std::setprecision (10) << pi << '\n' <<
		std::setprecision (std::numeric_limits<decltype (pi)>::digits10) << pi << std::endl
	;

	// is.clear ();
	// is.seekg (0, std::ios::beg);
	// is.seekp (0, std::ios::beg);
	// is << "10 010 10 010 10 010";
	ss = std::stringstream {"10 010 10 010 10 010"};

	int num1, num2;
	ss >> std::oct >> num1 >> num2;
	std::cout << num1 << " - " << num2 << std::endl;
	ss >> std::dec >> num1 >> num2;
	std::cout << num1 << " - " << num2 << std::endl;

	ss >> std::resetiosflags (std::ios_base::basefield) >> num1 >> num2;
	std::cout << num1 << " - " << num2 << std::endl;

	ss.seekg (0, std::ios_base::end);
	ss.seekp (0, std::ios_base::end);
	ss << std::setiosflags (std::ios_base::hex | std::ios_base::uppercase | std::ios_base::showbase);
	ss << 42;

	std::string str;
	ss >> str;
	std::cout << str << std::endl;

	char c1, c2, c3;
	ss = std::stringstream ("a b c");
	ss >> c1 >> c2 >> c3;
	std::printf ("[%c], [%c], [%c]\n", c1, c2, c3);
	std::fflush (stdout);
	ss.clear ();
	ss.seekg (0, std::ios_base::beg);
	ss >> std::noskipws >> c1 >> c2 >> c3;
	std::printf ("[%c], [%c], [%c]\n", c1, c2, c3);
	ss >> std::skipws;

	ss = {};
	std::string s1 = "str space \"quote\"";
	std::string s2;
	ss << std::quoted (s1);


	std::cout << s1 << '\n' << ss.str () << std::endl;
	ss >> std::quoted (s2);
	std::cout << s2 << std::endl;

	ss = {};
	ss.imbue (std::locale ("en_US.utf8"));
	ss << std::showbase << std::put_money (100) << '\n';
	ss << std::showbase << std::put_money (100, true);
	std::cout << ss.str () << std::endl;

	// ss = {};
	// ss.imbue (std::locale ("hy_AM.utf8")); // probably need to edit /etc/locale.gen and run locale-gen
	// ss << std::showbase << std::put_money (100) << '\n';
	// ss << std::showbase << std::put_money (100, true);
	// std::cout << ss.str () << std::endl;
}
