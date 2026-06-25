# include <chrono>
# include <future>
# include <iostream>
# include <latch>
# include <stdexcept>
# include <string>
# include <syncstream>
# include <thread>
# include <utility>

# include <sys/wait.h>
// NOLINTNEXTLINE(hicpp-deprecated-headers,modernize-deprecated-headers)
# include <time.h>
# include <unistd.h>

namespace {

class thread_joiner {
public:
	explicit thread_joiner (std::thread t)
		: m_thread (std::move (t)) {}

	thread_joiner (const thread_joiner &) = delete;
	thread_joiner & operator= (const thread_joiner &) = delete;

	thread_joiner (thread_joiner &&) noexcept = default;
	thread_joiner & operator= (thread_joiner &&) noexcept = default;

	~thread_joiner () {
		if (true == m_thread.joinable ()) {
			m_thread.join ();
		}
	}

private:
	std::thread m_thread;
};

} // end anonymous namespace

int main () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

	auto lambda1 = [] (int i) -> void {
		std::this_thread::sleep_for (std::chrono::milliseconds (500));
		std::cout << "hello from thread " << i << '\n';
	};

	pid_t pid = fork ();
	if (0 == pid) {
		std::thread t (lambda1, 1); // terminates the process
		return 0; // just a good practice
	}

	waitpid (pid, NULL, 0);



	{
		std::thread t (lambda1, 2);
		t.join ();
	}

	{
		thread_joiner tj (std::thread (lambda1, 3));
	}

	{
		std::jthread jt (lambda1, 4);
	}

	std::cout << "====================\n";



	auto lambda2 = [] (int i) -> void {
		std::this_thread::sleep_for (std::chrono::milliseconds (500));
		// cppcheck-suppress throwInEntryPoint
		throw std::runtime_error ("hello from thread " + std::to_string (i));
	};

	pid = fork ();

	if (0 == pid) {
		try {
			std::thread t (lambda2, 1);
			t.join ();
		} catch (const std::runtime_error & e) {
			std::cout << "error: " << e.what () << '\n';
		}

		return 0; // just a good practice
	}

	waitpid (pid, NULL, 0);

	try {
		auto f = std::async (std::launch::async, lambda2, 2);
		f.get ();
	} catch (const std::runtime_error & e) {
		std::cout << "error: " << e.what () << '\n';
	}

	std::cout << "====================\n";



	std::latch latch (5);

	for (int i = 0; i < 5; i++) {
		std::thread ([& latch, i = i] () -> void {
			std::osyncstream (std::cout) << "HELLO " << i << "!!!\n";
			latch.count_down ();
		}).detach ();
	}

	latch.wait ();

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
