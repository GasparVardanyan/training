# include <iostream>
# include <mutex>
# include <thread>
# include <vector>

int main () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
	int x = 0;
	std::mutex m;

	std::vector <std::thread> thrds;
	thrds.reserve (10);

	for (int i = 0; i < 10; i++) {
		thrds.emplace_back ([&x, &m] () -> void {
			std::scoped_lock lock (m);
			x++;
		});
	}

	for (std::thread & t : thrds) {
		t.join ();
	}

	std::cout << "x: " << x << '\n';

	thrds.clear ();

	for (int i = 0; i < 10; i++) {
		thrds.emplace_back ([& x, & m] () -> void {
			std::unique_lock ulock (m, std::defer_lock);

			ulock.lock ();
			x++;
			ulock.unlock ();
		});
	}

	for (std::thread & t : thrds) {
		t.join ();
	}

	std::cout << "x: " << x << '\n';

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
