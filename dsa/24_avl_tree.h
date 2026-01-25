# ifndef AVL_TREE_H_24
# define AVL_TREE_H_24

# include <concepts>
# include <cstddef>
# include <functional>
# include <iostream>
# include <iterator>
# include <ostream>

# include "23_binary_search_tree.h"

namespace detail {
template <typename T, typename C>
struct avl_tree__ {
	struct node_data {
		T value;
		std::ptrdiff_t level;

		friend std::ostream & operator<< (std::ostream & os, const node_data & data) {
			os << data.value;
			return os;
		}

		operator T () const {
			return value;
		}
	};

	struct compare_func {
		const C value_comparator;
		constexpr bool operator() (const node_data & a, const node_data & b) const {
			return value_comparator (a.value, b.value);
		}
	};

	using tree = binary_search_tree <node_data, compare_func>;
};
}

template <typename T, typename Comparator = std::less <T>>
requires std::strict_weak_order <Comparator, T, T>
class avl_tree : protected detail::avl_tree__ <T, Comparator>::tree {
public:

public:
	using tree = detail::avl_tree__ <T, Comparator>::tree;
	using data = detail::avl_tree__ <T, Comparator>::node_data;
	using node = tree::node;

public:
	template <typename U>
	requires std::convertible_to <U, T>
	void insert (U && value) {
		this->tree::insert (data {.value = value, .level = -1});
	}

	void remove (const T & value) {
		this->tree::remove (data {.value = value, .level = -1});
		// custom impl soon
	}

private:
	void rebalance (const stack <node> & path) {
		(void) path;
		// impl soon
	}

public:
	bool contains (const T & value) const {
		return this->tree::contains ({.value = value, .level=-1});
	}

	friend std::ostream & operator<< (std::ostream & os, const avl_tree & tree)
		requires requires (std::ostream & os, T t) {
			{ os << t } -> std::convertible_to <std::ostream &>;
		}
	{
		if (nullptr != tree.root () ) {
			os << * tree.root ();
		}
		return os;
	}

	operator vector <T> () const {
		// vector <data> d = * this;
		vector <T> v;
		v.reserve(size ());

		// std::copy (d.cbegin (), d.cend (), v.begin ());
		dump (std::back_inserter(v));

		return v;
	}

	template <typename It>
	requires std::output_iterator <It, T>
	void dump (It it) const {
		this->tree::dump (it);
	}

	const node * findMin () const { return this->tree::findMin (); }
	const node * findMax () const { return this->tree::findMax (); }
	void makeEmpty () { this->tree::makeEmpty (); }
	const node * root () const { return this->tree::root (); }
	std::size_t size () const { return this->tree::size (); }
	bool empty () const { return this->tree::empty (); }

private:
	Comparator m_isLessThan;
};

# endif // AVL_TREE_H_24
