# ifndef BINARY_SEARCH_TREE_23
# define BINARY_SEARCH_TREE_23

# include <concepts>
# include <functional>
# include <iterator>
# include <ostream>
# include <type_traits>
# include <utility>

# include "20_vector.h"
# include "23_binary_tree_node.h"

namespace detail {
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

template <typename T, typename Comparator, bool KeepInvariant__, bool _RemovePreserveLeft>
requires std::strict_weak_order <Comparator, T, T>
struct binary_search_tree__ {
	using equal_to = equal_to <T, Comparator>;
	using node = binary_tree_node <T, equal_to>;

	static constexpr bool KeepInvariant = KeepInvariant__;
	static constexpr bool RemovePreserveLeft = _RemovePreserveLeft;

	template <typename U>
	static constexpr bool is_node_link_v =
		std::is_pointer_v <U> &&
		std::is_pointer_v <std::remove_pointer_t <U>> &&
		std::is_same_v <
			std::remove_cv_t <std::remove_pointer_t <
				std::remove_cv_t <std::remove_pointer_t <
					std::remove_cv_t <U>
				>>
			>>,
			node
		>
	;
};
}

// TODO: learn commenting: https://www.doxygen.nl/manual/docblocks.html

/// @tparam T
/// @tparam Comparator STRICTLY-LESS-THAN comparator
/// @warning Comparator must impose a strict weak ordering
template <typename T, typename Comparator = std::less <T>, bool KeepInvariant = false, bool RemovePreserveLeft = false>
requires std::strict_weak_order <Comparator, T, T>
class binary_search_tree {
protected:
	using detail = detail::binary_search_tree__ <T, Comparator, KeepInvariant, RemovePreserveLeft>;

public:
	using node = detail::node;
	using node_link = node **;
	using const_node_link = const node * const *;
	static constexpr Comparator less_than {};

public:
	binary_search_tree ()
		: m_root (nullptr)
		, m_size (0)
	{}

