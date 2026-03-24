# ifndef BINARY_TREE_NODE_H_23
# define BINARY_TREE_NODE_H_23

# include <concepts>
# include <cstddef>
# include <functional>
# include <iostream>
# include <ostream>
# include <type_traits>
# include <utility>

# include "20_vector.h"
# include "22_queue.h"
# include "22_stack.h"

template <typename T, std::equivalence_relation <T, T> EqualTo = std::equal_to <T>>
struct binary_tree_node {
private:
	template <typename NPT>
	class iterator_base {
		friend struct binary_tree_node;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using iterator_category = std::bidirectional_iterator_tag;

	private:
		using _reference_t = std::conditional_t <std::is_const_v <std::remove_pointer_t <NPT>>, const T &, T &>;
		using _node_ptr_t = std::conditional_t <std::is_const_v <std::remove_pointer_t <NPT>>, const binary_tree_node *, binary_tree_node *>;
		using _non_cost_iterator_t = iterator_base <std::remove_pointer_t <std::remove_const_t <NPT>>> *;

	public:
		friend class iterator_base <const std::decay_t <std::remove_pointer_t <NPT>> *>;

		explicit iterator_base (NPT node = nullptr)
			: m_node (node)
		{}

		template <
			typename NPT2,
			typename = std::enable_if_t <
				std::is_same_v <NPT2, NPT> || std::is_same_v <std::remove_const_t <std::remove_pointer_t <NPT>> *, NPT2>
			>
		>
		// template <typename NPT2>
		// requires (std::same_as <NPT2, NPT> || std::same_as <std::remove_const_t <std::remove_pointer_t <NPT>> *, NPT2>)
		iterator_base (const iterator_base <NPT2> & other)
			: m_node (other.m_node)
		{}

		// iterator_base (const iterator_base &) = delete;
		// iterator_base (iterator_base &&) = delete;
		// iterator_base & operator= (const iterator_base &) = delete;
		// iterator_base & operator= (iterator_base &&) = delete;

		_reference_t & operator* () {
			return static_cast <_node_ptr_t> (m_node)->data;
		}

		const _reference_t & operator* () const {
			return static_cast <_node_ptr_t> (m_node)->data;
		}

		template <
			typename NPT2,
			typename = std::enable_if_t <
				true == std::is_same_v <
					std::remove_const_t <std::remove_pointer_t <NPT>>,
					std::remove_const_t <std::remove_pointer_t <NPT2>>
				>
			>
		>
		bool operator== (const iterator_base <NPT2> & other) const {
			return m_node == other.m_node;
		}

		iterator_base & operator++ () {
			* this = * m_next;
			return * this;
		}

		iterator_base operator++ (int) {
			iterator_base before = * this;
			* this = * m_next;
			return before;
		}

		iterator_base & operator-- () {
			* this = * m_prev;
			return * this;
		}

		iterator_base operator-- (int) {
			iterator_base before = * this;
			* this = * m_prev;
			return before;
		}

		NPT base () {
			return m_node;
		}

	private:
		NPT m_node;
		iterator_base * m_prev = nullptr;
		iterator_base * m_next = nullptr;
	};

public:
	using equivalence_relation = EqualTo;
	static constexpr equivalence_relation equal_to {};

	using iterator = iterator_base <binary_tree_node *>;
	using const_iterator = iterator_base <const binary_tree_node *>;

public:
	T data;
	binary_tree_node * left;
	binary_tree_node * right;

public:
	explicit binary_tree_node (const T & data, binary_tree_node * left = nullptr, binary_tree_node * right = nullptr)
		: data (data), left (left), right (right)
	{}

	explicit binary_tree_node (T && data, binary_tree_node * left = nullptr, binary_tree_node * right = nullptr)
		: data (std::move (data)), left (left), right (right)
	{}

public:
	binary_tree_node (const binary_tree_node & other)
		: data (other.data)
		, left (nullptr)
		, right (nullptr)
	{
		queue <binary_tree_node *> our;
		our.push (this);
		queue <const binary_tree_node *> their;
		their.push (& other);

		while (false == their.empty ()) {
			binary_tree_node * o = our.front ();
			our.pop ();
			const binary_tree_node * t = their.front ();
			their.pop ();

			if (t->left != nullptr) {
				o->left = new binary_tree_node (t->left->data);
				our.push (o->left);
				their.push (t->left);
			}

			if (nullptr != t->right) {
				o->right = new binary_tree_node (t->right->data);
				our.push (o->right);
				their.push (t->right);
			}
		}
	}

