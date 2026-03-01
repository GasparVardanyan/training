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
template <typename T, typename C, typename Data = std::monostate, bool _removePreserveLeft = false>
struct avl_tree__ {
	static constexpr bool removePreserveLeft = _removePreserveLeft;

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

	using tree = binary_search_tree <node_data, C, false, removePreserveLeft>;
};
}

// WARN: removePreserveLeft isn't used
template <typename T, typename Comparator = std::less <T>, typename Data = std::monostate, bool removePreserveLeft = false>
requires std::strict_weak_order <Comparator, T, T>
class avl_tree : protected detail::avl_tree__ <T, Comparator, Data, removePreserveLeft>::tree {
public:
	using detail = detail::avl_tree__ <T, Comparator, Data, removePreserveLeft>;
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
		node_link link = path.top (); // path is always non-empty

		if (nullptr == * link) {
			* link = new node (node_data (std::forward <U> (value), 1));
			this->m_size++;
			rebalanceAfterInsert (std::move (path));
		}
	}

	static bool verify_avlt (node * root) {
		stack <node *> node_stack;
		node_stack.push (root);

		bool ok = true;

		while (false == node_stack.empty ()) {
			auto n = node_stack.top ();
			if (nullptr == n) {
				std::cout << "FUP2" << std::endl;
			}

			node_stack.pop ();

			std::size_t lh, rh;

			if (nullptr != n->left) {
				lh = n->left->data.height_plus_one;
				node_stack.push (n->left);
			}
			else {
				lh = 0;
			}

			if (nullptr != n->right) {
				rh = n->right->data.height_plus_one;
				node_stack.push (n->right);
			}
			else {
				rh = 0;
			}

			if (lh > rh) {
				if (n->data.height_plus_one != lh + 1) {
					ok = false;
					break;
				}
				if (lh - rh > 1) { // probably unneccessary
					ok = false;
					break;
				}
			}
			else if (rh > lh) {
				if (n->data.height_plus_one != rh + 1) {
					ok = false;
					break;
				}
				if (rh - lh > 1) {
					ok = false;
					break;
				}
			}
			else {
				if (n->data.height_plus_one != lh + 1) {
					ok = false;
					break;
				}
			}
		}

		if (true == ok) {
			vector <value_type> v;
			root->inorder_traverse (
				[&v] (const node * n, const node *) -> void {
					v.push_back (n->data.value);
				}
			);
			auto v2 = v;

			std::sort (v2.begin (), v2.end ());

			if (v != v2) {
				ok = false;
			}
		}

		return ok;
	}

	void remove (const T & value) {
		stack <node_link> path = getLinkStack (value);
		node_link link = path.top ();
		path.pop ();

		if (nullptr != * link) {
			node_link linkLeft = & (* link)->left;
			node_link linkRight = & (* link)->right;

			bool rebalance_path = true;

			node * to_remove = * link;

			if (nullptr == * linkLeft) {
				* link = * linkRight;
				rebalanceAfterRemove (std::move (path));
			}
			else if (nullptr == * linkRight) {
				* link = * linkLeft;
				rebalanceAfterRemove (std::move (path));
			}
			else {
				stack <node_link> path2;
				stack <node_link> & leftRightmostLinkStack = path2;

				leftRightmostLinkStack = getLinkStackToRightmost (linkLeft);
				node_link leftRightmostLink = leftRightmostLinkStack.top ();
				leftRightmostLinkStack.pop ();

				node * leftRightmost = * leftRightmostLink;
				leftRightmost->right = * linkRight;

				if (leftRightmostLink != linkLeft) {
					* leftRightmostLink = leftRightmost->left;
					leftRightmost->left = * linkLeft;
				}

				leftRightmost->data.height_plus_one = to_remove->data.height_plus_one;
				* link = leftRightmost;
				path.push (link);

				if (false == path2.empty ()) {
					node_link lrm_parent = path2.top ();
					path2 = getLinkStack ((* lrm_parent)->data); // FIXME: avoid this stupid garbage
					rebalance_path = rebalanceAfterRemove (std::move (path2));
				}

				if (true == rebalance_path) {
					rebalanceAfterRemove (std::move (path));
				}
			}


			to_remove->left = nullptr;
			to_remove->right = nullptr;
			delete to_remove;
			this->m_size--;
		}
	}

