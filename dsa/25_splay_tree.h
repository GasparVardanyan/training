# ifndef SPLAY_TREE_23
# define SPLAY_TREE_23

# include "23_binary_search_tree.h"

namespace detail {
template <typename T, typename C>
struct splay_tree__ {
	using tree = binary_search_tree <T, C>;
};
}

template <typename T, typename Comparator = std::less <T>>
requires std::strict_weak_order <Comparator, T, T>
class splay_tree : protected detail::splay_tree__ <T, Comparator>::tree
{
protected:
	using detail = detail::splay_tree__ <T, Comparator>;

public:
	using tree = detail::tree;
	using node = tree::node;
	using node_link = tree::node_link;
	using const_node_link = tree::const_node_link;
	using value_type = T;
	using tree::less_than;

public: // binary_search_tree interface
	using tree::dump_invariant;
	using tree::dump_sorted;
	using tree::make_empty;
	using tree::root;
	using tree::size;
	using tree::empty;
	using tree::internal_path_length;

	bool operator== (const splay_tree & other) const {
		return
			   static_cast <const tree &> (* this)
			== static_cast <const tree &> (other)
		;
	}

	operator vector <T> () const {
		vector <T> v;
		v.reserve (size ());

		dump_sorted (std::back_inserter (v));

		return v;
	}

	friend std::ostream & operator<< (std::ostream & os, const splay_tree & tree)
		requires requires (std::ostream & os, T t) {
			{ os << t } -> std::convertible_to <std::ostream &>;
		}
	{
		return os << static_cast <const splay_tree::tree &> (tree);
	}

	template <typename U>
	requires std::convertible_to <U, T>
	void insert (U && value) {
		stack <node_link> path = get_link_stack (value);
		if (nullptr == * path.top ()) {
			tree::insert_at (path.top (), value);
		}
		splay (std::move (path));
	}

	bool contains (const T & value) const {
		// return const_cast <splay_tree *> (this)->contains (value);
		return tree::contains (value);
	}

	bool contains (const T & value) {
		stack <node_link> path = get_link_stack (value);
		bool c = * path.top () != nullptr;

		if (false == c) {
			path.pop ();

			if (false == path.empty ()) {
				splay (std::move (path));
			}
		}
		else {
			splay (std::move (path));
		}

		return c;
	}

	void remove (const T & value) {
		stack <node_link> path = get_link_stack (value);

		if (nullptr != * path.top ()) {
			remove_at (path.top ());
		}

		path.pop ();

		if (false == path.empty ()) {
			splay (std::move (path));
		}
	}

protected:
	void splay (stack <node_link> && path) {
		node_link current = path.top (); // path always contains & m_root
		path.pop ();

		if (nullptr == * current || true == path.empty ()) {
			return;
		}

		node_link parent = path.top ();
		path.pop ();

		node_link grandparent;

		if (false == path.empty ()) {
			grandparent = path.top ();
			path.pop ();
		}
		else {
			grandparent = nullptr;
		}

		bool currentIsParentLeft = * current == (* parent)->left;

		while (true) {
			bool parentIsGrandParentLeft;

			node * c = * current;
			node * p = * parent;

			if (nullptr == grandparent) {
				if (true == currentIsParentLeft) {
					p->left = c->right;
					c->right = p;
					* parent = c;
				}
				else {
					p->right = c->left;
					c->left = p;
					* parent = c;
				}

				break;
			}
			else {
				parentIsGrandParentLeft = * parent == (* grandparent)->left;

				node * g = * grandparent; (void) g;

				if (true == currentIsParentLeft) {
					if (true == parentIsGrandParentLeft) {
						g->left = p->right;
						p->left = c->right;
						p->right = g;
						c->right = p;
						* grandparent = c;
					}
					else {
						g->right = c->left;
						p->left = c->right;
						c->left = g;
						c->right = p;
						* grandparent = c;
					}
				}
				else {
					if (false == parentIsGrandParentLeft) {
						g->right = p->left;
						p->right = c->left;
						p->left = g;
						c->left = p;
						* grandparent = c;
					}
					else {
						g->left = c->right;
						p->right = c->left;
						c->left = p;
						c->right = g;
						* grandparent = c;
					}
				}

				if (false == path.empty ()) {
					current = grandparent;

					parent = path.top ();
					path.pop ();

					if (false == path.empty ()) {
						grandparent = path.top ();
						path.pop ();
					}
					else {
						grandparent = nullptr;
					}

					currentIsParentLeft = * current == (* parent)->left;
				}
				else {
					break;
				}
			}
		}

		return;
	}

protected:
	using tree::insert_at;
	using tree::remove_at;
	using tree::at;
	using tree::get_link_stack;
	using tree::get_link_stack_to_leftmost;
	using tree::get_link_stack_to_rightmost;
	using tree::get_link_to_leftmost;
	using tree::get_link_to_rightmost;
};

# endif // SPLAY_TREE_23