	binary_tree_node & operator= (const binary_tree_node & other) {
		if (this != & other) {
			binary_tree_node copy (other);
			std::swap (* this, copy);
		}
		return * this;
	}

	binary_tree_node (binary_tree_node && other)
		noexcept (std::is_nothrow_move_constructible_v <T>)
		: data (std::move (other.data))
		, left (other.left)
		, right (other.right)
	{
		other.left = nullptr;
		other.right = nullptr;
	}

	binary_tree_node & operator= (binary_tree_node && other)
		noexcept (std::is_nothrow_swappable_v <T>)
	{
		if (this != & other) {
			std::swap (data, other.data);
			std::swap (left, other.left);
			std::swap (right, other.right);
		}

		return * this;
	}

	~binary_tree_node () {
		queue <binary_tree_node *> to_delete;

		if (nullptr != left) {
			to_delete.push (left);
			left = nullptr;
		}
		if (nullptr != right) {
			to_delete.push (right);
			right = nullptr;
		}

		while (false == to_delete.empty ()) {
			binary_tree_node * n = to_delete.front ();
			to_delete.pop ();
			if (nullptr != n->left) {
				to_delete.push (n->left);
				n->left = nullptr;
			}
			if (nullptr != n->right) {
				to_delete.push (n->right);
				n->right = nullptr;
			}

			delete n;
		}
	}

public:
	bool operator== (const binary_tree_node & other) const {
		stack <const binary_tree_node *> our;
		our.push (this);
		stack <const binary_tree_node *> their;
		their.push (& other);

		bool equal = true;

		while (false == our.empty ()) {
			const binary_tree_node * o = our.top ();
			our.pop ();
			const binary_tree_node * t = their.top ();
			their.pop ();

			if (false == equal_to (o->data, t->data)) {
				equal = false;
				break;
			}

			const binary_tree_node * o_l = o->left;
			const binary_tree_node * o_r = o->right;
			const binary_tree_node * t_l = t->left;
			const binary_tree_node * t_r = t->right;

			const bool o_l_null = o_l == nullptr;
			const bool o_r_null = o_r == nullptr;
			const bool t_l_null = t_l == nullptr;
			const bool t_r_null = t_r == nullptr;

			if (o_l_null != t_l_null || o_r_null != t_r_null) {
				equal = false;
				break;
			}

			if (false == o_l_null) {
				our.push (o_l);
				their.push (t_l);
			}

			if (false == o_r_null) {
				our.push (o_r);
				their.push (t_r);
			}
		}

		return equal;
	}

	friend std::ostream & operator<< (std::ostream & os, const binary_tree_node & node)
		requires requires (std::ostream os, T t) {
			{ os << t } -> std::convertible_to <std::ostream &>;
		}
	{
		node.preorder_traverse (
			[path = stack <const binary_tree_node *> (), &os]
			(const binary_tree_node * node, const binary_tree_node * parent) mutable -> void {
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

				os << '\n';
			}
		);

		return os;
	}

public:
	void generate_iterators (vector <iterator *> & iterators) {
		// iterators.clear ();

		inorder_traverse ([&iterators] (binary_tree_node * node, binary_tree_node *) -> void {
			iterators.push_back (new iterator (node));
		});

		iterators.push_back (new iterator (nullptr));

		iterator * prev = nullptr;

		for (iterator * it : iterators) {
			if (nullptr != prev) {
				prev->m_next = it;
			}
			it->m_prev = prev;

			prev = it;
		}
	}

