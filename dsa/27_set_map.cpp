# include <algorithm>
# include <cstddef>
# include <fstream>
# include <iostream>
# include <iterator>
# include <numeric>
# include <string>
# include <utility>

# include "20_vector.h"
# include "26_map.h"
# include "26_set.h"

int main () { // NOLINT (bugprone-exception-escape)
	const std::size_t word_letter_count = 4;
	const std::size_t min_similiar_count = 16;

	vector <std::string> wordlist; {
		std::ifstream ifs (DATA_DIR "/dict.txt", std::ios_base::in);
		// cppcheck-suppress throwInEntryPoint
		wordlist.reserve (3'000); // NOLINT (cppcoreguidelines-avoid-magic-numbers)

		std::for_each (
			std::istream_iterator <std::string> (ifs),
			std::istream_iterator <std::string> (),
			[&wordlist] (const std::string & st) -> void {
				if (st.size () == word_letter_count) {
					wordlist.push_back (st);
				}
			}
		);
		wordlist.shrink_to_fit ();

		ifs.close ();
	}

	std::cout << wordlist.size () << '\n';

	map <std::string, vector <std::pair <std::string, std::size_t>>> info;

	map <std::string, map <std::size_t, set <char>>> lt; {
		for (const std::string & s : wordlist) {
			for (std::size_t i = 0; i < word_letter_count; i++) {
				const std::string sub = s.substr (0, i) + s.substr (i + 1, word_letter_count - 1 - i);
				const char l = s [i];

				lt [sub] [i].insert (l);
			}
		}
	}

	vector <vector <std::string>> result; {
		for (const auto & [sub, matches] : lt) {
			const std::size_t count = std::accumulate ( // NOLINT (boost-use-ranges)
				matches.cbegin (),
				matches.cend (),
				0,
				[] (std::size_t s, const map <std::size_t, set <char>>::node_data & m) -> std::size_t {
					return m.value.size () + s;
				}
			);

			vector <std::string> cluster;

			if (min_similiar_count <= count) {
				for (const auto & [index, chars] : matches) {
					for (const char c : chars) {
						const std::string word = sub.substr (0, index)
							+ c
							+ sub.substr (index, word_letter_count - 1 - index)
						;

						cluster.push_back (word);
					}
				}
			}

			if (false == cluster.empty ()) {
				result.push_back (std::move (cluster));
			}
		}
	}

	std::cout << result.size () << '\n';

	for (const vector <std::string> & cluster : result) {
		std::cout << cluster.size () << '\n';
		std::copy ( // NOLINT (boost-use-ranges, modernize-use-ranges, llvm-use-ranges)
			cluster.cbegin (),
			cluster.cend (),
			std::ostream_iterator <std::string> (std::cout, ", ")
		);
		std::cout << '\n';
	}
}
