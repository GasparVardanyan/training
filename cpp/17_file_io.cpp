# include <fstream>
# include <iostream>
# include <iterator>
# include <string>
# include <vector>

struct Line {
	std::string buf;

	friend std::istream & operator>> (std::istream & is, Line & line) {
		std::getline (is, line.buf);
		return is;
	}

	friend std::ostream & operator<< (std::ostream & os, const Line & line) {
		os << line.buf;
		return os;
	}

	operator const std::string & () const {
		return buf;
	}
};

int main ()
{
	std::string str = "a b c\nd e f\ng h i";

	std::ofstream ofs (DATA_DIR "/test.txt");

	if (true == ofs.is_open ()) {
		ofs << str;
		if (true == ofs.bad ()) {
			std::cerr << "error writing file" << std::endl;
		}
	}
	else {
		std::cerr << "error opening file" << std::endl;
	}

	ofs.close ();

	std::ifstream ifs (DATA_DIR "/test.txt");

	if (true == ifs.is_open ()) {
		Line l;
		while (ifs >> l) {
			std::cout << "line: " << l << std::endl;
		}
	}
	else {
		std::cerr << "error opening file" << std::endl;
	}

	std::cout << "====================" << std::endl;

	ifs.clear ();
	ifs.seekg (0, std::ios::beg);

	std::vector <std::string> lines {
		std::istream_iterator <Line> {ifs},
		std::istream_iterator <Line> {}
	};

	for (const std::string & line : lines) {
		std::cout << "line: " << line << std::endl;
	}

	ifs.close ();
}
