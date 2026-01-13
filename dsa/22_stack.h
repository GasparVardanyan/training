# ifndef STACK_H_22
# define STACK_H_22

# include <algorithm>
# include <concepts>

# include "20_vector.h"

template <template <typename...> typename C, typename E>
concept StackContainer = requires (C <E> t, C <E> const tc, E e) {
	t.push_back (e);
	t.push_back (std::move (e));
	t.pop_back ();
	t.clear ();

	{ t.back () } -> std::convertible_to <E &>;
	{ tc.back () } -> std::convertible_to <const E &>;

	{ tc.size () } -> std::convertible_to <std::size_t>;
	{ tc.empty () } -> std::convertible_to <bool>;
};

template <typename T, template <typename...> typename C = vector>
requires StackContainer <C, T>
class stack
{
public:
	void push (T object) {
		m_vector.push_back (std::move (object));
	}
	// void push (const T & object) { m_vector.push_back (object); }
	// void push (T && object) { m_vector.push_back (std::move (object)); }
	void pop () {
		m_vector.pop_back ();
	}
	T & top () {
		return m_vector.back ();
	}
	const T & top () const {
		return m_vector.back ();
	}
	std::size_t size () const {
		return m_vector.size ();
	}
	bool empty () const {
		return m_vector.empty ();
	}
	void clear () {
		m_vector.clear ();
	}
private:
	C <T> m_vector;
};

# endif // STACK_H_22
