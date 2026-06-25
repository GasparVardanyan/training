# include <chrono>
# include <future>
# include <iostream>
# include <syncstream>
# include <thread>

int main () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
	auto square = [] (int i) -> int { return i * i; };

	auto f = std::async (std::launch::async, square, 6);
	std::cout << "sqaure is currently running\n";
	std::cout << f.get () << '\n';

	auto delayedPrint = [] (int i) -> void {
		std::this_thread::sleep_for (std::chrono::milliseconds (2'000));
		std::osyncstream (std::cout) << "delayed print " << i << '\n';
	};

	auto res = std::async (std::launch::async, delayedPrint, 1);
	std::osyncstream (std::cout) << "hello 1\n";
	res.wait ();

	std::async (std::launch::async, delayedPrint, 2);
	std::osyncstream (std::cout) << "hello 2\n";

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
