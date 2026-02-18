# ifndef VECTOR_H_20
# define VECTOR_H_20

# include <algorithm>
# include <cstddef>
# include <cstdlib>
# include <initializer_list>
# include <iterator>
# include <limits>
# include <new>
# include <stdexcept>
# include <type_traits>

template <typename T>
class vector {
public:
	static_assert (std::is_destructible_v <T>, "T must be destructible.");

	using iterator = T *;
	using const_iterator =  const T *;

	using reference = T &;
	using const_reference = const T &;
	using size_type = std::size_t;
	using value_type = T;

	iterator begin () { return m_data; }
	iterator end  () { return m_data + m_size; }
	const_iterator cbegin () const { return m_data; }
	const_iterator cend () const { return m_data + m_size; }
	const_iterator begin () const { return cbegin (); }
	const_iterator end () const { return cend (); }

public:
	explicit vector (const std::size_t size = 0) {
		_initialize (size);

		try {
			for (; m_size < size; m_size++) {
				new (m_data + m_size) T ();
			}
		} catch (...) { // std::bad_alloc?
			_destroy ();
			throw;
		}
	}

	vector (std::initializer_list <T> init) {
		_initialize (init.size ());

		try {
			for (const T & e : init) {
				new (m_data + m_size) T (e);
				m_size++;
			}
		} catch (...) {
			_destroy ();
			throw;
		}
	}

	vector (const_iterator begin, const_iterator end) {
		std::size_t size = std::distance (begin, end);
		_initialize (size);

		try {
			for (const_iterator it = begin; it != end; ++it) {
				new (m_data + m_size) T (* it);
				m_size++;
			}
		} catch (...) {
			_destroy ();
			throw;
		}
	}

	template <
		typename input_it,
		typename = std::enable_if_t <
			std::is_convertible_v <
				typename std::iterator_traits <input_it>::iterator_category,
				std::input_iterator_tag
			>
		>
	> vector (input_it begin, input_it end) {
		if constexpr (
			std::is_same_v <
				typename std::iterator_traits <input_it>::iterator_category,
				std::input_iterator_tag
			>
		) {
			_initialize (0);

			try {
				for (auto it = begin; it != end; it++) {
					push_back (* it);
				}
			} catch (...) {
				_destroy ();
				throw;
			}
		}
		else { // at least forward_iterator
			std::size_t size = std::distance (begin, end);
			_initialize (size);

			try {
				for (input_it it = begin; it != end; ++it) {
					new (m_data + m_size) T (* it);
					m_size++;
				}
			} catch (...) {
				_destroy ();
				throw;
			}
		}
	}

public:
	vector (const vector & other) {
		_initialize (other.m_size);

		try {
			while (m_size < other.m_size) {
				new (m_data + m_size) T (other.m_data [m_size]);
				m_size++;
			}
		}
		catch (...) {
			_destroy ();
			throw;
		}
	}

	vector & operator= (const vector & other) {
		if (this != & other) {
			vector copy (other);
			std::swap (* this, copy);
		}

		return * this;
	}

	vector (vector && other) noexcept {
		m_capacity = other.m_capacity;
		m_size = other.m_size;
		m_data = other.m_data;

		other._zero ();
	}

	vector & operator= (vector && other) noexcept {
		if (this != & other) {
			swap (* this, other);
		}

		return * this;
	}

	~vector () noexcept (noexcept (_destroy ())) {
		_destroy ();
	}

public:
	friend void swap (vector & first, vector & second) {
		std::swap (first.m_size, second.m_size);
		std::swap (first.m_capacity, second.m_capacity);
		std::swap (first.m_data, second.m_data);
	}

	void clear () {
		vector tmp;
		swap (* this, tmp);
	}

	std::size_t size () const {
		return m_size;
	}

	std::size_t capacity () const {
		return m_capacity;
	}

	bool empty () const {
		return 0 == m_size;
	}

	void resize (const std::size_t new_size) {
		_resize (new_size);
	}

	void resize (const std::size_t new_size, const T & value) {
		_resize (new_size, value);
	}

