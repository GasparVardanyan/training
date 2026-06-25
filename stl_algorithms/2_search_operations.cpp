# include <algorithm>
# include <array>
# include <execution>
# include <functional>
# include <iostream>
# include <iterator>
# include <ranges>
# include <utility>

# include <boost/range/algorithm_ext/iota.hpp>

int main () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

	std::array <int, 10> arr; {
		boost::iota (arr, 0);
	}



	std::cout << std::all_of (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i < 10;
	}) << ' ';

	std::cout << std::all_of (std::execution::par, arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i < 9;
	}) << ' ';

	std::cout << std::any_of (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i == 7;
	}) << ' ';

	std::cout << std::any_of (std::execution::par_unseq, arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i == 77;
	}) << ' ';

	std::cout << std::none_of (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i == 11;
	}) << ' ';

	std::cout << std::none_of (std::execution::seq, arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i == 1;
	}) << ' ';

	std::cout << '\n';



	std::cout << std::ranges::all_of (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return 0 == i % 2;
	}, [] (int i) -> int {
		return 2 * i;
	}) << ' ';

	std::cout << std::ranges::all_of (std::as_const (arr), [] (int i) -> bool {
		return i < 10;
	}, [] (int i) -> int {
		return 2 * i;
	}) << ' ';

	std::cout << std::ranges::any_of (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i > 10;
	}, [] (int i) -> int {
		return i * 2;
	}) << ' ';

	std::cout << std::ranges::any_of (std::as_const (arr), [] (int i) -> bool {
		return i > 10;
	}, [] (int i) -> int {
		return i / 2;
	}) << ' ';

	std::cout << std::ranges::none_of (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i < 2;
	}, [] (int i) -> int {
		return (1 + i) * 2;
	}) << ' ';

	std::cout << std::ranges::none_of (std::as_const (arr), [] (int i) -> bool {
		return i > 2;
	}, [] (int i) -> int {
		return (1 + i) * 2;
	}) << ' ';

	std::cout << '\n';



	decltype (arr)::const_iterator it;

	it = std::find (arr.cbegin (), arr.cend (), 9);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find (std::execution::par, arr.cbegin (), arr.cend (), 99);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_if (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return 8 > i && 4 < i;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_if (std::execution::par, arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return 6 < i;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_if_not (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i < 4;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_if_not (std::execution::seq, arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i < 6;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	std::cout << '\n';



	it = std::ranges::find (std::as_const (arr), 36, [] (int i) -> int {
		return i * i;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::ranges::find (arr.cbegin (), arr.cend (), 64, [] (int i) -> int {
		return i * i;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::ranges::find_if (std::as_const (arr), [] (int i) -> bool {
		return i == 81;
	}, [] (int i) -> int {
		return i * i;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::ranges::find_if (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i == 49;
	}, [] (int i) -> int {
		return i * i;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::ranges::find_if_not (std::as_const (arr), [] (int i) -> bool {
		return i < 36;
	}, [] (int i) -> int {
		return i * i;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::ranges::find_if_not (arr.cbegin (), arr.cend (), [] (int i) -> bool {
		return i < 16;
	}, [] (int i) -> int {
		return i * i;
	});
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	std::cout << '\n';



	it = std::find_end (
		arr.cbegin (),
		arr.cend (),
		arr.cbegin () + 5,
		arr.cbegin () + 7
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_end (
		std::execution::par_unseq,
		arr.cbegin (),
		arr.cend (),
		arr.cbegin () + 5,
		arr.cbegin () + 7
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	decltype (arr) arr2;
	std::transform (arr.cbegin (), arr.cend (), arr2.begin (), [] (int i) -> int {
		return i * 2;
	});

	it = std::find_end (
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin () + 5,
		arr2.cbegin () + 7,
		[] (int ai, int a2i) -> bool {
			return 2 * ai == a2i;
		}
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_end (
		std::execution::seq,
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin () + 5,
		arr2.cbegin () + 7,
		[] (int ai, int a2i) -> bool {
			return 2 * ai == a2i;
		}
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	std::cout << '\n';



	std::ranges::subrange sr = std::ranges::find_end (
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin () + 5,
		arr2.cbegin () + 7,
		[] (int ai, int a2i) -> bool { return 2 * ai == a2i; },
		[] (int i) -> int { return i * i; },
		[] (int i) -> int { int o = i / 2; return 2 * o * o; }
	);
	// cppcheck-suppress mismatchingContainerExpression
	std::cout << std::distance (arr.cbegin (), sr.begin ()) << ' ';

	sr = std::ranges::find_end (
		std::as_const (arr),
		std::ranges::subrange (arr2.cbegin () + 5, arr2.cbegin () + 7),
		[] (int ai, int a2i) -> bool { return 2 * ai == a2i; },
		[] (int i) -> int { return i * i; },
		[] (int i) -> int { int o = i / 2; return 2 * o * o; }
	);
	// cppcheck-suppress mismatchingContainerExpression
	std::cout << std::distance (arr.cbegin (), sr.begin ()) << ' ';

	std::cout << '\n';


	std::array arr3 = {10, 20, 5, 3, 7};
	decltype (arr3) arr4;
	std::transform (
		arr3.cbegin (),
		arr3.cend (),
		arr4.begin (),
		[] (int i) -> int { return 2 * i; }
	);



	it = std::find_first_of (
		arr.cbegin (),
		arr.cend (),
		arr3.cbegin (),
		arr3.cend ()
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_first_of (
		std::execution::par,
		arr.cbegin (),
		arr.cend (), // cppcheck-suppress mismatchingContainerExpression
		arr3.cbegin (),
		arr3.cend ()
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_first_of (
		arr.cbegin (),
		arr.cend (),
		arr4.cbegin (),
		arr4.cend (),
		[] (int ai, int a4i) -> bool { return 2 * ai == a4i; }
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::find_first_of (
		std::execution::par, // NOTE: bug with par_unseq
		arr.cbegin (),
		arr.cend (),
		arr4.cbegin (),
		arr4.cend (),
		[] (int ai, int a4i) -> bool { return 2 * ai == a4i; }
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	std::cout << '\n';



	it = std::ranges::find_first_of (
		arr.cbegin (),
		arr.cend (),
		arr4.cbegin (),
		arr4.cend (),
		[] (int ai, int a4i) -> bool { return 2 * ai == a4i; },
		[] (int ai) -> int { return 2 * ai * ai; },
		[] (int a4i) -> int { return a4i * a4i; }
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::ranges::find_first_of (
		std::as_const (arr),
		std::as_const (arr4),
		[] (int ai, int a4i) -> bool { return 2 * ai == a4i; },
		[] (int ai) -> int { return 2 * ai * ai; },
		[] (int a4i) -> int { return a4i * a4i; }
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	std::cout << '\n';



	arr = decltype (arr) {1, 2, 3, 5, 4, 4, 5, 6, 9, 9};

	it = std::adjacent_find (arr.cbegin (), arr.cend ());
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::adjacent_find (std::execution::par, arr.cbegin (), arr.cend ());
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::adjacent_find (
		arr.cbegin (),
		arr.cend (),
		[] (int a, int b) -> bool {
			return a % 2 == b % 2; // (a - b) % 2 == 0
		}
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::adjacent_find (
		std::execution::par_unseq,
		arr.cbegin (),
		arr.cend (),
		[] (int a, int b) -> bool {
			return a % 2 == b % 2;
		}
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	std::cout << '\n';



	it = std::ranges::adjacent_find (
		std::as_const (arr),
		[] (int a, int b) -> bool { return a == b; },
		[] (int i) -> int { return i; }
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::ranges::adjacent_find (
		arr.cbegin (),
		arr.cend (),
		[] (int a, int b) -> bool { return a % 2 == b % 2; },
		[] (int i) -> int { return i; }
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	std::cout << '\n';



	std::cout << std::count (arr.cbegin (), arr.cend (), 4) << ' ';

	std::cout
		<< std::count (std::execution::par, arr.cbegin (), arr.cend (), 4)
		<< ' '
	;

	std::cout
		<< std::count_if (
			arr.cbegin (),
			arr.cend (),
			[] (int i) -> bool { return 0 == i % 2; }
		)
		<< ' '
	;

	std::cout
		<< std::count_if (
			std::execution::par,
			arr.cbegin (),
			arr.cend (),
			[] (int i) -> bool { return 0 == i % 2; }
		)
		<< ' '
	;

	std::cout << '\n';



	std::cout
		<< std::ranges::count (
			arr.cbegin (),
			arr.cend (),
			8,
			[] (int i) -> int { return 2 * i; }
		)
		<< ' '
	;

	std::cout
		<< std::ranges::count (
			std::as_const (arr),
			8,
			[] (int i) -> int { return 2 * i; }
		)
		<< ' '
	;

	std::cout
		<< std::ranges::count_if (
			arr.cbegin (),
			arr.cend (),
			[] (int i) -> bool { return 8 == i; },
			[] (int i) -> int { return 2 * i; }
		)
		<< ' '
	;

	std::cout
		<< std::ranges::count_if (
			std::as_const (arr),
			[] (int i) -> bool { return 8 == i; },
			[] (int i) -> int { return 2 * i; }
		)
		<< ' '
	;

	std::cout << '\n';



	boost::iota (arr, 0);
	boost::iota (arr2, 0);
	arr2 [5] = 50;



	auto its = std::mismatch (
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin ()
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	its = std::mismatch (
		std::execution::par,
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin ()
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	its = std::mismatch (
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		[] (int ai, int a2i) -> bool {
			return ai % 2 == a2i % 2;
		}
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	its = std::mismatch (
		std::execution::par,
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		[] (int ai, int a2i) -> bool {
			return ai % 2 == a2i % 2;
		}
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	its = std::mismatch (
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		arr2.cend ()
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	its = std::mismatch (
		std::execution::par_unseq,
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		arr2.cend ()
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	its = std::mismatch (
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		arr2.cend (),
		[] (int ai, int a2i) -> bool { return ai == a2i; }
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	its = std::mismatch (
		std::execution::par,
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		arr2.cend (),
		[] (int ai, int a2i) -> bool { return ai == a2i; }
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	std::cout << '\n';



	std::ranges::mismatch_result mr = std::ranges::mismatch (
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		arr2.cend (),
		[] (int ai, int a2i) -> bool {
			return ai == a2i;
		},
		[] (int i) -> int { return i; },
		[] (int i) -> int { return i; }
	);
	std::cout << std::distance (arr.cbegin (), mr.in1) << ' ';

	mr = std::ranges::mismatch (
		std::as_const (arr),
		std::as_const (arr2),
		[] (int ai, int a2i) -> bool {
			return ai == a2i;
		},
		[] (int i) -> int { return i; },
		[] (int i) -> int { return i; }
	);
	std::cout << std::distance (arr.cbegin (), its.first) << ' ';

	std::cout << '\n';



	bool e = std::equal (
		arr.cbegin (),
		arr.cbegin () + 4,
		arr2.cbegin ()
	);
	std::cout << e << ' ';

	e = std::equal (
		std::execution::par,
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin ()
	);
	std::cout << e << ' ';

	e = std::equal (
		arr.cbegin (),
		arr.cbegin () + 4,
		arr2.cbegin (),
		[] (int ai, int a2i) -> bool { return ai == a2i; }
	);
	std::cout << e << ' ';

	e = std::equal (
		std::execution::par,
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		[] (int ai, int a2i) -> bool { return ai == a2i; }
	);
	std::cout << e << ' ';

	e = std::equal (
		arr.cbegin (),
		arr.cbegin () + 4,
		arr2.cbegin (),
		arr2.cbegin () + 4
	);
	std::cout << e << ' ';

	e = std::equal (
		std::execution::par,
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		arr2.cend ()
	);
	std::cout << e << ' ';

	e = std::equal (
		arr.cbegin (),
		arr.cbegin () + 4,
		arr2.cbegin (),
		arr2.cbegin () + 4,
		[] (int ai, int a2i) -> bool { return ai == a2i; }
	);
	std::cout << e << ' ';

	e = std::equal (
		arr.cbegin (),
		arr.cend (),
		arr2.cbegin (),
		arr2.cend (),
		[] (int ai, int a2i) -> bool { return ai == a2i; }
	);
	std::cout << e << ' ';

	std::cout << '\n';



	e = std::ranges::equal (
		arr.cbegin (),
		arr.cbegin () + 4,
		arr2.cbegin (),
		arr2.cbegin () + 4,
		[] (int ai, int a2i) -> bool { return ai == a2i; },
		[] (int i) -> int { return i; },
		[] (int i) -> int { return i; }
	);
	std::cout << e << ' ';

	e = std::ranges::equal (
		std::as_const (arr),
		std::as_const (arr2),
		[] (int ai, int a2i) -> bool { return ai == a2i; },
		[] (int i) -> int { return i; },
		[] (int i) -> int { return i; }
	);
	std::cout << e << ' ';

	std::cout << '\n';




	it = std::search (
		arr.cbegin (),
		arr.cend (),
		arr.cbegin () + 5,
		arr.cbegin () + 7
	);
	std::cout << std::distance (arr.cbegin (), it) <<  ' ';

	it = std::search (
		std::execution::par,
		arr.cbegin (),
		arr.cend (),
		arr.cbegin () + 5,
		arr.cbegin () + 7
	);
	std::cout << std::distance (arr.cbegin (), it) <<  ' ';

	it = std::search (
		arr.cbegin (),
		arr.cend (),
		arr.cbegin () + 5,
		arr.cbegin () + 7,
		[] (int i, int j) -> bool {
			return i == j;
		}
	);
	std::cout << std::distance (arr.cbegin (), it) <<  ' ';

	it = std::search (
		arr.cbegin (),
		arr.cend (),
		arr.cbegin () + 5,
		arr.cbegin () + 7,
		[] (int i, int j) -> bool {
			return i == j;
		}
	);
	std::cout << std::distance (arr.cbegin (), it) <<  ' ';

	it = std::search (
		std::execution::par_unseq,
		arr.cbegin (),
		arr.cend (),
		arr.cbegin () + 5,
		arr.cbegin () + 7,
		[] (int i, int j) -> bool {
			return i == j;
		}
	);
	std::cout << std::distance (arr.cbegin (), it) <<  ' ';

	it = std::search (
		arr.cbegin (),
		arr.cend (),
		std::default_searcher (arr.cbegin () + 5, arr.cbegin () + 7)
	);
	std::cout << std::distance (arr.cbegin (), it) <<  ' ';

	std::cout << '\n';



	sr = std::ranges::search (
		arr.cbegin (),
		arr.cend (),
		arr.cbegin () + 5,
		arr.cbegin () + 7,
		[] (int i, int j) -> bool { return 3 * i == 2 * j; },
		[] (int i) -> int { return 2 * i; },
		[] (int i) -> int { return 3 * i; }
	);
	// cppcheck-suppress mismatchingContainerExpression
	std::cout << std::distance (arr.cbegin (), sr.begin ()) << ' ';

	sr = std::ranges::search (
		arr,
		std::ranges::subrange (arr.cbegin () + 5, arr.cbegin () + 7),
		[] (int i, int j) -> bool { return 3 * i == 2 * j; },
		[] (int i) -> int { return 2 * i; },
		[] (int i) -> int { return 3 * i; }
	);
	// cppcheck-suppress mismatchingContainerExpression
	std::cout << std::distance (arr.cbegin (), sr.begin ()) << ' ';

	std::cout << '\n';



	// std::ranges::copy (arr, std::ostream_iterator <int> (std::cout, " "));
	arr = decltype (arr) {1, 2, 3, 5, 4, 4, 5, 6, 9, 9};

	it = std::search_n (arr.cbegin (), arr.cend (), 2, 4);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::search_n (std::execution::par, arr.cbegin (), arr.cend (), 2, 4);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::search_n (
		arr.cbegin (),
		arr.cend (),
		2,
		5,
		[] (int i, int j) -> bool { return i == j - 1; }
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	it = std::search_n (
		std::execution::par_unseq,
		arr.cbegin (),
		arr.cend (),
		2,
		8,
		[] (int i, int j) -> bool { return 2 * i == j; }
	);
	std::cout << std::distance (arr.cbegin (), it) << ' ';

	std::cout << '\n';



	sr = std::ranges::search_n (
		arr.cbegin (),
		arr.cend (),
		2,
		8,
		[] (int i, int j) -> bool { return 2 * i == 3 * j; },
		[] (int i) -> int { return 3 * i; }
	);
	// cppcheck-suppress mismatchingContainerExpression
	std::cout << std::distance (arr.cbegin (), sr.begin ()) << ' ';

	sr = std::ranges::search_n (
		arr,
		2,
		12,
		[] (int i, int j) -> bool { return 3 * i == 2 * j; },
		[] (int i) -> int { return 2 * i; }
	);
	// cppcheck-suppress mismatchingContainerExpression
	std::cout << std::distance (arr.cbegin (), sr.begin ()) << ' ';

	std::cout << '\n';

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
