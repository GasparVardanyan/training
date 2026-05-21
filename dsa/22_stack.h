# ifndef STACK_H_22
# define STACK_H_22

# include <algorithm>
# include <concepts>
# include <cstddef>
# include <iostream>
# include <utility>

# include "20_vector.h"

template <typename C, typename E>
concept StackContainer = requires (C t, C const tc, E e) {
	t.push_back (e);
	t.push_back (std::move (e));
	t.pop_back ();
	t.clear ();

	{ t.back () } -> std::convertible_to <E &>;
	{ tc.back () } -> std::convertible_to <const E &>;

	{ tc.size () } -> std::convertible_to <std::size_t>;
	{ tc.empty () } -> std::convertible_to <bool>;
};

template <typename T, typename C = vector <T>>
requires StackContainer <C, T>
class stack
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

	stack () noexcept (std::is_nothrow_constructible_v <C>) = default;

	template <typename U>
	requires std::is_constructible_v <T, U> && (!std::is_same_v <T, U>)
	stack (const stack <U> & other) {
		for (const U & item : other.m_container) {
			m_container.push_back (item);
		}
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

	const C & container () const {
		std::clog << "!!! stack::container is for only debugging purposes !!!" << std::endl;
		return m_container;
	}

private:
	C m_container;
};

# endif // STACK_H_22
