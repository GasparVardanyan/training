# include <algorithm>
# include <array>
# include <concepts>
# include <iostream>
# include <iterator>
# include <numeric>
# include <utility>
# include <vector>

namespace {
// NOLINTBEGIN(llvm-prefer-static-over-anonymous-namespace)

namespace impl {
template <
	std::input_iterator InputIt,
	std::invocable <decltype (* std::declval <InputIt> ())> UnaryFunc
>
constexpr UnaryFunc for_each (InputIt first, InputIt last, UnaryFunc f) {
	for (; first != last; ++first) {
		f (* first);
	}

	return f;
}

void run () {
	std::array <int, 10> arr; {
		std::iota (arr.begin (), arr.end (), 0);
	}

	auto func = [sum = 0] (int i, bool end = false) mutable -> void {
		if (true == end) {
			std::cout << "(sum: " << sum << ")\n";
			sum = 0;
		}
		else {
			sum += i;
			std::cout << i << ' ';
		}
	};

	// NOTE: std::for_each must not be visible via ADL
	impl::for_each (arr.cbegin (), arr.cend (), func) (0, true);
	std::for_each (arr.cbegin (), arr.cend (), func) (0, true);
}
}

namespace example {
void run () {
	// std::vector<int>::const_pointer p;
}
} // end namespace example

// NOLINTEND(llvm-prefer-static-over-anonymous-namespace)
} // end anonymous namespace

int main () {
	impl::run ();
	example::run ();
}
