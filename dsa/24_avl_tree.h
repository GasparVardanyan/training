# ifndef AVL_TREE_H_24
# define AVL_TREE_H_24

# include <concepts>
# include <cstddef>
# include <functional>
# include <iostream>
# include <iterator>
# include <ostream>
# include <variant>

# include "20_vector.h"
# include "22_stack.h"
# include "23_binary_tree_node.h"
# include "23_binary_search_tree.h"

namespace detail {
template <typename T, typename C, typename Data = std::monostate>
struct avl_tree__ {
	struct node_data : Data {
		static_assert (false == requires (Data d) { d.value; }, "the value member of Data is reserved");
		static_assert (
			false == requires (Data d) { d.height_plus_one; },
			"the height_plus_one member of Data is reserved"
		);

		T value;
		std::size_t height_plus_one;

		explicit (false) node_data (const T & value, std::size_t height_plus_one = 0)
			: value (value), height_plus_one (height_plus_one) {}

		explicit (false) node_data (T && value, std::size_t height_plus_one = 0)
			: value (std::move (value)), height_plus_one (height_plus_one) {}

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
	using node_link = tree::node_link;
	using const_node_link = tree::const_node_link;
	using node_data = detail::node_data;
	using value_type = T;

public: // binary_search_tree interface
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
	using tree::internal_path_length;

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
		stack <node_link> path = getLinkStack (value);
		node_link link = path.top ();

		if (nullptr == * link) {
			* link = new node (node_data (std::forward <U> (value), 1));
			this->m_size++;
			rebalance (std::move (path));
		}
	}

private:
	/// rebalance the tree
	/// @param path stack of links from leaf to root
	template <bool removing = false>
	void rebalance (stack <node_link> && path) {
		node_link child = path.top ();
		path.pop ();

		bool _left;

		while (false == path.empty ()) {
			node_link link = path.top ();
			path.pop ();

			bool _left1;
			if constexpr (false == removing) {
				_left1 = * child == (* link)->left;
			}
			else {
				_left1 = * child == (* link)->right;
			}

			std::size_t lh = getNodeHeightPlusOne ((* link)->left);
			std::size_t rh = getNodeHeightPlusOne ((* link)->right);

			if (lh > rh && lh - rh > 1) {
				if (true == _left) {
					node * n = * link;
					node * nl = n->left;
					node * nlr = nl->right;

					nl->right = n;
					n->left = nlr;

					* link = nl;

					if constexpr (false == removing) {
						n->data.height_plus_one -= 1;
					}
					else {
						n->data.height_plus_one -= 2;
					}

					break; // since link->data.height_plus_one remains the same
				}
				else {
					node * n = * link;
					node * nl = n->left;
					node * nlr = nl->right;
					node * nlrl = nlr->left;
					node * nlrr = nlr->right;

					* link = nlr;

					nlr->left = nl;
					nlr->right = n;
					nl->right = nlrl;
					n->left = nlrr;

					if constexpr (false == removing) {
						n->data.height_plus_one -= 1;
					}
					else {
						n->data.height_plus_one -= 2;
					}
					nl->data.height_plus_one -= 1;
					nlr->data.height_plus_one += 1;
				}
			}
			else if (rh > lh && rh - lh > 1) {
				if (false == _left) {
					node * n = * link;
					node * nr = n->right;
					node * nrl = nr->left;

					* link = nr;

					nr->left = n;
					n->right = nrl;

					if constexpr (false == removing) {
						n->data.height_plus_one -= 1;
					}
					else {
						n->data.height_plus_one -= 2;
					}

					break; // since link->data.height_plus_one remains the same
				}
				else {
					node * n = * link;
					node * nr = n->right;
					node * nrl = nr->left;
					node * nrll = nrl->left;
					node * nrlr = nrl->right;

					* link = nrl;

					nrl->left = n;
					nrl->right = nr;
					n->right = nrll;
					nr->left = nrlr;

					if constexpr (false == removing) {
						n->data.height_plus_one -= 1;
					}
					else {
						n->data.height_plus_one -= 2;
					}
					nr->data.height_plus_one -= 1;
					nrl->data.height_plus_one += 1;
				}
			}
			else {
				const std::size_t newHeight = 1 + (lh > rh ? lh : rh);
				node * n = * link;

				if (newHeight == n->data.height_plus_one) {
					break;
				}
				else {
					n->data.height_plus_one = newHeight;
				}
			}

			child = link;

			_left = _left1;
		}
	}

	std::size_t getNodeHeightPlusOne (node * node) {
		if (nullptr == node) {
			return 0;
		}
		else {
			return node->data.height_plus_one;
		}
	}

protected:
	using tree::getLinkStack;
};

# endif // AVL_TREE_H_24
