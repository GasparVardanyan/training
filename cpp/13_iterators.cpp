# include <cstddef>
# include <functional>
# include <iostream>
# include <iterator>
# include <optional>

template <class T>
struct generator_iterator {
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T;
	using iterator_category = std::input_iterator_tag;

	std::optional <T> state;
	std::function <std::optional <T> ()> operation;

	T operator* () const {
		return * state;
	}

	generator_iterator & operator++ () {
		state = operation ();
		return * this;
	}

	generator_iterator operator++ (int) {
		generator_iterator <T> r = * this;
		++(* this);
		return r;
	}

	bool operator== (const generator_iterator & rhs) const {
		if (false == this->state.has_value () && false == rhs.state.has_value ()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator!= (const generator_iterator & rhs) const {
		return false == (* this == rhs);
	}

	generator_iterator () = default;
	generator_iterator (const generator_iterator &) = default;
	generator_iterator (generator_iterator &&) = default;
	generator_iterator & operator= (const generator_iterator &) = default;
	generator_iterator & operator= (generator_iterator &&) = default;

	generator_iterator (std::function<std::optional<T>()> f)
		: operation(std::move(f))
	{
		if (operation) {
			state = operation ();
		}
	}
};

template <class It>
struct range_it {
	It b, e;
	It begin () const { return b; }
	It end () const { return e; }
};

template <class It>
range_it <It> range (It b, It e) {
	return {std::move (b), std::move (e)};
}

template <class It>
range_it <It> range (It b) {
	return range (b, {});
}

template <class F>
auto generator (F f) {
	using V = std::decay_t <decltype (* f ())>;
	return range (
		generator_iterator <V> (std::move (f))
	);
}



template <typename T>
struct simple_it {
	std::function<std::optional<T> ()> operation;
	std::optional<T> state;

	simple_it & next () {
		state = operation ();
		return * this;
	}

	bool valid () {
		return true == state.has_value ();
	}

	T value () {
		return state.value ();
	}

	simple_it (decltype(operation) op) {
		operation = op;
		state = operation ();
	}
};

simple_it <int> myIt (int to) {
	return simple_it <int> (
		[i = 0, to = to] () mutable -> std::optional<int> {
			i++;
			if (i <= to) {
				return i;
			}
			else {
				return {};
			}
		}
	);
}



int main ()
{
	auto from_1_to_10 = [i = 0] () mutable -> std::optional <int> {
		i++;
		if (i > 10) {
			return {};
		}
		else {
			return i;
		}
	};

	for (int i : generator (from_1_to_10)) {
		std::cout << i << std::endl;
	}

	for (auto it = myIt (10); it.valid (); it.next ()) {
		std::cout << it.value () << std::endl;
	}
}
