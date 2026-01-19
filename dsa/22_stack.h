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
	using reference = C <T>::reference;
	using const_reference = C <T>::const_reference;
	using size_type = C <T>::size_type;
	using value_type = C <T>::value_type;

public:
	template <typename U>
	requires std::convertible_to <U, T>
	void push (U && object) {
		m_container.push_back (std::forward <U> (object));
	}

	void pop () {
		m_container.pop_back ();
	}
	T & top () {
		return m_container.back ();
	}
	const T & top () const {
		return m_container.back ();
	}
	std::size_t size () const {
		return m_container.size ();
	}
	bool empty () const {
		return m_container.empty ();
	}
	void clear () {
		m_container.clear ();
	}
private:
	C <T> m_container;
};

# endif // STACK_H_22
