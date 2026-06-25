# include <barrier>
# include <chrono>
# include <condition_variable>
# include <cstddef>
# include <iostream>
# include <mutex>
# include <semaphore>
# include <syncstream>
# include <thread>
# include <vector>

namespace {

void example1 () {
	std::binary_semaphore sem1 (0), sem2 (1);

	int count = 0;

	std::jthread jt ([&sem1, &sem2, &count] () -> void {
		bool exit = false;

		while (true) {
			sem1.acquire ();

			std::cout << "count: " << count << '\n';

			if (5 == count) {
				exit = true;
			}

			sem2.release ();

			if (true == exit) {
				break;
			}
		}
	});

	for (int i = 0; i < 5; i++) {
		sem2.acquire ();
		std::this_thread::sleep_for (std::chrono::milliseconds (50));
		count++;
		sem1.release ();
	}
}

void example2 () {
	std::mutex mtx;
	std::condition_variable cond;
	bool notified = false;

	int result = 0;

	std::jthread producer ([&] () -> void {
		{
			std::scoped_lock <std::mutex> lock (mtx);

			result = 42;

			// simulate some work:
			std::this_thread::sleep_for (std::chrono::milliseconds (500));

			notified = true;
		}

		cond.notify_one ();
	});

	std::jthread consumer ([&] () -> void {
		std::unique_lock <std::mutex> lock (mtx);

		cond.wait (lock, [& notified] () -> bool { return notified; });

		std::cout << "result = " << result << '\n';
	});
}

void example3 () {
	std::condition_variable cond;
	std::mutex mtx;
	bool notified = false;
	int result = 0;

	bool finished = false;

	std::jthread producer ([&] () -> void {
		for (int i = 0; i < 5; i++) {
			{
				std::unique_lock <std::mutex> lock (mtx);
				cond.wait (lock, [&notified] () -> bool { return false == notified; });

				result += i;

				notified = true;
			}

			cond.notify_one ();
		}

		{
			std::unique_lock <std::mutex> lock (mtx);
			cond.wait (lock, [&notified] () -> bool { return false == notified; });

			finished = true;

			notified = true;
		}

		cond.notify_one ();
	});

	std::jthread consumer ([&] () -> void {
		while (true) {
			{
				std::unique_lock <std::mutex> lock (mtx);
				cond.wait (lock, [&notified] () -> bool { return notified; });
				notified = false;

				if (true == finished) {
					break;
				}
				else {
					std::cout << "Value: " << result << '\n';
				}
			}

			cond.notify_one ();
		}
	});
}

void example4 () {
	const std::size_t consumer_count = 5;
	const std::size_t product_count = 5;

	std::condition_variable cond;
	std::mutex mtx;

	int result = 0;
	bool result_ready = false;
	bool finished = false;

	std::binary_semaphore batch_end_sem (0);
	std::barrier barrier (consumer_count, [&] () -> void {
		result_ready = false;
		batch_end_sem.release ();
	});

	std::jthread producer ([&] () -> void {
		for (std::size_t i = 0; i < product_count; i++) {
			{
				std::scoped_lock <std::mutex> lock (mtx);

				result += i;
				result_ready = true;
				std::cout << "produced: " << result << '\n';
			}

			cond.notify_all ();

			batch_end_sem.acquire ();
		}

		{
			std::scoped_lock <std::mutex> lock (mtx);
			finished = true;
			result_ready = true;
			std::cout << "producer finished\n";
		}

		cond.notify_all ();
	});

	std::vector <std::jthread> consumers;
	consumers.reserve (consumer_count);

	for (std::size_t i = 0; i < consumer_count; i++) {
		consumers.emplace_back ([&, consumer_id = i] () -> void {
			while (true) {
				int _result = 0;

				{
					std::unique_lock <std::mutex> lock (mtx);
					cond.wait (lock, [& result_ready] () -> bool { return result_ready; });
					if (true == finished) {
						std::cout << "consumer " << consumer_id << " finished\n";
						break;
					}
					_result = result;
				}

				std::osyncstream (std::cout) << "consumer " << consumer_id << " got result " << _result << '\n';

				barrier.arrive_and_wait ();
			}
		});
	}
}

} // end anonymous namespace

int main () {
	example1 ();
	std::cout << "====================\n";
	example2 ();
	std::cout << "====================\n";
	example3 ();
	std::cout << "====================\n";
	example4 ();
}
