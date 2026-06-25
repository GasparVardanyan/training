# include <functional>
# include <iostream>
# include <thread>

int main () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

	auto setTo10 = [] (int & i) -> void { i = 10; };
	auto printAddr = [] (const int & i) -> void {
		std::cout << "addr: " << & i << '\n';
	};

	int i = 2;

	std::thread t (setTo10, std::ref (i));
	t.join ();
	std::cout << i << '\n';

	t = std::thread (printAddr, std::cref (i));
	t.join ();

	std::cout << "addr: " << & i << '\n';

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
