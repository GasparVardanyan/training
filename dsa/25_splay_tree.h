# ifndef SPLAY_TREE_23
# define SPLAY_TREE_23

# include "23_binary_search_tree.h"

template <typename T, typename Comparator = std::less <T>, bool KeepInvariant = false, bool RemovePreserveLeft = false>
requires std::strict_weak_order <Comparator, T, T>
class splay_tree
	: public detail::binary_search_tree_base <
		splay_tree <T, Comparator, KeepInvariant, RemovePreserveLeft>,
		T, Comparator, KeepInvariant, RemovePreserveLeft
	>
{
public:
	using tree = detail::binary_search_tree_base <
		splay_tree <T, Comparator, KeepInvariant, RemovePreserveLeft>,
		T, Comparator, KeepInvariant, RemovePreserveLeft
	>;

	friend tree;

	using node = tree::node;
	using node_link = tree::node_link;
	using const_node_link = tree::const_node_link;
	using value_type = T;

protected:
	const_node_link get_link (const T & value) const {
		return const_cast <splay_tree *> (this)->get_link (value);
	}

	tree::node_link get_link (const T & value) {
		return tree::get_link (value);
	}

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

				bool zigzag = false;

				node * g = * grandparent; (void) g;

				if (true == currentIsParentLeft) {
					if (true == parentIsGrandParentLeft) {
						p->left = c->right;
						c->right = p;
						* parent = c;
					}
					else {
						g->right = c->left;
						p->left = c->right;
						c->left = g;
						c->right = p;
						* grandparent = c;
						zigzag = true;
					}
				}
				else {
					if (false == parentIsGrandParentLeft) {
						p->right = c->left;
						c->left = p;
						* parent = c;
					}
					else {
						g->left = c->right;
						p->right = c->left;
						c->left = p;
						c->right = g;
						* grandparent = c;
						zigzag = true;
					}
				}

				if (true == zigzag) {
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
				else {
					current = parent;
					parent = grandparent;

					if (false == path.empty ()) {
						grandparent = path.top ();
						path.pop ();
					}
					else {
						grandparent = nullptr;
					}

					// currentIsParentLeft remains the same
				}
			}
		}

		return;
	}

protected:
	using tree::at;
	using tree::get_link_stack;
	using tree::get_link_stack_to_leftmost;
	using tree::get_link_stack_to_rightmost;
	using tree::get_link_to_leftmost;
	using tree::get_link_to_rightmost;
};

# endif // SPLAY_TREE_23
