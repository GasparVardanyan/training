# ifndef VECTOR_H_20
# define VECTOR_H_20

# include <algorithm>
# include <cstddef>
# include <initializer_list>
# include <iterator>
# include <limits>
# include <stdexcept>
# include <type_traits>

// TODO: test with the verbose_class against std::vector

template <typename T>
class vector {
public:
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
		m_size = size;
		m_capacity = make_capacity (m_size);
		m_data = new T [m_capacity] (); // TODO: use placement-new
	}

	vector (std::initializer_list <T> init) {
		m_size = init.size ();
		m_capacity = make_capacity (m_size);
		m_data = new T [m_capacity];

		std::copy (init.begin (), init.end (), m_data);
	}

	vector (const_iterator begin, const_iterator end) {
		m_size = std::distance (begin, end);
		m_capacity = make_capacity (m_size);
		m_data = new T [m_capacity];

		std::copy (begin, end, m_data);
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
		if (
			std::is_same_v <
				typename std::iterator_traits <input_it>::iterator_category,
				std::input_iterator_tag
			>
		) {
			m_size = 0;
			m_capacity = make_capacity (m_size);
			m_data = new T [m_capacity];

			for (auto it = begin; it != end; it++) {
				push_back (* it);
			}
		}
		else { // at least forward_iterator
			m_size = std::distance (begin, end);
			m_capacity = make_capacity (m_size);
			m_data = new T [m_capacity];

			std::copy (begin, end, m_data);
		}
	}

public:
	vector (const vector & other) {
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_data = new T [m_capacity];

		std::copy (other.m_data, other.m_data + other.m_size, m_data);
	}

	vector & operator= (const vector & other) {
		if (this != & other) {
			// if (m_capacity < other.m_capacity) {
			// 	m_capacity = other.m_capacity;
			// 	delete [] m_data;
			// 	m_data = new T [m_capacity];
			// }
			// m_size = other.m_size;
			// std::copy (other.m_data, other.m_data + other.m_size, m_data);

			vector copy (other); // uses copy constructor
			// std::swap (m_data, copy.m_data);
			// std::swap (m_capacity, copy.m_capacity);
			// std::swap (m_size, copy.m_size);
			std::swap (* this, copy); // uses move assignments
		}

		return * this;
	}

	vector (vector && other) noexcept {
		m_capacity = other.m_capacity;
		m_size = other.m_size;
		m_data = other.m_data;
		other.m_capacity = 0;
		other.m_size = 0;
		other.m_data = nullptr;
	}

	vector & operator= (vector && other) noexcept {
		if (this != & other) {
			delete [] m_data;

			m_capacity = other.m_capacity;
			m_size = other.m_size;
			m_data = other.m_data;
			other.m_capacity = 0;
			other.m_size = 0;
			other.m_data = nullptr;
		}

		return * this;
	}

	~vector () {
		delete [] m_data;
		m_data = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

public:
	std::size_t size () const {
		return m_size;
	}

	std::size_t capacity () const {
		return m_capacity;
	}

	bool empty () const {
		return 0 == m_size;
	}

	void clear () {
		m_size = 0;
		m_capacity = make_capacity (m_size);
		delete [] m_data;
		m_data = new T [m_capacity];
	}

	void resize (const std::size_t new_size) {
		if (new_size > m_capacity) {
			reserve (new_size);
		}
		else if (new_size < m_capacity) {
			shrink_to_contain (new_size);
		}
		m_size = new_size;
	}

	void reserve (const std::size_t new_capacity) {
		const std::size_t new_cap = make_capacity (new_capacity);

		if (new_cap > m_capacity) {
			T * old_data = m_data;
			m_data = new T [new_cap];
			m_capacity = new_cap;
			move_copy_or_whatever_data (old_data, m_data, m_size);
			delete [] old_data;
		}
	}

	void shrink_to_contain (const std::size_t at_least) {
		const std::size_t new_cap = make_capacity (at_least);

		if (m_size > at_least) {
			m_size = at_least;
		}

		if (new_cap < m_capacity) {
			T * old_data = m_data;
			m_data = new T [new_cap];
			m_capacity = new_cap;
			move_copy_or_whatever_data (old_data, m_data, m_size);
			delete [] old_data;
		}
	}

	T & operator [] (const std::size_t index) {
		return m_data [index]; // don't check bounds as std::vector doesn't
	}

	const T & operator [] (const std::size_t index) const {
		return m_data [index];
	}

	T & back () {
		if (0 == m_size) {
			m_size = 1; // capacity is always at least 1
		}

		return m_data [m_size - 1];
	}

	const T & back () const {
		if (0 == m_size) {
			m_size = 1; // and this is why m_size is mutable
						// now a const method changes the class state
						// this is bad...
		}

		return m_data [m_size - 1];
	}

	void push_back (const T & object) {
		resize (m_size + 1);
		m_data [m_size - 1] = object;
	}

	void push_back (T && object) {
		resize (m_size + 1);
		m_data [m_size - 1] = std::move (object);
	}

	void pop_back () {
		if (m_size > 0) {
			m_size--;
		}
	}

private:
	static inline constexpr std::size_t s_max_capacity = std::numeric_limits <std::size_t>::max () >> 1;

	static size_t make_capacity (const std::size_t for_size) {
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

	static void move_copy_or_whatever_data (T * src, T * dst, const std::size_t count) {
		// TODO: consider using std::copy

		if (true == std::is_move_assignable_v <T>) {
			for (std::size_t i = 0; i < count; i++) {
				dst [i] = std::move (src [i]);
			}
		}
		else if (true == std::is_copy_assignable_v <T>) {
			for (std::size_t i = 0; i < count; i++) {
				dst [i] = src [i];
			}
		}
		else {
			throw std::runtime_error ("don't be lazy, use place-new or whatever needed");
		}
	}

private:
	mutable std::size_t m_size;
	std::size_t m_capacity;
	T * m_data;
};

# endif // VECTOR_H_20
