# ifndef LIST_H_21
# define LIST_H_21

# include <algorithm>
# include <initializer_list>
# include <type_traits>
# include <utility>

template <typename T>
class list {
private:
	struct node_base {
		explicit node_base (node_base * prev = nullptr, node_base * next = nullptr)
			: prev (prev), next (next) {}

		node_base * prev;
		node_base * next;
	};

	struct node : public node_base {
		explicit node (const T & value, node_base * prev = nullptr, node_base * next = nullptr)
			: node_base (prev, next), value (value)
		{}
		explicit node (T && value, node_base * prev = nullptr, node_base * next = nullptr)
			: node_base (prev, next), value (std::move (value))
		{}

		T value;
	};

	template <typename NPT>
	class iterator_base {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using iterator_category = std::bidirectional_iterator_tag;

	private:
		using _reference_t = std::conditional_t <std::is_const_v <std::remove_pointer_t <NPT>>, const T &, T &>;
		using _node_ptr_t = std::conditional_t <std::is_const_v <std::remove_pointer_t <NPT>>, const node *, node *>;

	public:
		friend class iterator_base <const std::decay_t <std::remove_pointer_t <NPT>> *>;

		explicit iterator_base (NPT node)
			: m_node (node)
		{}

		template <
			typename = std::enable_if <
				true == std::is_const_v <std::remove_pointer_t <NPT>>
			>
		>
		iterator_base (const iterator_base <std::decay_t <std::remove_pointer_t <NPT>> *> & other)
			: m_node (other.m_node)
		{}

		// iterator_base (const iterator_base &) = delete;
		// iterator_base (iterator_base &&) = delete;
		// iterator_base & operator= (const iterator_base &) = delete;
		// iterator_base & operator= (iterator_base &&) = delete;

		_reference_t operator* () {
			return static_cast <_node_ptr_t> (m_node)->value;
		}

		template <
			typename = std::enable_if <
				false == std::is_const_v <std::remove_pointer_t <NPT>>
			>
		>
		const _reference_t operator* () const {
			return static_cast <_node_ptr_t> (m_node)->value;
		}

		bool operator== (const iterator_base & other) const {
			return m_node == other.m_node;
		}

		iterator_base & operator++ () {
			m_node = m_node->next;
			return * this;
		}

		iterator_base operator++ (int) {
			m_node = m_node->next;
			return iterator_base (m_node->prev); // don't increment end() or cend()
		}

		iterator_base & operator-- () {
			m_node = m_node->prev;
			return * this;
		}

		iterator_base operator-- (int) {
			m_node = m_node->prev;
			return iterator_base (m_node->next); // don't decrement begin() or cbegin()
		}

		NPT base () {
			return m_node;
		}

	private:
		NPT m_node;
	};

public:
	using iterator = iterator_base <node_base *>;
	using const_iterator = iterator_base <const node_base *>;
	using value_type = T;

	iterator begin () { return iterator (m_head->next); }
	iterator end () { return iterator (m_tail); }
	const_iterator cbegin () const { return const_iterator (m_head->next); }
	const_iterator cend () const { return const_iterator (m_tail); }
	const_iterator begin () const { return const_iterator (m_head->next); }
	const_iterator end () const { return const_iterator (m_tail); }

public:
	explicit list (std::size_t size = 0) {
		m_size = size;
		m_head = new node_base;
		m_tail = new node_base;

		node_base * _node = m_head;

		for (std::size_t i = 0; i < m_size; i++) {
			node * new_node = new node (T {});
			_node->next = new_node;
			new_node->prev = _node;

			_node = new_node;
		}

		_node->next = m_tail;
		m_tail->prev = _node;
	}

	list (std::initializer_list <T> init) {
		m_size = init.size ();
		m_head = new node_base;
		m_tail = new node_base;

		node_base * _node = m_head;

		for (auto it = init.begin (); it != init.end (); std::advance (it, 1)) {
			node * new_node = new node (* it);
			_node->next = new_node;
			new_node->prev = _node;

			_node = new_node;
		}

		_node->next = m_tail;
		m_tail->prev = _node;
	}

	list (const const_iterator & begin, const const_iterator & end) {
		m_size = 0;
		m_head = new node_base;
		m_tail = new node_base;

		node_base * _node = m_head;

		for (auto it = begin; it != end; ++it) {
			node * new_node = new node (* it);
			_node->next = new_node;
			new_node->prev = _node;
			m_size++;

			_node = new_node;
		}

		_node->next = m_tail;
		m_tail->prev = _node;
	}