private:
	void rebalanceAfterInsert (stack <node_link> && path) {
		node_link child = path.top ();
		path.pop ();

		bool prevLeft;

		while (false == path.empty ()) {
			node_link link = path.top ();
			path.pop ();

			bool left = * child == (* link)->left;

			std::size_t lh = getNodeHeightPlusOne ((* link)->left);
			std::size_t rh = getNodeHeightPlusOne ((* link)->right);

			if (lh > rh && lh - rh > 1) {
				// Yes, when removing=false, at the first iteration of this loop
				// prevLeft is uninitialized. This condition will reach only after
				// the first iteration.
				if (true == prevLeft) {
					node * n = * link;
					node * nl = n->left;
					node * nlr = nl->right;

					nl->right = n;
					n->left = nlr;

					* link = nl;

					n->data.height_plus_one -= 1;
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

					n->data.height_plus_one -= 1;
					nl->data.height_plus_one -= 1;
					nlr->data.height_plus_one += 1;
				}

				break; // since link->data.height_plus_one remains the same
			}
			else if (rh > lh && rh - lh > 1) {
				if (false == prevLeft) {
					node * n = * link;
					node * nr = n->right;
					node * nrl = nr->left;

					* link = nr;

					nr->left = n;
					n->right = nrl;

					n->data.height_plus_one -= 1;
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

					n->data.height_plus_one -= 1;
					nr->data.height_plus_one -= 1;
					nrl->data.height_plus_one += 1;
				}

				break; // since link->data.height_plus_one remains the same
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
			prevLeft = left;
		}
	}

	bool rebalanceAfterRemove (stack <node_link> && path) {
		bool finished = false;

		while (false == path.empty ()) {
			node_link link = path.top ();
			path.pop ();
			node * n = * link;
			node * nl = n->left;
			node * nr = n->right;

			std::size_t lh = getNodeHeightPlusOne (nl);
			std::size_t rh = getNodeHeightPlusOne (nr);

			if (lh > rh && lh - rh > 1) {
				node * nll = nl->left;
				node * nlr = nl->right;

				std::size_t nllh = getNodeHeightPlusOne (nll);
				std::size_t nlrh = getNodeHeightPlusOne (nlr);

				if (nllh >= nlrh) {
					n->left = nlr;
					nl->right = n;
					* link = nl;

					if (nllh == nlrh) {
						n->data.height_plus_one -= 1;
						nl->data.height_plus_one += 1;
						finished = true;
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

				std::size_t nrlh = getNodeHeightPlusOne (nrl);
				std::size_t nrrh = getNodeHeightPlusOne (nrr);

				if (nrlh <= nrrh) {
					n->right = nrl;
					nr->left = n;
					* link = nr;

					if (nrlh == nrrh) {
						n->data.height_plus_one -= 1;
						nr->data.height_plus_one += 1;
						finished = true;
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
				const std::size_t newHeight = 1 + (lh > rh ? lh : rh);

				if (newHeight == n->data.height_plus_one) {
					finished = true;
					break;
				}
				else {
					n->data.height_plus_one = newHeight;
				}
			}
		}

		return finished;
	}

	inline std::size_t getNodeHeightPlusOne (node * node) {
		if (nullptr == node) {
			return 0;
		}
		else {
			return node->data.height_plus_one;
		}
	}

protected:
	using tree::getLinkStack;
	using tree::getLinkStackToLeftmost;
	using tree::getLinkStackToRightmost;
};

# endif // AVL_TREE_H_24