	template <typename F>
	requires std::invocable <F &, binary_tree_node *, binary_tree_node *>
	void preorder_traverse (F && func) {
		preorder_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, binary_tree_node *, binary_tree_node *>
	void inorder_traverse (F && func) {
		inorder_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, binary_tree_node *, binary_tree_node *>
	void postorder_traverse (F && func) {
		postorder_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, binary_tree_node *, binary_tree_node *>
	void level_order_traverse (F && func) {
		level_order_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, binary_tree_node *, binary_tree_node *, std::size_t>
	void level_order_traverse (F && func) {
		level_order_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, const binary_tree_node *, const binary_tree_node *>
	void preorder_traverse (F && func) const {
		preorder_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, const binary_tree_node *, const binary_tree_node *>
	void inorder_traverse (F && func) const {
		inorder_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, const binary_tree_node *, const binary_tree_node *>
	void postorder_traverse (F && func) const {
		postorder_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, const binary_tree_node *, const binary_tree_node *>
	void level_order_traverse (F && func) const {
		level_order_traverse (this, func);
	}

	template <typename F>
	requires std::invocable <F &, const binary_tree_node *, const binary_tree_node *, std::size_t>
	void level_order_traverse (F && func) const {
		level_order_traverse (this, func);
	}

private:
	template <typename U, typename F>
	requires
		   std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
		&& std::invocable <F &, U, U>
	static void preorder_traverse (U node, F && func) {
		stack <U> nodes;
		stack <U> parents;
		nodes.push (node);
		parents.push (nullptr);

		while (false == nodes.empty ()) {
			U n = nodes.top ();
			nodes.pop ();
			U p = parents.top ();
			parents.pop ();

			func (n, p);

			if (nullptr != n->right) {
				nodes.push (n->right);
				parents.push (n);
			}
			if (nullptr != n->left) {
				nodes.push (n->left);
				parents.push (n);
			}
		}
	}

	template <typename U, typename F>
	requires
		   std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
		&& std::invocable <F &, U, U>
	static void inorder_traverse (U node, F && func, U parent = nullptr) {
		if (nullptr != node->left) {
			inorder_traverse (static_cast <U> (node->left), func, node);
		}

		func (node, parent);

		if (nullptr != node->right) {
			inorder_traverse (static_cast <U> (node->right), func, node);
		}
	}

	template <typename U, typename F>
	requires
		   std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
		&& std::invocable <F &, U, U>
	static void postorder_traverse (U node, F && func, U parent = nullptr) {
		if (nullptr != node->left) {
			postorder_traverse (static_cast <U> (node->left), func, node);
		}

		if (nullptr != node->right) {
			postorder_traverse (static_cast <U> (node->right), func, node);
		}

		func (node, parent);
	}

	template <typename U, typename F>
	requires
		   std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
		&& std::invocable <F &, U, U>
	static void level_order_traverse (U node, F && func, U parent = nullptr) {
		queue <U> nodes;
		nodes.push (node);
		queue <U> parents;
		parents.push (parent);

		while (false == nodes.empty ()) {
			U n = nodes.front ();
			nodes.pop ();
			U p = parents.front ();
			parents.pop ();

			if (nullptr != n->left) {
				nodes.push (n->left);
				parents.push (n);
			}

			if (nullptr != n->right) {
				nodes.push (n->right);
				parents.push (n);
			}

			func (n, p);
		}
	}

	template <typename U, typename F>
	requires
		   std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
		&& std::invocable <F &, U, U, std::size_t>
	static void level_order_traverse (U node, F && func, U parent = nullptr) {
		queue <U> nodes, nodes_next;
		nodes.push (node);
		queue <U> parents, parents_next;
		parents.push (parent);

		std::size_t level = 0;

		while (true) {
			while (false == nodes.empty ()) {
				U n = nodes.front ();
				nodes.pop ();
				U p = parents.front ();
				parents.pop ();

				if (nullptr != n->left) {
					nodes_next.push (n->left);
					parents_next.push (n);
				}

				if (nullptr != n->right) {
					nodes_next.push (n->right);
					parents_next.push (n);
				}

				func (n, p, level);
			}
			if (false == nodes_next.empty ()) {
				nodes = std::move (nodes_next);
				parents = std::move (parents_next);
				nodes_next = {};
				parents_next = {};
				level++;
			}
			else {
				break;
			}
		}
	}
};

# endif // BINARY_TREE_NODE_H_23
