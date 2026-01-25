# ifndef BINARY_TREE_NODE_H_23
# define BINARY_TREE_NODE_H_23

# include <concepts>
# include <ostream>
# include <type_traits>
# include <utility>

# include "22_queue.h"
# include "22_stack.h"

template <typename T>
struct binary_tree_node {
public:
	T data;
	binary_tree_node * left;
	binary_tree_node * right;

	explicit binary_tree_node (const T & data, binary_tree_node * left = nullptr, binary_tree_node * right = nullptr)
		: data (data), left (left), right (right)
	{}

	explicit binary_tree_node (T && data, binary_tree_node * left = nullptr, binary_tree_node * right = nullptr)
		: data (std::move (data)), left (left), right (right)
	{}

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

	binary_tree_node (binary_tree_node && other) noexcept
		: data (
			(std::is_move_constructible_v <T> ? std::move (other.data) : other.data)
		)
		, left (other.left)
		, right (other.right)
	{
		other.left = nullptr;
		other.right = nullptr;
	}

	binary_tree_node & operator= (binary_tree_node && other) noexcept {
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
	template <typename F>
	requires requires (F f, binary_tree_node * n) { f (n, n); }
	void preorder_traverse (F && func) {
		preorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, binary_tree_node * n) { f (n, n); }
	void inorder_traverse (F && func) {
		inorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, binary_tree_node * n) { f (n, n); }
	void postorder_traverse (F && func) {
		postorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, binary_tree_node * n) { f (n, n); }
	void level_order_traverse (F && func) {
		level_order_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, binary_tree_node * n) { f (n, n, std::size_t {}); }
	void level_order_traverse (F && func) {
		level_order_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_tree_node * n) { f (n, n); }
	void preorder_traverse (F && func) const {
		preorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_tree_node * n) { f (n, n); }
	void inorder_traverse (F && func) const {
		inorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_tree_node * n) { f (n, n); }
	void postorder_traverse (F && func) const {
		postorder_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_tree_node * n) { f (n, n); }
	void level_order_traverse (F && func) const {
		level_order_traverse (this, func);
	}

	template <typename F>
	requires requires (F f, const binary_tree_node * n) { f (n, n, std::size_t {}); }
	void level_order_traverse (F && func) const {
		level_order_traverse (this, func);
	}


	friend std::ostream & operator<< (std::ostream & os, const binary_tree_node & node)
		requires requires (std::ostream os, T t) {
			{ os << t } -> std::convertible_to <std::ostream &>;
		}
	{
		stack <const binary_tree_node *> path;

		node.preorder_traverse (
			[&path, &os]
			(const binary_tree_node * node, const binary_tree_node * parent) -> void {
				if (nullptr != parent) {
					while (path.top () != parent) {
						path.pop ();
					}
				}
				path.push (node);

				std::size_t depth = path.size ();

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

private:
	template <typename U, typename F>
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
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
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
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
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
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
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
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

	template <typename U, typename F>
	   requires std::same_as <std::remove_cv_t <std::remove_pointer_t <U>>, binary_tree_node>
	&& requires (F f, U u) {
		f (u, u, std::size_t {});
	}
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
				nodes = nodes_next;
				parents = parents_next;
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
