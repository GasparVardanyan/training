# ifndef SPLAY_TREE_23
# define SPLAY_TREE_23

# include "23_binary_search_tree.h"
# include <iostream>

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


protected:
	tree::const_node_link get_link (const T & value) const {
		return const_cast <splay_tree *> (this)->get_link (value);
	}

	tree::node_link get_link (const T & value) {
		return tree::get_link (value);
	}
};

# endif // SPLAY_TREE_23
