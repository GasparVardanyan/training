# include <chrono>
# include <iostream>
# include <syncstream>
# include <thread>



int main () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

	std::jthread jt;



	{
		auto print_id = [] () -> void {
			std::cout << "thread id: " << std::this_thread::get_id () << '\n';
		};

		print_id ();

		jt = std::jthread (print_id);
	}



	{
		auto sleep1 = [] (int i, bool sleep = true) -> void {
			if (true == sleep) {
				std::this_thread::sleep_for (std::chrono::milliseconds (500));
			}
			std::osyncstream (std::cout) << "hello " << i << '\n';
		};

		jt = std::jthread (sleep1, 1);

		sleep1 (2, false);
	}



	{
		auto sleep2 = [] (int i, bool sleep = true) -> void {
			if (true == sleep) {
				std::this_thread::sleep_until (std::chrono::system_clock::now () + std::chrono::milliseconds (1000));
			}
			std::osyncstream (std::cout) << "hello " << i << '\n';
		};

		jt = std::jthread (sleep2, 1);

		sleep2 (2, false);
	}



	{
		auto yield = [] (int i, bool sleep = true) -> void {
			if (true == sleep) {
				const auto end = std::chrono::steady_clock::now () + std::chrono::milliseconds (500);

				do {
					std::this_thread::yield ();
				} while (std::chrono::steady_clock::now () < end);
			}

			std::osyncstream (std::cout) << "I: " << i << '\n';
		};

		jt = std::jthread (yield, 1);

		yield (2, false);
	}


	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