	binary_search_tree (const binary_search_tree & other) {
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

		dump_sorted (std::back_inserter (v));

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
		node_link link = get_link (value);
		if (nullptr == * link) {
			* link = new node (std::forward <U> (value));
			m_size++;
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
		return * get_link (value);
	}

	node * at (const T & value) {
		return * get_link (value);
	}

	void remove (const T & value) {
		node_link link = get_link (value);

		if (nullptr != * link) {
			node_link link_left = & (* link)->left;
			node_link link_right = & (* link)->right;

			node * to_remove = * link;

			if (nullptr == * link_right) {
				* link = * link_left;
			}
			else if (nullptr == * link_left) {
				* link = * link_right;
			}
			else {
				if constexpr (true == detail::KeepInvariant) {
					if constexpr (true == detail::RemovePreserveLeft) {
						node * left_rightmost = * get_link_to_rightmost (link_left);
						left_rightmost->right = * link_right;
						* link = * link_left;
					}
					else {
						node * right_leftmost = * get_link_to_leftmost (link_right);
						right_leftmost->left = * link_left;
						* link = * link_right;
					}
				}
				else {
					if constexpr (true == detail::RemovePreserveLeft) {
						node_link left_rightmost_link = get_link_to_rightmost (link_left);

						node * left_rightmost = * left_rightmost_link;
						left_rightmost->right = * link_right;

						if (left_rightmost_link != link_left) {
							* left_rightmost_link = left_rightmost->left;
							left_rightmost->left = * link_left;
						}

						* link = left_rightmost;
					}
					else {
						node_link right_leftmost_link = get_link_to_leftmost (link_right);

						node * right_leftmost = * right_leftmost_link;
						right_leftmost->left = * link_left;

						if (right_leftmost_link != link_right) {
							* right_leftmost_link = right_leftmost->right;
							right_leftmost->right = * link_right;
						}

						* link = right_leftmost;
					}
				}
			}

			to_remove->left = nullptr;
			to_remove->right = nullptr;
			delete to_remove;
			m_size--;
		}
	}

	void dump_sorted (std::output_iterator <T> auto it) const {
		if (nullptr != m_root) {
			m_root->inorder_traverse (
				[&it] (const node * n, const node *) -> void {
					*it++ = n->data;
				}
			);
		}
	}

	void dump_invariant (std::output_iterator <T> auto it) const {
		if (nullptr != m_root) {
			m_root->level_order_traverse (
				[&it] (const node * n, const node *) -> void {
					*it++ = n->data;
				}
			);
		}
	}

	const node * find_min () const {
		return * get_link_to_leftmost (& m_root);
	}

	const node * find_max () const {
		return * get_link_to_rightmost (& m_root);
	}

	void make_empty () {
		if (nullptr != m_root) {
			delete m_root;
			m_root = nullptr;
			m_size = 0;
		}
	}

	const node * root () const { return m_root; }
	std::size_t size () const { return m_size; }
	bool empty () const { return 0 == m_size; }

	std::size_t internal_path_length () {
		std::size_t s = 0;

		if (nullptr != m_root) {
			m_root->level_order_traverse ([& s] (const node *, const node *, std::size_t l) -> void {
				s += l;
			});
		}

		return s;
	}

protected:
	const_node_link get_link (const T & value) const {
		const_node_link link = & m_root;

		while (nullptr != * link) {
			bool lt = less_than (value, (* link)->data);
			bool gt = less_than ((* link)->data, value);

			if (true == lt) {
				link = & (* link)->left;
			}
			else if (true == gt) {
				link = & (* link)->right;
			}
			else {
				break;
			}
		}

		return link;
	}

	node_link get_link (const T & value) {
		return const_cast <node_link> (const_cast <const binary_search_tree *> (this)->get_link (value));
	}

	stack <const_node_link> get_link_stack (const T & value) const {
		return get_link_stack <const_node_link> (& m_root, value);
	}

	stack <node_link> get_link_stack (const T & value) {
		return get_link_stack <node_link> (& m_root, value);
	}

	stack <const_node_link> get_link_stack_to_leftmost (const_node_link link) const {
		return get_link_stack_to_leftmost <const_node_link> (link);
	}

	stack <node_link> get_link_stack_to_leftmost (node_link link) {
		return get_link_stack_to_leftmost <node_link> (link);
	}

	stack <const_node_link> get_link_stack_to_rightmost (const_node_link link) const {
		return get_link_stack_to_rightmost <const_node_link> (link);
	}

	stack <node_link> get_link_stack_to_rightmost (node_link link) {
		return get_link_stack_to_rightmost <node_link> (link);
	}

protected:
	node * m_root;
	std::size_t m_size;

protected:
	template <typename U>
	requires detail::template is_node_link_v <U>
	static stack <U> get_link_stack (U link, const T & value) {
		stack <U> link_stack;
		link_stack.push (link);

		while (nullptr != * link) {
			bool lt = less_than (value, (* link)->data);
			bool gt = less_than ((* link)->data, value);

			if (true == lt) {
				link = & (* link)->left;
				link_stack.push (link);
			}
			else if (true == gt) {
				link = & (* link)->right;
				link_stack.push (link);
			}
			else {
				break;
			}
		}

		return link_stack;
	}

	template <typename U>
	requires detail::template is_node_link_v <U>
	static U get_link_to_leftmost (U link) {
		if (nullptr != * link) {
			while (nullptr != (* link)->left) {
				link = & (* link)->left;
			}
		}

		return link;
	}

	template <typename U>
	requires detail::template is_node_link_v <U>
	static U get_link_to_rightmost (U link) {
		if (nullptr != * link) {
			while (nullptr != (* link)->right) {
				link = & (* link)->right;
			}
		}

		return link;
	}

	template <typename U>
	requires detail::template is_node_link_v <U>
	static stack <U> get_link_stack_to_leftmost (U link) {
		stack <U> link_stack;
		link_stack.push (link);

		if (nullptr != * link) {
			while (nullptr != (* link)->left) {
				link = & (* link)->left;
				link_stack.push (link);
			}
		}

		return link_stack;
	}

	template <typename U>
	requires detail::template is_node_link_v <U>
	static stack <U> get_link_stack_to_rightmost (U link) {
		stack <U> link_stack;
		link_stack.push (link);

		if (nullptr != * link) {
			while (nullptr != (* link)->right) {
				link = & (* link)->right;
				link_stack.push (link);
			}
		}

		return link_stack;
	}
};

# endif // BINARY_SEARCH_TREE_23
