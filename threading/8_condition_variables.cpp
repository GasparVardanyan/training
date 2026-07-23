# include <barrier>
# include <chrono>
# include <condition_variable>
# include <cstddef>
# include <iostream>
# include <mutex>
# include <queue>
# include <semaphore>
# include <stack>
# include <syncstream>
# include <thread>
# include <vector>

namespace {
// NOLINTBEGIN(llvm-prefer-static-over-anonymous-namespace)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

void example1 () {
	std::binary_semaphore sem1 (0), sem2 (1);

	int count = 0;

	const std::jthread jt ([&sem1, &sem2, &count] () -> void {
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

	const std::jthread producer ([&] () -> void {
		{
			const std::scoped_lock <std::mutex> lock (mtx);

			result = 42;

			// simulate some work:
			std::this_thread::sleep_for (std::chrono::milliseconds (500));

			notified = true;
		}

		cond.notify_one ();
	});

	const std::jthread consumer ([&] () -> void {
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

	const std::jthread producer ([&] () -> void {
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

	const std::jthread consumer ([&] () -> void {
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

	const std::jthread producer ([&] () -> void {
		for (std::size_t i = 0; i < product_count; i++) {
			{
				const std::scoped_lock <std::mutex> lock (mtx);

				result += static_cast <int> (i);
				result_ready = true;
				std::cout << "produced: " << result << '\n';
			}

			cond.notify_all ();

			batch_end_sem.acquire ();
		}

		{
			const std::scoped_lock <std::mutex> lock (mtx);
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

					// reading result under the lock
					_result = result;
				}

				// processing the result in parallel
				std::osyncstream (std::cout) << "consumer " << consumer_id << " got result " << _result << '\n';

				barrier.arrive_and_wait ();
			}
		});
	}
}

void example5 () {
	const std::size_t numProducers = 5;
	const std::size_t numProductsPerProducer = 5;

	std::mutex mtx;
	std::condition_variable cond;
	std::queue <std::size_t> resultQueue;

	bool finished = false;

	std::barrier finishBarrier (numProducers, [&] () -> void {
		{
			std::scoped_lock <std::mutex> lock (mtx);
			finished = true;
		}
		cond.notify_one ();
	});

	std::jthread consumer ([&] () -> void {
		while (true) {
			std::stack <std::size_t> procStack;
			bool _finished = false;

			{
				std::unique_lock <std::mutex> lock (mtx);
				cond.wait (lock, [&finished, &resultQueue] () -> bool {
					return finished || false == resultQueue.empty ();
				});

				while (false == resultQueue.empty ()) {
					procStack.push (resultQueue.front ());
					resultQueue.pop ();
				}

				_finished = finished;

				std::cout << "==========\n";
			}

			while (false == procStack.empty ()) {
				std::size_t r = procStack.top ();
				procStack.pop ();

				std::osyncstream (std::cout) << "GOT: " << r << '\n';
			}

			if (true == _finished) {
				break;
			}
		}
	});

	std::vector <std::jthread> producers;
	producers.reserve (numProducers);

	for (std::size_t i = 0; i < numProducers; i++) {
		producers.emplace_back ([&, result = 0] () mutable -> void {
			for (std::size_t j = 0; j < numProductsPerProducer; j++) {
				result += static_cast <int> (j);

				{
					std::scoped_lock <std::mutex> lock (mtx);
					resultQueue.push (result);
				}

				cond.notify_one ();
			}

			finishBarrier.arrive_and_drop ();
		});
	}
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
// NOLINTEND(llvm-prefer-static-over-anonymous-namespace)
} // end anonymous namespace

int main () {
	example1 ();
	std::cout << "====================\n";
	example2 ();
	std::cout << "====================\n";
	example3 ();
	std::cout << "====================\n";
	example4 ();
	std::cout << "====================\n";
	example5 ();
}