	template <
		typename input_it,
		typename = std::enable_if <
			std::is_convertible_v <
				typename std::iterator_traits <input_it>::iterator_category,
				std::input_iterator_tag
			>
		>
	> list (input_it begin, input_it end) {
		m_size = 0;
		m_head = new node_base;
		m_tail = new node_base;
		m_head->next = m_tail;
		m_tail->prev = m_head;

		std::copy (begin, end, std::back_inserter (* this));
	}

public:
	list (const list & other) {
		m_size = other.m_size;
		m_head = new node_base;
		m_tail = new node_base;

		node_base * _node = m_head;

		if (0 != other.m_size) {
			for (node_base * _n = other.m_head->next; _n != other.m_tail; _n = _n->next) {
				node * new_node = new node (static_cast <node *> (_n)->value);
				_node->next = new_node;
				new_node->prev = _node;

				_node = new_node;
			}
		}

		_node->next = m_tail;
		m_tail->prev = _node;
	}

	list & operator= (const list & other) {
		if (this != & other) {
			list copy (other);
			std::swap (* this, copy);
		}
		return * this;
	}

	list (list && other) {
		m_size = other.m_size;
		m_head = other.m_head;
		m_tail = other.m_tail;
		other.m_head = new node_base;
		other.m_tail = new node_base;
		other.m_head->next = other.m_tail;
		other.m_tail->prev = other.m_head;
		other.m_size = 0;
	}

	list & operator= (list && other) {
		if (this != & other) {
			std::swap (m_size, other.m_size);
			std::swap (m_tail, other.m_tail);
			std::swap (m_head, other.m_head);
		}

		return * this;
	}

	~list () {
		clear ();
		delete m_head;
		delete m_tail;
	}

public:
	void clear () {
		if (0 != m_size) {
			for (
				node_base * n = m_head->next;
				n != m_tail;
				n = n->next, delete n->prev
			);

			m_head->next = m_tail;
			m_tail->prev = m_head;

			m_size = 0;
		}
	}

	iterator insert (const_iterator at, const T & value) {
		node_base * nb = const_cast <node_base *> (at.base ());
		node * new_node = new node (value, nb->prev, nb);

		if (nullptr != nb->prev) {
			nb->prev->next = new_node;
		}

		nb->prev = new_node;

		m_size++;

		return iterator (new_node);
	}

	iterator insert (const_iterator at, T && value) {
		node_base * nb = const_cast <node_base *> (at.base ());
		node * new_node = new node (std::move (value), nb->prev, nb);

		if (nullptr != nb->prev) {
			nb->prev->next = new_node;
		}

		nb->prev = new_node;

		m_size++;

		return iterator (new_node);
	}

	iterator erase (const_iterator at) {
		node_base * nb = const_cast <node_base *> (at.base ());
		node_base * ret_node;

		if (m_tail != nb) {
			ret_node = nb->next;

			nb->prev->next = nb->next;
			if (nullptr != nb->next) { // nb == m_tail ?
				nb->next->prev = nb->prev;
			}

			delete nb;

			m_size--;
		}
		else {
			ret_node = m_tail;
		}

		return iterator (ret_node);
	}

	iterator erase (const_iterator from, const_iterator to) {
		// assuming from <= to
		node_base * nb_from = const_cast <node_base *> (from.base ());
		node_base * ret_node;

		node_base * nb_to = const_cast <node_base *> (to.base ());
		ret_node = nb_to;

		if (m_tail != nb_from && from != to) {
			// tail != from < to

			nb_from->prev->next = nb_to;
			nb_to->prev = nb_from->prev;

			for (const node_base * nb = nb_from; nb != nb_to;) {
				const node_base * next = nb->next;
				delete nb;
				nb = next;
				m_size--;
			}
		}

		return iterator (ret_node);
	}

public:
	std::size_t size () const {
		return m_size;
	}

	bool empty () const {
		return 0 == m_size;
	}

	T & front () {
		return static_cast <node *> (m_head->next)->value;
	}

	const T & front () const {
		return static_cast <node *> (m_head->next)->value;
	}

	T & back () {
		return static_cast <node *> (m_tail->prev)->value;
	}

	const T & back () const {
		return static_cast <node *> (m_tail->prev)->value;
	}

	void push_front (const T & value) { insert (cbegin (), value); }
	void push_front (T && value) { insert (cbegin (), std::move (value)); }

	void push_back (const T & value) { insert (cend (), value); }
	void push_back (T && value) { insert (cend (), std::move (value)); }

	void pop_front () { erase (cbegin ()); }
	void pop_back () { erase (-- cend ()); }

private:
	node_base * m_head;
	node_base * m_tail;
	std::size_t m_size;
};

# endif // LIST_H_21
