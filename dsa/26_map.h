# ifndef MAP_H_26
# define MAP_H_26

# include "23_binary_search_tree.h"
# include "23_binary_tree_node.h"
# include "24_avl_tree.h"
# include <type_traits>

// namespace detail {
// template <typename T, typename C, typename Data = std::monostate, typename = void>
// struct map__ {
// 	static constexpr bool container_verified = false;
// };
//
// template <typename T, template <typename> typename C, typename Data>
// struct map__ <T, C <T>, Data, std::enable_if_t <is_tree_v <C <T>>>> {
// 	static constexpr bool container_verified = true;
//
// 	struct node_data : Data {
// 		static_assert (false == requires (Data d) { d.key; }, "the 'key' member of Data is reserved");
// 		static_assert (false == requires (Data d) { d.value; }, "the 'value' member of Data is reserved");
// 	};
// };
// }

namespace detail {
template <
	typename Kt,
	typename Vt,
	typename Data = std::monostate
>
struct map_node_data__ : Data {
	static_assert (false == requires (Data d) { d.key; }, "the 'key' member of Data is reserved");
	static_assert (false == requires (Data d) { d.value; }, "the 'value' member of Data is reserved");

	Kt key;
	Vt value;

	template <std::size_t I>
	struct node_data_type;

	template <>
	struct node_data_type <0> {
		using type = Kt;
	};

	template <>
	struct node_data_type <1> {
		using type = Vt;
	};

	template <typename Kt_, typename Vt_>
	requires std::is_constructible_v <Kt, Kt_> && std::is_constructible_v <Vt, Vt_>
	explicit map_node_data__ (Kt_ && key, Vt_ && value)
		: key (std::forward <Kt_> (key)), value (std::forward <Vt_> (value))
	{}

	friend std::ostream & operator<< (std::ostream & os, const map_node_data__ & data) {
		os << '{' << data.key << ", " << data.value << '}';
		return os;
	}

	bool operator< (const map_node_data__ & other) const {
		return key < other.key;
	}

	bool operator== (const map_node_data__ & other) const {
		return key == other.key;
	}

	template <std::size_t I>
	requires (2 > I)
	friend constexpr const node_data_type <I>::type & get (const map_node_data__ & nd) noexcept {
		if constexpr (0 == I) {
			return nd.key;
		}
		else {
			return nd.value;
		}
	}

	template <std::size_t I>
	requires (2 > I)
	friend constexpr node_data_type <I>::type & get (map_node_data__ & nd) noexcept {
		if constexpr (0 == I) {
			return nd.key;
		}
		else {
			return nd.value;
		}
	}

	template <std::size_t I>
	requires (2 > I)
	friend constexpr node_data_type <I>::type && get (map_node_data__ && nd) noexcept {
		if constexpr (0 == I) {
			return std::move (nd.key);
		}
		else {
			return std::move (nd.value);
		}
	}
};

template <
	typename Kt,
	typename Vt,
	typename Comparator = std::less <Kt>,
	template <typename, typename> typename Container = binary_search_tree,
	typename Data = std::monostate
>
struct map__ {
	using node_data = map_node_data__ <Kt, Vt, Data>;

	struct less_than {
		static constexpr Comparator comparator {};

		bool operator () (const node_data & first, const node_data & second) const {
			return comparator (first.key, second.key);
		}

		bool operator () (const Kt & first, const Kt & second) const {
			return comparator (first, second);
		}

		bool operator () (const Kt & key, const node_data & node) const {
			return comparator (key, node.key);
		}

		bool operator () (const node_data & node, const Kt & key) const {
			return comparator (node.key, key);
		}
	};

	using tree = Container <node_data, less_than>;

	// static_assert (is_tree_v <tree>, "Container of map isn't a tree");
};
}

template <
	typename Kt,
	typename Vt,
	typename Comparator = std::less <Kt>,
	template <typename, typename> typename Container = binary_search_tree,
	typename Data = std::monostate
>
requires std::strict_weak_order <Comparator, Kt, Kt>
struct map : protected detail::map__ <Kt, Vt, Comparator, Container, Data>::tree {
protected:
	using detail = detail::map__ <Kt, Vt, Comparator, Container, Data>;
public:
	using tree = detail::tree;
	using node = tree::node;
	using node_link = tree::node_link;
	using const_node_link = tree::const_node_link;
	using node_data = detail::node_data;
	using key_type = Kt;
	using value_type = Vt;
	using tree::less_than;
	using iterator = tree::iterator;
	using const_iterator = tree::const_iterator;

public:
	using tree::dump_invariant;
	using tree::dump_sorted;
	using tree::make_empty;
	using tree::root;
	using tree::size;
	using tree::empty;
	using tree::internal_path_length;
	// using tree::insert;
	// using tree::remove;
	using tree::contains;

	using tree::begin;
	using tree::cbegin;
	using tree::end;
	using tree::cend;
	using tree::find;

	bool operator== (const map & other) const {
		return
			   static_cast <const tree &> (* this)
			== static_cast <const tree &> (other)
		;
	}

	void dump_sorted (std::output_iterator <std::pair <Kt, Vt>> auto it) const {
		if (nullptr != this->m_root) {
			this->m_root->inorder_traverse (
				[&it] (const node * n, const node *) -> void {
					*it++ = { n->key, n->value };
				}
			);
		}
	}

	operator vector <std::pair <Kt, Vt>> () const {
		vector <std::pair <Kt, Vt>> v;
		v.reserve (size ());

		dump_sorted (std::back_inserter (v));

		return v;
	}

	friend std::ostream & operator<< (std::ostream & os, const map & tree)
		requires requires (std::ostream & os, Kt k, Vt v) {
			{ os << k } -> std::convertible_to <std::ostream &>;
			{ os << v } -> std::convertible_to <std::ostream &>;
		}
	{
		return os << static_cast <const map::tree &> (tree);
	}

public:
	template <typename Kt_, typename Vt_>
	requires std::convertible_to <Kt_, Kt> && std::convertible_to <Vt_, Vt>
	bool insert (Kt_ && key, Vt_ && value) {
		bool inserting = false;

		if (false == contains (key)) {
			this->tree::insert (node_data (std::forward <Kt_> (key), std::forward <Vt_> (value)));
			inserting = true;
		}	// after move constructing value we can't use it to get an iterator,
			// so insert should return one.

		return inserting;
	}
};

namespace std {

template <typename Kt, typename Vt, typename Data>
struct tuple_size <typename detail::map_node_data__ <Kt, Vt, Data>>
	: std::integral_constant <std::size_t, 2> {};

template <typename Kt, typename Vt, typename Data>
struct tuple_element <0, detail::map_node_data__ <Kt, Vt, Data>> {
	using type = Kt;
};

template <typename Kt, typename Vt, typename Data>
struct tuple_element <1, detail::map_node_data__ <Kt, Vt, Data>> {
	using type = Vt;
};
}

# endif // MAP_H_26
