# include <cstdlib>
# include <ctime>
# include <fstream>
# include <iostream>
# include <iterator>
# include <locale>
# include <ostream>
# include <string>
# include <vector>

int main ()
{
	std::srand (std::time (NULL));

	std::string file = DATA_DIR "/test.txt";

	std::ofstream ofs (file);

	if (true == ofs.is_open ()) {
		std::string str {"hello"};
		ofs.write (str.c_str (), str.length ());

		ofs << " " << std::to_string (std::rand ());

		if (true == ofs.bad ()) {
			std::cerr << "failed to write to the file" << std::endl;
		}
		else {
			std::clog << "write successful" << std::endl;
		}
	}
	else {
		std::cerr << "failed to open the file" << std::endl;
	}

	ofs.close ();

	std::ifstream ifs;
	ifs.open (file);

	if (true == ifs.is_open ()) {
		// std::string whole_file;
		// ifs.seekg (0, std::ios::end);
		// whole_file.reserve (ifs.tellg ());
		// ifs.seekg (0, std::ios::beg);
		// whole_file.assign (
		// 	std::istreambuf_iterator <char> (ifs),
		// 	std::istreambuf_iterator <char> ()
		// );

		// std::string whole_file {
		// 	std::istreambuf_iterator <char> {ifs},
		// 	std::istreambuf_iterator <char> {}
		// };

		ifs.seekg (0, std::ios::end);
		std::size_t size = ifs.tellg ();
		ifs.seekg (0, std::ios::beg);

		std::string whole_file (size, ' ');

		ifs.read (& whole_file [0], size);

		std::cout << "data: " << whole_file << std::endl;
	}

	ifs.close ();

	double val = 1234.678;

	std::ofstream ofs1;
	ofs.imbue (std::locale (""));
	ofs.open (file, std::ios::out | std::ios::trunc);
	ofs << val << std::endl;
	ofs.close ();

	ofs.imbue (std::locale ("en_US.UTF-8"));
	ofs.open (file, std::ios::out | std::ios::app);
	ofs << val << std::endl;
	ofs.close ();

	ofs.imbue (std::locale ("C"));
	ofs.open (file, std::ios::out | std::ios::app);
	ofs << val << std::endl;
	ofs.close ();

	ofs.imbue (std::locale::classic ());
	ofs.open (file, std::ios::out | std::ios::app);
	ofs << val << std::endl;
	ofs.close ();

	ifs.open (file);
	// ifs.seekg (0, std::ios::end);
	// std::size_t size = ifs.tellg ();
	// ifs.seekg (0, std::ios::beg);
	// std::string str (size, ' ');
	// ifs.read (& str [0], size);
	// ifs.close ();
	// std::cout << str << std::endl;

	std::string str;
	while (ifs >> str) {
		std::cout << str << std::endl;
	}

	ifs.close ();

	std::cout.flush ();
	std::cout << std::flush;

	ifs.open (file);
	std::vector <std::string> data {
		std::istream_iterator<std::string> {ifs},
		std::istream_iterator<std::string> {}
	};

	for (decltype (data)::iterator it = std::begin (data); it != data.end (); std::advance (it, 1)) {
		std::cout << * it << std::endl;
	}

}
