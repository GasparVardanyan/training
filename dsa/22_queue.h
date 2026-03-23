# ifndef QUEUE_H_22
# define QUEUE_H_22

# include <algorithm>
# include <concepts>
# include <utility>

# include "21_list.h"

template <typename C, typename E>
concept QueueContainer = requires (C t, C const tc) {
	t.push_back (E {});
	t.push_back (std::move (E {}));
	t.pop_front ();
	t.clear ();

	{ t.front () } -> std::convertible_to <E &>;
	{ t.back () } -> std::convertible_to <E &>;
	{ tc.front () } -> std::convertible_to <const E &>;
	{ tc.back () } -> std::convertible_to <const E &>;

	{ tc.size () } -> std::convertible_to <std::size_t>;
	{ tc.empty () } -> std::convertible_to <bool>;
};

template <typename T, typename C = list <T>>
requires QueueContainer <C, T>
class queue
{
public:
	using reference = C::reference;
	using const_reference = C::const_reference;
	using size_type = C::size_type;
	using value_type = C::value_type;

public:
	template <typename U>
	requires std::convertible_to <U, T>
	void push (U && object) {
		m_container.push_back (std::forward <U> (object));
	}

	void pop () { m_container.pop_front (); }

	T & front () { return m_container.front (); }
	const T & front () const { return m_container.front (); }
	T & back () { return m_container.back (); }
	const T & back () const { return m_container.back (); }

	std::size_t size () const {
		return m_container.size ();
	}
	bool empty () const {
		return m_container.empty ();
	}
	void clear () {
		m_container.clear ();
	}

	const C & container () const {
		return m_container;
	}

private:
	C m_container;
};

# endif // QUEUE_H_22
