# ifndef AVL_TREE_H_24
# define AVL_TREE_H_24

# include <concepts>
# include <cstddef>
# include <functional>
# include <iterator>
# include <ostream>
# include <variant>

# include "20_vector.h"
# include "22_stack.h"
# include "23_binary_tree_node.h"
# include "23_binary_search_tree.h"

template <typename T, typename Comparator, typename Data>
requires std::strict_weak_order <Comparator, T, T>
class avl_tree;

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
		std::size_t height_plus_one; // TODO: replace height with diff of subtree heights, which is always -1, 0 or 1

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

	using tree = binary_search_tree_base <
		avl_tree <T, C, Data>, node_data, C, false
	>;
};
}

template <typename T, typename Comparator = std::less <T>, typename Data = std::monostate>
requires std::strict_weak_order <Comparator, T, T>
class avl_tree : public detail::avl_tree__ <T, Comparator, Data>::tree {
protected:
	using detail = detail::avl_tree__ <T, Comparator, Data>;

public:
	using tree = detail::tree;
	friend tree;

	using node = tree::node;
	using node_link = tree::node_link;
	using const_node_link = tree::const_node_link;
	using node_data = detail::node_data;
	using value_type = T;
	using tree::less_than;

public: // binary_search_tree interface
	bool operator== (const avl_tree & other) const {
		return
			   static_cast <const tree &> (* this)
			== static_cast <const tree &> (other)
		;
	}

	operator vector <T> () const {
		vector <T> v;
		v.reserve (this->tree::size ());

		this->tree::dump_sorted (std::back_inserter (v));

		return v;
	}

	friend std::ostream & operator<< (std::ostream & os, const avl_tree & tree)
		requires requires (std::ostream & os, T t) {
			{ os << t } -> std::convertible_to <std::ostream &>;
		}
	{
		// return os << static_cast <const avl_tree::tree &> (tree);

		tree.root ()->preorder_traverse (
			[path = stack <const typename std::decay_t <decltype (tree)>::node *> (), &os]
			(auto * node, auto * parent) mutable -> void {
				if (nullptr != parent) {
					while (path.top () != parent) {
						path.pop ();
					}
				}
				path.push (node);

				std::size_t depth = path.size () - 1;

				for (std::size_t i = 0; i < depth; i++) {
					os << "  ";
				}

				if (nullptr != parent) {
					os << "|- ";
				}

				os << node->data;
				os << " (" << ((int) node->data.height_plus_one - 1) << ")";

				if (nullptr != parent) {
					if (nullptr == parent->left || nullptr == parent->right) {
						if (node == parent->left) {
							os << " (L)";
						}
						else {
							os << " (R)";
						}
					}
				}

				os << std::endl;
			}
		);

		return os;
	}

public:
	template <typename U>
	requires std::convertible_to <U, T>
	void insert (U && value) {
		stack <node_link> path = get_link_stack (value);
		node_link link = path.top (); // path is always non-empty

		if (nullptr == * link) {
			* link = new node (node_data (std::forward <U> (value), 1));
			this->m_size++;
			rebalance_after_insert (std::move (path));
		}
	}

