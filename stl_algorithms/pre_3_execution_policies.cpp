# include <algorithm>
# include <array>
# include <chrono>
# include <execution>
# include <iostream>
# include <iterator>
# include <limits>
# include <mutex>
# include <syncstream>
# include <thread>
# include <vector>

# include <boost/range/algorithm_ext/iota.hpp>

int main () {
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	std::array <int, 100> arr {0};
	// std::iota (arr.begin (), arr.end (), 1);
	boost::iota (arr, 1);
	std::vector <int> v;

	std::mutex mtx;
	int counter = 1;

	auto f = [&mtx, &v, &counter] (int x) -> void {
		if (false == mtx.try_lock ()) {
			std::osyncstream (std::cout) << "locked " << counter++ << '\n';
		}
		else {
			mtx.unlock ();
		}

		const std::scoped_lock sl (mtx);
		v.push_back (x);
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
		std::this_thread::sleep_for (std::chrono::milliseconds (5));
	};

	auto check_sorted = [& v] {
		int min = std::numeric_limits <int>::min ();
		bool sorted = true;
		for (const int i : v) {
			if (i <= min) {
				sorted = false;
				break;
			}
			min = i;
		}

		if (true == sorted) {
			// didn't had to be sorted with std::execution::seq, but happens to
			// always be sorted with my env setup.
			std::cout << "SORTED\n";
		}
		else {
			std::cout << "NOT SORTED\n";
		}
	};

	v.clear ();
	std::for_each (std::execution::par, std::cbegin (arr), std::cend (arr), f);
	check_sorted ();

	v.clear ();
	std::for_each (
		std::execution::seq,	// NOTE: unlike the previous overload, determined evaluation order isn't
									// guaranteed:
									// The execution policy type used as a unique type to disambiguate
									// parallel algorithm overloading and require that a parallel algorithm's
									// execution may not be parallelized. The invocations of element access
									// functions in parallel algorithms invoked with this policy (usually
									// specified as std::execution::seq) are indeterminately sequenced in
									// the calling thread.
		std::cbegin (arr),
		std::cend (arr),
		f
	);
	check_sorted ();
}
