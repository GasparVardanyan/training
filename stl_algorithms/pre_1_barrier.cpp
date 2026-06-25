# include <algorithm>
# include <barrier>
# include <initializer_list>
# include <iostream>
# include <iterator>
# include <syncstream>
# include <thread>
# include <vector>

int main () {
	const std::initializer_list <const char *> args = {"a", "b", "c"};

	auto on_completion = [] () -> void {
		static const char * state = "ENTERED";
		std::osyncstream (std::cout) << state << '\n';
		state = "EXITED";
	};

	std::barrier barrier (std::ssize (args), on_completion);

	auto work = [&barrier] (const char * value) -> void {
		std::osyncstream (std::cout) << "entering " << value << '\n';
		barrier.arrive_and_wait ();
		std::osyncstream (std::cout) << "exiting " << value << '\n';
		barrier.arrive_and_wait ();
	};

	std::vector <std::jthread> threads;
	threads.reserve (args.size ());

	std::ranges::for_each ( args, [&threads, &work] (const char * v) -> void {
		threads.emplace_back (work, v);
	});
}
