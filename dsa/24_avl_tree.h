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
	using tree::insert;
	using tree::remove;
	using tree::contains;
	using tree::dump;
	using tree::findMin;
	using tree::findMax;
	using tree::makeEmpty;
	using tree::root;
	using tree::size;
	using tree::empty;
	using tree::at;

	friend std::ostream & operator<< (std::ostream & os, const avl_tree & tree)
		requires requires (std::ostream & os, T t) {
			{ os << t } -> std::convertible_to <std::ostream &>;
		}
	{
		return os << static_cast <const avl_tree::tree &> (tree);
	}

	bool operator== (const avl_tree & other) const {
		return
			   static_cast <const avl_tree::tree &> (* this)
			== static_cast <const avl_tree::tree &> (other)
		;
	}

	operator vector <T> () const {
		vector <T> v;
		v.reserve (size ());

		dump (std::back_inserter (v));

		return v;
	}
};

# endif // AVL_TREE_H_24