	void remove (const T & value) {
		stack <node_link> path = get_link_stack (value);
		node_link link = path.top ();

		if (nullptr != * link) {
			node_link link_left = & (* link)->left;
			node_link link_right = & (* link)->right;

			node * to_remove = * link;

			if (nullptr == * link_left) {
				path.pop ();
				* link = * link_right;
			}
			else if (nullptr == * link_right) {
				path.pop ();
				* link = * link_left;
			}
			else {
				if ((* link_left)->data.height_plus_one > (* link_right)->data.height_plus_one) {
					node_link left_rightmost_link = get_link_to_rightmost (link_left);

					node * left_rightmost = * left_rightmost_link;
					left_rightmost->right = * link_right;
					left_rightmost->data.height_plus_one = to_remove->data.height_plus_one;

					if (left_rightmost_link != link_left) {
						* left_rightmost_link = left_rightmost->left;
						left_rightmost->left = * link_left;

						node_link n = & left_rightmost->left;

						do {
							path.push (n);
							n = & (* n)->right;
						}
						while (* left_rightmost_link != * n);
					}

					* link = left_rightmost;
				}
				else {
					node_link right_leftmost_link = get_link_to_leftmost (link_right);

					node * right_leftmost = * right_leftmost_link;
					right_leftmost->left = * link_left;
					right_leftmost->data.height_plus_one = to_remove->data.height_plus_one;

					if (right_leftmost_link != link_right) {
						* right_leftmost_link = right_leftmost->right;
						right_leftmost->right = * link_right;

						node_link n = & right_leftmost->right;

						do {
							path.push (n);
							n = & (* n)->left;
						}
						while (* right_leftmost_link != * n);
					}

					* link = right_leftmost;
				}
			}

			to_remove->left = nullptr;
			to_remove->right = nullptr;
			delete to_remove;
			this->m_size--;
			rebalance_after_remove (std::move (path));
		}
	}

protected:
	void rebalance_after_insert (stack <node_link> && path) {
		node_link child = path.top ();
		path.pop ();

		bool prev_left = false;

		while (false == path.empty ()) {
			node_link link = path.top ();
			path.pop ();
			node * n = * link;
			node * nl = n->left;
			node * nr = n->right;

			bool left = * child == (* link)->left;

			std::size_t lh = get_node_height_plus_one (nl);
			std::size_t rh = get_node_height_plus_one (nr);

			if (lh > rh && lh - rh > 1) {
				if (true == prev_left) { // [ROT] this is called right rotation... confusing names !!!
					node * nlr = nl->right;

					nl->right = n;
					n->left = nlr;

					* link = nl;

					n->data.height_plus_one -= 1;
				}
				else { // [ROT] and this is left-right
					node * nlr = nl->right;
					node * nlrl = nlr->left;
					node * nlrr = nlr->right;

					* link = nlr;

					nlr->left = nl;
					nlr->right = n;
					nl->right = nlrl;
					n->left = nlrr;

					n->data.height_plus_one -= 1;
					nl->data.height_plus_one -= 1;
					nlr->data.height_plus_one += 1;
				}

				break; // since link->data.height_plus_one remains the same
			}
			else if (rh > lh && rh - lh > 1) {
				if (false == prev_left) { // [ROT] this is left rotation
					node * nrl = nr->left;

					* link = nr;

					nr->left = n;
					n->right = nrl;

					n->data.height_plus_one -= 1;
				}
				else { // [ROT] and this is right-left rotation
					node * nrl = nr->left;
					node * nrll = nrl->left;
					node * nrlr = nrl->right;

					* link = nrl;

					nrl->left = n;
					nrl->right = nr;
					n->right = nrll;
					nr->left = nrlr;

					n->data.height_plus_one -= 1;
					nr->data.height_plus_one -= 1;
					nrl->data.height_plus_one += 1;
				}

				break; // since link->data.height_plus_one remains the same
			}
			else {
				const std::size_t new_height = 1 + (lh > rh ? lh : rh);

				if (new_height == n->data.height_plus_one) {
					break;
				}
				else {
					n->data.height_plus_one = new_height;
				}
			}

			child = link;
			prev_left = left;
		}
	}

	void rebalance_after_remove (stack <node_link> && path) {
		while (false == path.empty ()) {
			node_link link = path.top ();
			path.pop ();
			node * n = * link;
			node * nl = n->left;
			node * nr = n->right;

			std::size_t lh = get_node_height_plus_one (nl);
			std::size_t rh = get_node_height_plus_one (nr);

			if (lh > rh && lh - rh > 1) {
				node * nll = nl->left;
				node * nlr = nl->right;

				std::size_t nllh = get_node_height_plus_one (nll);
				std::size_t nlrh = get_node_height_plus_one (nlr);

				if (nllh >= nlrh) {
					n->left = nlr;
					nl->right = n;
					* link = nl;

					if (nllh == nlrh) {
						n->data.height_plus_one -= 1;
						nl->data.height_plus_one += 1;
						break;
					}
					else {
						n->data.height_plus_one -= 2;
					}
				}
				else {
					node * nlrl = nlr->left;
					node * nlrr = nlr->right;

					n->left = nlrr;
					nl->right = nlrl;
					nlr->left = nl;
					nlr->right = n;

					* link = nlr;

					n->data.height_plus_one -= 2;
					nl->data.height_plus_one -= 1;
					nlr->data.height_plus_one += 1;
				}
			}
			else if (rh > lh && rh - lh > 1) {
				node * nrl = nr->left;
				node * nrr = nr->right;

				std::size_t nrlh = get_node_height_plus_one (nrl);
				std::size_t nrrh = get_node_height_plus_one (nrr);

				if (nrlh <= nrrh) {
					n->right = nrl;
					nr->left = n;
					* link = nr;

					if (nrlh == nrrh) {
						n->data.height_plus_one -= 1;
						nr->data.height_plus_one += 1;
						break;
					}
					else {
						n->data.height_plus_one -= 2;
					}
				}
				else {
					node * nrll = nrl->left;
					node * nrlr = nrl->right;

					n->right = nrll;
					nr->left = nrlr;
					nrl->left = n;
					nrl->right = nr;
					* link = nrl;

					n->data.height_plus_one -= 2;
					nr->data.height_plus_one -= 1;
					nrl->data.height_plus_one += 1;
				}
			}
			else {
				const std::size_t new_height = 1 + (lh > rh ? lh : rh);

				if (new_height == n->data.height_plus_one) {
					break;
				}
				else {
					n->data.height_plus_one = new_height;
				}
			}
		}
	}

	inline std::size_t get_node_height_plus_one (node * node) {
		if (nullptr == node) {
			return 0;
		}
		else {
			return node->data.height_plus_one;
		}
	}

protected:
	using tree::at;
	using tree::get_link_stack;
	using tree::get_link_stack_to_leftmost;
	using tree::get_link_stack_to_rightmost;
	using tree::get_link_to_leftmost;
	using tree::get_link_to_rightmost;
};

# endif // AVL_TREE_H_24
