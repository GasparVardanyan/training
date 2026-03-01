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

template <typename T, typename Comparator, bool _keepInvariant, bool _removePreserveLeft>
requires std::strict_weak_order <Comparator, T, T>
struct binary_search_tree__ {
	using equal_to = equal_to <T, Comparator>;
	using node = binary_tree_node <T, equal_to>;

	static constexpr bool keepInvariant = _keepInvariant;
	static constexpr bool removePreserveLeft = _removePreserveLeft;

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
template <typename T, typename Comparator = std::less <T>, bool keepInvariant = false, bool removePreserveLeft = false>
requires std::strict_weak_order <Comparator, T, T>
class binary_search_tree {
public:
	using detail = detail::binary_search_tree__ <T, Comparator, keepInvariant, removePreserveLeft>;
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
		node_link link = getLink (value);
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
		return * getLink (value);
	}

	node * at (const T & value) {
		return * getLink (value);
	}

	void remove (const T & value) {
		node_link link = getLink (value);

		if (nullptr != * link) {
			node_link linkLeft = & (* link)->left;
			node_link linkRight = & (* link)->right;

			node * to_remove = * link;

			if (nullptr == * linkRight) {
				* link = * linkLeft;
			}
			else if (nullptr == * linkLeft) {
				* link = * linkRight;
			}
			else {
				if constexpr (true == detail::keepInvariant) {
					if constexpr (true == detail::removePreserveLeft) {
						node * leftRightmost = * getLinkToRightmost (linkLeft);
						leftRightmost->right = * linkRight;
						* link = * linkLeft;
					}
					else {
						node * rightLeftmost = * getLinkToLeftmost (linkRight);
						rightLeftmost->left = * linkLeft;
						* link = * linkRight;
					}
				}
				else {
					if constexpr (true == detail::removePreserveLeft) {
						node_link leftRightmostLink = getLinkToRightmost (linkLeft);

						node * leftRightmost = * leftRightmostLink;
						leftRightmost->right = * linkRight;

						if (leftRightmostLink != linkLeft) {
							* leftRightmostLink = leftRightmost->left;
							leftRightmost->left = * linkLeft;
						}

						* link = leftRightmost;
					}
					else {
						node_link rightLeftmostLink = getLinkToLeftmost (linkRight);

						node * rightLeftmost = * rightLeftmostLink;
						rightLeftmost->left = * linkLeft;

						if (rightLeftmostLink != linkRight) {
							* rightLeftmostLink = rightLeftmost->right;
							rightLeftmost->right = * linkRight;
						}

						* link = rightLeftmost;
					}
				}
			}

			to_remove->left = nullptr;
			to_remove->right = nullptr;
			delete to_remove;
			m_size--;
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
		return * getLinkToLeftmost (& m_root);
	}

	const node * findMax () const {
		return * getLinkToRightmost (& m_root);
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
	const_node_link getLink (const T & value) const {
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

	node_link getLink (const T & value) {
		return const_cast <node_link> (const_cast <const binary_search_tree *> (this)->getLink (value));
	}

	stack <const_node_link> getLinkStack (const T & value) const {
		return getLinkStack <const_node_link> (& m_root, value);
	}

	stack <node_link> getLinkStack (const T & value) {
		return getLinkStack <node_link> (& m_root, value);
	}

	stack <const_node_link> getLinkStackToLeftmost (const_node_link link) const {
		return getLinkStackToLeftmost <const_node_link> (link);
	}

	stack <node_link> getLinkStackToLeftmost (node_link link) {
		return getLinkStackToLeftmost <node_link> (link);
	}

	stack <const_node_link> getLinkStackToRightmost (const_node_link link) const {
		return getLinkStackToRightmost <const_node_link> (link);
	}

	stack <node_link> getLinkStackToRightmost (node_link link) {
		return getLinkStackToRightmost <node_link> (link);
	}

protected:
	node * m_root;
	std::size_t m_size;

protected:
	template <typename U>
	requires detail::template is_node_link_v <U>
	static stack <U> getLinkStack (U link, const T & value) {
		stack <U> linkStack;
		linkStack.push (link);

		while (nullptr != * link) {
			bool lt = less_than (value, (* link)->data);
			bool gt = less_than ((* link)->data, value);

			if (true == lt) {
				link = & (* link)->left;
				linkStack.push (link);
			}
			else if (true == gt) {
				link = & (* link)->right;
				linkStack.push (link);
			}
			else {
				break;
			}
		}

		return linkStack;
	}

	template <typename U>
	requires detail::template is_node_link_v <U>
	static U getLinkToLeftmost (U link) {
		if (nullptr != * link) {
			while (nullptr != (* link)->left) {
				link = & (* link)->left;
			}
		}

		return link;
	}

	template <typename U>
	requires detail::template is_node_link_v <U>
	static U getLinkToRightmost (U link) {
		if (nullptr != * link) {
			while (nullptr != (* link)->right) {
				link = & (* link)->right;
			}
		}

		return link;
	}

	template <typename U>
	requires detail::template is_node_link_v <U>
	static stack <U> getLinkStackToLeftmost (U link) {
		stack <U> linkStack;
		linkStack.push (link);

		if (nullptr != * link) {
			while (nullptr != (* link)->left) {
				link = & (* link)->left;
				linkStack.push (link);
			}
		}

		return linkStack;
	}

	template <typename U>
	requires detail::template is_node_link_v <U>
	static stack <U> getLinkStackToRightmost (U link) {
		stack <U> linkStack;
		linkStack.push (link);

		if (nullptr != * link) {
			while (nullptr != (* link)->right) {
				link = & (* link)->right;
				linkStack.push (link);
			}
		}

		return linkStack;
	}
};

# endif // BINARY_SEARCH_TREE_23
