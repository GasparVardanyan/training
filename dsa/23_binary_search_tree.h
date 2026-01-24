# ifndef BINARY_SEARCH_TREE_23
# define BINARY_SEARCH_TREE_23

# include <concepts>
# include <iterator>
# include <ostream>
# include <utility>

# include "20_vector.h"
# include "23_binary_tree_node.h"

// TODO: learn commenting: https://www.doxygen.nl/manual/docblocks.html

/// @tparam T
/// @tparam Comparator STRICTLY-LESS-THAN comparator
/// @warning Comparator must impose a strict weak ordering
template <typename T, auto Comparator = [] (const T & t1, const T & t2) -> bool { return t1 < t2; }>
requires requires (T t1, T t2) {
	{ Comparator (t1, t2) } -> std::convertible_to <bool>;
}
class binary_search_tree {
public:
	using node = binary_tree_node <T>;

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
				if (Comparator (value, n->data)) {
					if (nullptr == n->left) {
						n->left = new node (std::forward <U> (value));
						m_size++;
						break;
					}
					else {
						n = n->left;
					}
				}
				else if (Comparator (n->data, value)) {
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
		node const * const * link = & m_root;

		while (nullptr != * link) {
			bool lt = false == Comparator ((* link)->data, value);
			bool gt = false == Comparator (value, (* link)->data);

			if (lt && gt) {
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
			return true;
		}
		else {
			return false;
		}
	}

	void remove (const T & value) {
		if (nullptr == m_root) {
			return;
		}
		else {
			node ** link = & m_root;

			while (nullptr != (* link)) {
				bool lt = false == Comparator ((* link)->data, value);
				bool gt = false == Comparator (value, (* link)->data);

				if (lt && gt) {
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
				node * linked_node = * link;

				if (nullptr == (* link)->right) {
					* link = (* link)->left;
				}
				else {
					if (nullptr != (* link)->left) {
						node * dl = (* link)->right;

						while (nullptr != dl->left) {
							dl = dl->left;
						}
						dl->left = (* link)->left;
					}

					* link = (* link)->right;
				}

				linked_node->left = nullptr;
				linked_node->right = nullptr;
				delete linked_node;
				m_size--;
			}
		}
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

	operator vector <T> () const {
		vector <T> v;
		v.reserve (m_size);

		dump (std::back_inserter (v));

		return v;
	}

	template <typename It>
	requires std::output_iterator <It, T>
	void dump (It it) const {
		if (nullptr != m_root) {
			m_root->inorder_traverse (
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

	const node * root () { return m_root; }
	std::size_t size () const { return m_size; }
	bool empty () const { return 0 == m_size; }

private:
	node * m_root;
	std::size_t m_size;
};

# endif // BINARY_SEARCH_TREE_23
