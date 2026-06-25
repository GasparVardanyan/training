# include <algorithm>
# include <functional>
# include <future>
# include <iostream>
# include <syncstream>
# include <thread>
# include <vector>

int main () {
	std::promise <int> prm1, prm2;

	std::vector <std::function <void ()>> funcs = {
		[&prm1, &prm2] () -> void {
			prm1.get_future ().get ();
			prm2.set_value (1);
			std::osyncstream (std::cout) << "DONE f1\n";
		},
		[&prm1, &prm2] () -> void {
			prm2.get_future ().get ();
			prm1.set_value (1);
			std::osyncstream (std::cout) << "DONE f2\n";
		},
	};

	std::vector <std::jthread> threads;
	threads.reserve (funcs.size ());

	std::ranges::for_each (funcs, [&threads] (std::function <void ()> & f) -> void {
		threads.emplace_back (f);
	});
}