	void reserve (const std::size_t new_capacity) {
		const std::size_t new_cap = _make_capacity (new_capacity);

		if (new_cap > m_capacity) {
			_realloc (new_cap);
		}
	}

	void shrink_to_fit () {
		const std::size_t new_cap = _make_capacity (m_size);

		if (new_cap < m_capacity) {
			_realloc (new_cap);
		}
	}

	T & operator [] (const std::size_t index) {
		return m_data [index]; // don't check bounds as std::vector doesn't
	}

	const T & operator [] (const std::size_t index) const {
		return m_data [index];
	}

	T & back () {
		return m_data [m_size - 1];
	}

	const T & back () const {
		return m_data [m_size - 1];
	}

	template <typename U>
	requires std::is_constructible_v <T, U>
	void push_back (U && object) {
		reserve (m_size + 1);
		new (m_data + m_size) T (std::forward <U> (object));
		m_size++;
	}

	void pop_back () {
		m_data [m_size - 1].~T ();
		m_size--;
	}

private:
	static inline constexpr std::size_t s_max_capacity = std::numeric_limits <std::size_t>::max () >> 1;

	static size_t _make_capacity (const std::size_t for_size) {
		std::size_t cap = 1;

		while (for_size > cap) {
			if (cap > s_max_capacity) {
				throw std::runtime_error ("capacity limit reached");
			}
			else {
				cap <<= 1;
			}
		}

		return cap;
	}

	void _initialize (std::size_t size) { // WARN: assumes no data is allocated
		m_size = 0;
		m_capacity = _make_capacity (size);
		m_data = static_cast <T *> (::operator new (m_capacity * sizeof (T)));
	}

	void _destroy () noexcept (std::is_nothrow_destructible_v <T>) {
		for (std::size_t i = 0; i < m_size; i++) {
			m_data [i].~T ();
		}

		m_size = 0;
		m_capacity = 0;
		::operator delete (m_data);
		m_data = nullptr;
	}

	void _zero () noexcept {
		m_size = 0;
		m_capacity = 0;
		m_data = nullptr;
	}

	void _realloc (std::size_t new_cap) {
		std::size_t new_size = m_size;
		if (new_size > new_cap) {
			new_size = new_cap;
		}

		T * new_data = static_cast <T *> (::operator new (new_cap * sizeof (T)));

		std::size_t i = 0;

		try {
			for (; i < new_size; i++) {
				if constexpr (true == std::is_nothrow_move_constructible_v <T>) {
					new (new_data + i) T (std::move (m_data [i]));
				}
				else {
					new (new_data + i) T (m_data [i]); // this can throw
				}
			}
		} catch (...) {
			for (std::size_t j = 0; j < i; j++) {
				new_data [j].~T ();
			}

			::operator delete (new_data);
			throw;
		}

		T * old_data = m_data;
		m_data = new_data;
		m_capacity = new_cap;
		std::size_t old_size = m_size;
		m_size = new_size;

		try {
			for (std::size_t i = 0; i < old_size; i++) {
				old_data [i].~T ();
			}
		}
		catch (...) {
			::operator delete (old_data);
			throw;
		}

		::operator delete (old_data);
	}

	template <typename ... U>
	requires (1 >= sizeof ... (U)) && (std::is_constructible_v <T, U> && ...)
	void _resize (const std::size_t new_size, const std::decay_t <U> & ... value) {
		if (new_size > m_size) {
			reserve (new_size);

			std::size_t i = m_size;
			try {
				for (; i < new_size; i++) {
					new (m_data + i) T (value ...);
				}
			}
			catch (...) {
				for (std::size_t j = m_size; j < i; j++) {
					m_data [j].~T ();
				}
				throw;
			}

			m_size = new_size;
		}
		else if (new_size < m_size) {
			for (std::size_t i = new_size; i < m_size; i++) {
				m_data [i].~T ();
			} // do not catch destruction exceptions... stl doesn't too
			m_size = new_size;
		}
	}

private:
	mutable std::size_t m_size;
	std::size_t m_capacity;
	T * m_data;
};

# endif // VECTOR_H_20
