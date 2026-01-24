# ifndef BINARY_NODE_H_23
# define BINARY_NODE_H_23

# include <concepts>
# include <type_traits>
# include <utility>

# include "22_queue.h"

template <typename T>
struct binary_node {
public:
	T data;
	binary_node * left;
	binary_node * right;

	explicit binary_node (const T & data, binary_node * left = nullptr, binary_node * right = nullptr)
		: data (data), left (left), right (right)
	{}

	explicit binary_node (T && data, binary_node * left = nullptr, binary_node * right = nullptr)
		: data (std::move (data)), left (left), right (right)
	{}

	binary_node (const binary_node & other)
		: data (other.data)
		, left (nullptr)
		, right (nullptr)
	{
		queue <binary_node *> our;
		our.push (this);
		queue <const binary_node *> their;
		their.push (& other);

		while (false == their.empty ()) {
			binary_node * o = our.front ();
			our.pop ();
			const binary_node * t = their.front ();
			their.pop ();

			if (t->left != nullptr) {
				o->left = new binary_node (t->left->data);
				our.push (o->left);
				their.push (t->left);
			}

			if (nullptr != t->right) {
				o->right = new binary_node (t->right->data);
				our.push (o->right);
				their.push (t->right);
			}
		}
	}

	binary_node & operator= (const binary_node & other) {
		if (this != & other) {
			binary_node copy (other);
			std::swap (* this, copy);
		}
		return * this;
	}

	binary_node (binary_node && other) noexcept
		: data (
			(std::move_constructible <T> ? std::move (other.data) : other.data)
		)
		, left (other.left)
		, right (other.right)
	{
		other.left = nullptr;
		other.right = nullptr;
	}

	binary_node & operator= (binary_node && other) noexcept {
		if (this != & other) {
			std::swap (data, other.data);
			std::swap (left, other.left);
			std::swap (right, other.right);
		}

		return * this;
	}

	~binary_node () {
		queue <binary_node *> to_delete;

		if (nullptr != left) {
			to_delete.push (left);
			left = nullptr;
		}
		if (nullptr != right) {
			to_delete.push (right);
			right = nullptr;
		}

		while (false == to_delete.empty ()) {
			binary_node * n = to_delete.front ();
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

	template <typename F>
	requires requires (F f, binary_node * n) { f (n, n); }
	void preorder_traverse (F && func) {
		preorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, binary_node * n) { f (n, n); }
	void inorder_traverse (F && func) {
		inorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, binary_node * n) { f (n, n); }
	void postorder_traverse (F && func) {
		postorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, binary_node * n) { f (n, n); }
	void level_order_traverse (F && func) {
		level_order_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_node * n) { f (n, n); }
	void preorder_traverse (F && func) const {
		preorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_node * n) { f (n, n); }
	void inorder_traverse (F && func) const {
		inorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_node * n) { f (n, n); }
	void postorder_traverse (F && func) const {
		postorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_node * n) { f (n, n); }
	void level_order_traverse (F && func) const {
		level_order_traverse (this, func);
	}

private:
	template <typename U, typename F>
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_node>
	&& requires (F f, U u) {
		f (u, u);
	}
	static void preorder_traverse (U node, F && func, U parent = nullptr) {
		func (node, parent);

		if (nullptr != node->left) {
			preorder_traverse ((U) node->left, func, node);
		}

		if (nullptr != node->right) {
			preorder_traverse ((U) node->right, func, node);
		}
	}

	template <typename U, typename F>
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_node>
	&& requires (F f, U u) {
		f (u, u);
	}
	static void inorder_traverse (U node, F && func, U parent = nullptr) {
		if (nullptr != node->left) {
			inorder_traverse ((U) node->left, func, node);
		}

		func (node, parent);

		if (nullptr != node->right) {
			inorder_traverse ((U) node->right, func, node);
		}
	}

	template <typename U, typename F>
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_node>
	&& requires (F f, U u) {
		f (u, u);
	}
	static void postorder_traverse (U node, F && func, U parent = nullptr) {
		if (nullptr != node->left) {
			postorder_traverse ((U) node->left, func, node);
		}

		if (nullptr != node->right) {
			postorder_traverse ((U) node->right, func, node);
		}

		func (node, parent);
	}

	template <typename U, typename F>
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_node>
	&& requires (F f, U u) {
		f (u, u);
	}
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
};

# endif // BINARY_NODE_H_23
