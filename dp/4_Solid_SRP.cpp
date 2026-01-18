# include <algorithm>
# include <fstream>
# include <iostream>
# include <iterator>
# include <ostream>
# include <ranges>
# include <stdexcept>
# include <string>
# include <vector>

# include <boost/lexical_cast.hpp>

struct Journal {
private:
	std::vector <std::string> _entries; // Properties
	std::string _title;

public:
	decltype (_entries) & entries () { return _entries; }
	const decltype (_entries) & entries () const { return _entries; }


	decltype (_title) & title () { return _title; }
	const decltype (_title) & title () const { return _title; }
	void title (const decltype (_title) & title) {
		_title = title;
	}

	explicit Journal (const std::string & title) {
		this->title (title);
	}

	void add (const std::string & entry) {
		static int count = 1;
		entries ().push_back (boost::lexical_cast <std::string> (count++) + ": " + entry);
	}

	void save (const std::string & path) {
		throw std::runtime_error ("Journal::save violates [S]OLID");
	}
};

struct PersistenceManager {
	template <typename Persistable>
	requires requires (const Persistable & j) {
		{ j.entries () } -> std::ranges::input_range;
		requires requires (std::ranges::range_reference_t<decltype (j.entries ())> e, std::ostream & os) {
			os << e;
		};
	}
	static void save (const Persistable & j, const std::string & path) {
		std::ofstream os (path);

		std::copy (
			j.entries ().begin (),
			j.entries ().end (),
			std::ostream_iterator <std::string> (os, "\n")
		);

		os.close ();
	}
};

int main () {
	Journal j {"My Journal"};
	j.add ("Hello");
	j.add ("World");
	PersistenceManager::save (j, DATA_DIR "/j.txt");
}
