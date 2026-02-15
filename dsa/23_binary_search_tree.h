# ifndef BINARY_SEARCH_TREE_23
# define BINARY_SEARCH_TREE_23

# include <concepts>
# include <functional>
# include <iterator>
# include <ostream>
# include <utility>

# include "20_vector.h"
# include "23_binary_tree_node.h"

namespace detail {
namespace binary_search_tree__ {
	template <typename T, typename Comparator>
	requires std::strict_weak_order <Comparator, T, T>
	struct equal_to {
		static constexpr Comparator comparator {};

		bool operator () (const T & first, const T & second) const {
			return
				   false == comparator (first, second)
				&& false == comparator (second, first)
			;
		}
	};

	template <typename T, typename U>
	requires std::convertible_to <T, U>
	struct equal_to <T, std::less <U>> : std::equal_to <U> {};

	template <typename T, typename U>
	requires std::convertible_to <T, U>
	struct equal_to <T, std::greater <U>> : std::equal_to <U> {};
}
}

// TODO: learn commenting: https://www.doxygen.nl/manual/docblocks.html

/// @tparam T
/// @tparam Comparator STRICTLY-LESS-THAN comparator
/// @warning Comparator must impose a strict weak ordering
template <typename T, typename Comparator = std::less <T>>
requires std::strict_weak_order <Comparator, T, T>
class binary_search_tree {
public:
	using node = binary_tree_node <T, detail::binary_search_tree__::equal_to <T, Comparator>>;
	static constexpr Comparator less_than {};

public:
	binary_search_tree ()
		: m_root (nullptr)
		, m_size (0)
	{}

	binary_search_tree (const binary_search_tree & other)
	{
		if (nullptr != other.m_root) {
			m_root = new node (* other.m_root);
			m_size = other.m_size;
		}
		else {
			m_root = nullptr;
			m_size = 0;
		}
	}

	binary_search_tree & operator= (const binary_search_tree & other) {
		if (this != & other) {
			delete m_root;

			if (nullptr != other.m_root) {
				m_root = new node (* other.m_root);
				m_size = other.m_size;
			}
			else {
				m_root = nullptr;
				m_size = 0;
			}
		}

		return * this;
	}

	binary_search_tree (binary_search_tree && other) noexcept
		: m_root (other.m_root)
		, m_size (other.m_size)
	{
		other.m_root = nullptr;
		other.m_size = 0;
	}

	binary_search_tree & operator= (binary_search_tree && other) noexcept {
		if (this != & other) {
			std::swap (m_root, other.m_root);
			std::swap (m_size, other.m_size);
		}

		return * this;
	}

	~binary_search_tree () {
		delete m_root;
		m_size = 0;
	}

public:
	bool operator== (const binary_search_tree & other) const {
		if (m_size != other.m_size) {
			return false;
		}

		if (nullptr != m_root && nullptr != other.m_root) {
			return * m_root == * other.m_root;
		}
		else {
			return m_root == other.m_root;
		}
	}

	operator vector <T> () const {
		vector <T> v;
		v.reserve (m_size);

		dumpSorted (std::back_inserter (v));

		return v;
	}

	friend std::ostream & operator<< (std::ostream & os, const binary_search_tree & tree)
		requires requires (std::ostream & os, T t) {
			{ os << t } -> std::convertible_to <std::ostream &>;
		}
	{
		if (nullptr != tree.m_root) {
			os << * tree.m_root;
		}

		return os;
	}

public:
	template <typename U>
	requires std::convertible_to <U, T>
	void insert (U && value) {
		if (nullptr == m_root) {
			m_root = new node (std::forward <U> (value));
			m_size++;
		}
		else {
			node * n = m_root;

			while (true) {
				if (true == less_than (value, n->data)) {
					if (nullptr == n->left) {
						n->left = new node (std::forward <U> (value));
						m_size++;
						break;
					}
					else {
						n = n->left;
					}
				}
				else if (true == less_than (n->data, value)) {
					if (nullptr == n->right) {
						n->right = new node (std::forward <U> (value));
						m_size++;
						break;
					}
					else {
						n = n->right;
					}
				}
				else {
					// n == value
					break;
				}
			}
		}
	}

	bool contains (const T & value) const {
		const node * n = at (value);

		if (nullptr != n) {
			return true;
		}
		else {
			return false;
		}
	}

	const node * at (const T & value) const {
		node * const * link = & m_root;

		while (nullptr != * link) {
			bool lt = less_than (value, (* link)->data);
			bool gt = less_than ((* link)->data, value);

			if (false == lt && false == gt) {
				break;
			}
			else if (true == lt) {
				link = & (* link)->left;
			}
			else if (true == gt) {
				link = & (* link)->right;
			}
		}

		return * link;
	}

	void remove (const T & value) {
		if (nullptr == m_root) {
			return;
		}
		else {
			node ** link = & m_root;

			while (nullptr != (* link)) {
				bool lt = less_than (value, (* link)->data);
				bool gt = less_than ((* link)->data, value);

				if (false == lt && false == gt) {
					break;
				}
				else if (lt) {
					link = & (* link)->left;
				}
				else if (gt) {
					link = & (* link)->right;
				}
			}

			if (nullptr != * link) {
				node * to_remove = * link;

				if (nullptr == (* link)->right) {
					* link = (* link)->left;
				}
				else {
					if (nullptr != (* link)->left) {
						node * leftmost = (* link)->right;

						while (nullptr != leftmost->left) {
							leftmost = leftmost->left;
						}
						leftmost->left = (* link)->left;
					}

					* link = (* link)->right;
				}

				to_remove->left = nullptr;
				to_remove->right = nullptr;
				delete to_remove;
				m_size--;
			}
		}
	}

	void dumpSorted (std::output_iterator <T> auto it) const {
		if (nullptr != m_root) {
			m_root->inorder_traverse (
				[&it] (const node * n, const node *) -> void {
					*it++ = n->data;
				}
			);
		}
	}

	void dumpInvariant (std::output_iterator <T> auto it) const {
		if (nullptr != m_root) {
			m_root->level_order_traverse (
				[&it] (const node * n, const node *) -> void {
					*it++ = n->data;
				}
			);
		}
	}

	const node * findMin () const {
		if (nullptr == m_root) {
			return nullptr;
		}
		else {
			const node * n = m_root;
			while (nullptr != n->left) {
				n = n->left;
			}
			return n;
		}
	}

	const node * findMax () const {
		if (nullptr == m_root) {
			return nullptr;
		}
		else {
			const node * n = m_root;
			while (nullptr != n->right) {
				n = n->right;
			}
			return n;
		}
	}

	void makeEmpty () {
		if (nullptr != m_root) {
			delete m_root;
			m_root = nullptr;
			m_size = 0;
		}
	}

	const node * root () const { return m_root; }
	std::size_t size () const { return m_size; }
	bool empty () const { return 0 == m_size; }

protected:
	node * _at (const T & value) {
		return const_cast <node *> (at (value));
	}

protected:
	node * m_root;
	std::size_t m_size;
};

# endif // BINARY_SEARCH_TREE_23
