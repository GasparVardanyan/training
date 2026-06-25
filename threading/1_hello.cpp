# include <iostream>
# include <syncstream>
# include <thread>

namespace {

void foo (int x) { // NOLINT (llvm-prefer-static-over-anonymous-namespace)
	std::osyncstream (std::cout) << x << '\n';
}

class Bar {
public:
	void foo (int x) {
		std::osyncstream (std::cout) << x << '\n';
	}

	void operator() (int x, int y) {
		std::osyncstream (std::cout) << x << " - " << y << '\n';
	}
};

} // end anonymous namespace

int main () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
	std::thread thread (foo, 10);
	thread.join ();

	void (Bar::* bmp) (int) = & Bar::foo;
	Bar bar;
	Bar * bp = & bar;
	(bar.*bmp) (100);
	(bp->*bmp) (1000);

	thread = std::thread (& Bar::foo, & bar, 10);
	thread.join ();

	thread = std::thread (bar, 10, 20);
	thread.join ();

	auto l = [] (int x) -> void {
		std::osyncstream (std::cout) << x << '\n';
	};

	thread = std::thread (l, 20);
	thread.join ();
	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
