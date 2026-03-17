# ifndef AVL_TREE_H_24
# define AVL_TREE_H_24

# include <concepts>
# include <cstddef>
# include <functional>
# include <iterator>
# include <ostream>
# include <stdexcept>
# include <variant>

# include "20_vector.h"
# include "21_list.h"
# include "22_stack.h"
# include "23_binary_tree_node.h"
# include "23_binary_search_tree.h"

namespace detail {
template <typename T, typename C, typename Data = std::monostate, bool _removePreserveLeft = false>
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

	using tree = binary_search_tree <node_data, C, false>;
};
}

template <typename T, typename Comparator = std::less <T>, typename Data = std::monostate>
requires std::strict_weak_order <Comparator, T, T>
class avl_tree : protected detail::avl_tree__ <T, Comparator, Data>::tree {
protected:
	using detail = detail::avl_tree__ <T, Comparator, Data>;

public:
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

	void remove (const T & value) {
		stack <node_link> path = getLinkStack (value);
		node_link link = path.top ();

		if (nullptr != * link) {
			node_link linkLeft = & (* link)->left;
			node_link linkRight = & (* link)->right;

			node * to_remove = * link;

			if (nullptr == * linkLeft) {
				path.pop ();
				* link = * linkRight;
			}
			else if (nullptr == * linkRight) {
				path.pop ();
				* link = * linkLeft;
			}
			else {
				if ((* linkLeft)->data.height_plus_one > (* linkRight)->data.height_plus_one) {
					node_link leftRightmostLink = getLinkToRightmost (linkLeft);

					node * leftRightmost = * leftRightmostLink;
					leftRightmost->right = * linkRight;
					leftRightmost->data.height_plus_one = to_remove->data.height_plus_one;

					if (leftRightmostLink != linkLeft) {
						* leftRightmostLink = leftRightmost->left;
						leftRightmost->left = * linkLeft;

						node_link n = & leftRightmost->left;

						do {
							path.push (n);
							n = & (* n)->right;
						}
						while (* leftRightmostLink != * n);
					}

					* link = leftRightmost;
				}
				else {
					node_link rightLeftmostLink = getLinkToLeftmost (linkRight);

					node * rightLeftmost = * rightLeftmostLink;
					rightLeftmost->left = * linkLeft;
					rightLeftmost->data.height_plus_one = to_remove->data.height_plus_one;

					if (rightLeftmostLink != linkRight) {
						* rightLeftmostLink = rightLeftmost->right;
						rightLeftmost->right = * linkRight;

						node_link n = & rightLeftmost->right;

						do {
							path.push (n);
							n = & (* n)->left;
						}
						while (* rightLeftmostLink != * n);
					}

					* link = rightLeftmost;
				}
			}

			to_remove->left = nullptr;
			to_remove->right = nullptr;
			delete to_remove;
			this->m_size--;
			rebalanceAfterRemove (std::move (path));
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
				// Yes, at the first iteration of this loop prevLeft is
				// uninitialized. This condition will reach only after the first
				// iteration.
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

	void rebalanceAfterRemove (stack <node_link> && path) {
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
					break;
				}
				else {
					n->data.height_plus_one = newHeight;
				}
			}
		}
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
	using tree::getLinkToLeftmost;
	using tree::getLinkToRightmost;
};

# endif // AVL_TREE_H_24
