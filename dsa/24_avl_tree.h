# ifndef AVL_TREE_H_24
# define AVL_TREE_H_24

# include <concepts>
# include <cstddef>
# include <functional>
# include <iostream>
# include <iterator>
# include <ostream>
# include <variant>

# include "23_binary_search_tree.h"

namespace detail {
template <typename T, typename C, typename Data = std::monostate>
struct avl_tree__ {
	struct node_data : Data {
		static_assert (false == requires (Data d) { d.value; }, "the value member of Data is reserved");
		static_assert (false == requires (Data d) { d.height; }, "the height member of Data is reserved");

		T value;
		std::size_t height;

		explicit (false) node_data (const T & value, std::size_t height = 0)
			: value (value), height (height) {}

		explicit (false) node_data (T && value, std::size_t height = 0)
			: value (std::move (value)), height (height) {}

		friend std::ostream & operator<< (std::ostream & os, const node_data & data) {
			os << data.value;
			return os;
		}

		operator const T & () const {
			return value;
		}

		operator T & () {
			return value;
		}
	};

	using tree = binary_search_tree <node_data, C>;
};
}

template <typename T, typename Comparator = std::less <T>, typename Data = std::monostate>
requires std::strict_weak_order <Comparator, T, T>
class avl_tree : protected detail::avl_tree__ <T, Comparator, Data>::tree {
public:
	using detail = detail::avl_tree__ <T, Comparator, Data>;
	using tree = detail::tree;
	using node = tree::node;
	using node_data = detail::node_data;

public: // binary_search_tree interface
	// using tree::insert;
	// using tree::remove;
	using tree::contains;
	using tree::dumpInvariant;
	using tree::dumpSorted;
	using tree::findMin;
	using tree::findMax;
	using tree::makeEmpty;
	using tree::root;
	using tree::size;
	using tree::empty;
	using tree::at;

	bool operator== (const avl_tree & other) const {
		return
			   static_cast <const tree &> (* this)
			== static_cast <const tree &> (other)
		;
	}

	operator vector <T> () const {
		vector <T> v;
		v.reserve (size ());

		dumpSorted (std::back_inserter (v));

		return v;
	}

	friend std::ostream & operator<< (std::ostream & os, const avl_tree & tree)
		requires requires (std::ostream & os, T t) {
			{ os << t } -> std::convertible_to <std::ostream &>;
		}
	{
		return os << static_cast <const avl_tree::tree &> (tree);
	}

public:
	template <typename U>
	requires std::convertible_to <U, T>
	void insert (U && value) {
		stack <node **> path = getLinkStack (value);
		node ** link = path.top ();

		if (nullptr == * link) {
			* link = new node (node_data (std::forward <U> (value),  0));
			this->m_size++;
			rebalance (std::move (path));
		}
	}

private:
	/// rebalance the tree
	/// @param path stack of links from leaf to root
	void rebalance (stack <node **> && path) {
		(void) path;
		// while (false == path.empty ()) {
		// 	node ** link = path.top ();
		// 	path.pop ();
		//
		// 	std::size_t lh = getNodeHeightPlusOne ((* link)->left);
		// 	std::size_t rh = getNodeHeightPlusOne ((* link)->right);
		//
		// 	if (lh > rh && lh - rh > 1) {
		//
		// 	}
		// 	else if (rh > lh && rh - lh > 1) {
		//
		// 	}
		// }
	}

	std::size_t getNodeHeightPlusOne (node * node) {
		if (nullptr == node) {
			return 0;
		}
		else {
			return node->data.height + 1;
		}
	}

	void calcNodeHeight (node * node) {
		std::size_t hl = getNodeHeightPlusOne (node->left);
		std::size_t hr = getNodeHeightPlusOne (node->right);
		std::size_t mh = hl > hr ? hl : hr;

		setNodeHeight (node, mh + 1);
	}

	void setNodeHeight (node * node, std::size_t heightPlusOne) {
		node->data.height = heightPlusOne - 1;
	}

	void rotateRight (node ** link) {
		node * currBefore = * link;
		node * leftBefore = currBefore->left;
		node * leftRightBefore = leftBefore->right;

		* link = leftBefore;
		leftBefore->right = currBefore;
		currBefore->left = leftRightBefore;

		calcNodeHeight (currBefore);
		calcNodeHeight (leftBefore);
		// height (leftRightBefore) = const
	}

	void rotateLeft (node ** link) {
		node * currBefore = * link;
		node * rightBefore = currBefore->right;
		node * rightLeftBefore = rightBefore->left;

		* link = rightBefore;
		rightBefore->left = currBefore;
		currBefore->right = rightLeftBefore;

		calcNodeHeight (currBefore);
		calcNodeHeight (rightBefore);
		// height (rightLeftBefore) = const
	}

protected:
	using tree::getLinkStack;
};

# endif // AVL_TREE_H_24
