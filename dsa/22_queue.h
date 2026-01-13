# ifndef QUEUE_H_22
# define QUEUE_H_22

# include <algorithm>
# include <concepts>

# include "21_list.h"

template <template <typename...> typename C, typename E>
concept QueueContainer = requires (C <E> t, C <E> const tc) {
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

template <typename T, template <typename...> typename C = list>
requires QueueContainer <C, T>
class queue
{
public:
	void push (T object) { m_list.push_back (std::move (object)); }
	void pop () { m_list.pop_front (); }

	T & front () { return m_list.front (); }
	const T & front () const { return m_list.front (); }
	T & back () { return m_list.back (); }
	const T & back () const { return m_list.back (); }

	std::size_t size () const {
		return m_list.size ();
	}
	bool empty () const {
		return m_list.empty ();
	}
	void clear () {
		m_list.clear ();
	}
private:
	C <T> m_list;
};

# endif // QUEUE_H_22
