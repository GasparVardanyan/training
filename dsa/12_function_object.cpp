# include <cctype>
# include <cstddef>
# include <functional>
# include <iostream>
# include <vector>

class CaseInsensitiveCompare {
public:
	bool operator () (const std::string & a, const std::string & b) {
		const char * a_ptr = a.c_str ();
		const char * b_ptr = b.c_str ();

		while (* a_ptr && * b_ptr) {
			int d = std::tolower (* a_ptr) - std::tolower (* b_ptr);

			if (d < 0) {
				return true;
			}
			else if (d > 0) {
				return false;
			}
			else {
				a_ptr++;
				b_ptr++;
			}
		}

		if (* b_ptr) {
			return true;
		}
		else {
			return false;
		}
	}
};

template <typename Object, typename Comparator>
const Object & findMax (const std::vector<Object> & arr, Comparator isLessThan) {
	std::size_t maxIndex = 0;

	for (std::size_t i = 1; i < arr.size (); i++) {
		if (isLessThan (arr [maxIndex], arr [i])) {
			maxIndex = i;
		}
	}

	return arr [maxIndex];
}

template <typename Object>
const Object & findMax (const std::vector<Object> & arr) {
	return findMax (arr, std::less<Object> {});
}

int main ()
{
	std::vector<std::string> arr = {
		"ZEBRAXYY", "alligator", "crocodile"
	};

	std::cout << findMax (arr, CaseInsensitiveCompare {}) << std::endl;
	std::cout << findMax (arr) << std::endl;
}
