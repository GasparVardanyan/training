# ifndef EXPRESSION_TREE_H_23
# define EXPRESSION_TREE_H_23

# include <string>
# include <utility>

# include "22_balance_checker.h"
# include "22_config.h"
# include "22_notation_converter.h"
# include "23_binary_tree_node.h"
# include "22_stack.h"

struct ExpressionTree : protected NotationConverter {
	static binary_tree_node <std::string> * PostfixToTree (const std::string & s) {
		MyQueue <Token> tokens = Tokenize (s);
		MyStack <binary_tree_node <std::string> *> nodes;

		while (false == tokens.empty ()) {
			Token t = tokens.front ();
			tokens.pop ();

			if (TokenType::Operand == t.type) {
				nodes.push (new binary_tree_node <std::string> (t.value));
			}
			else if (TokenType::Operator == t.type) {
				auto o2 = nodes.top ();
				nodes.pop ();
				auto o1 = nodes.top ();
				nodes.pop ();
				nodes.push (new binary_tree_node <std::string> (t.value, o1, o2));
			}
		}

		if (1 != nodes.size ()) {
			while (false == nodes.empty ()) {
				auto n = nodes.top ();
				nodes.pop ();
				delete n;
			}
			return nullptr;
		}
		else {
			return nodes.top ();
		}
	}

	static std::string TreeToInfix (const binary_tree_node <std::string> * tree) {
		binary_tree_node <std::string> tree_copy = (* tree);

		MyStack <binary_tree_node <std::string> *> nodes;
		MyStack <binary_tree_node <std::string> *> parents;

		tree_copy.level_order_traverse ([&nodes, &parents] (binary_tree_node <std::string> * n, binary_tree_node <std::string> * p) -> void {
			nodes.push (n);
			parents.push (p);
		});

		while (false == nodes.empty ()) {
			auto c = nodes.top ();
			nodes.pop ();
			auto p = parents.top ();
			parents.pop ();

			if (nullptr != c->left && nullptr != c->right) {
				bool needParen = false;

				if ('+' == c->data [0] || '-' == c->data [0]) {
					if (nullptr != p) {
						if ('*' == p->data [0] || '/' == p->data [0]) {
							needParen = true;
						}
					}
				}

				if (true == needParen) {
					c->data = '(' + c->left->data + ' ' + c->data + ' ' + c->right->data + ')';
				}
				else {
					c->data = c->left->data + ' ' + c->data + ' ' + c->right->data;
				}
			}
		}

		std::string s = tree_copy.data;

		return s;
	}

	static std::string TreeToInfix2 (const binary_tree_node <std::string> * tree) {
		binary_tree_node <std::string> tree_copy (* tree);

		tree_copy.postorder_traverse ([] (
			binary_tree_node <std::string> * node,
			binary_tree_node <std::string> *
		) -> void {
			if (nullptr != node->left && nullptr != node->right) {
				if ('*' == node->data [0] || '/' == node->data [0]) {
					if (true == NeedParenthesis (node->left->data)) {
						node->left->data = '(' + node->left->data + ')';
					}

					if (true == NeedParenthesis (node->right->data)) {
						node->right->data = '(' + node->right->data + ')';
					}
				}

				node->data = node->left->data + ' ' + node->data + ' ' + node->right->data;
			}
		});

		return tree_copy.data;
	}

	static std::string TreeToInfix3 (const binary_tree_node <std::string> * tree) {
		binary_tree_node <std::pair <std::string, std::string>> pair_tree ({
			tree->data, tree->data // orig, result
		});
		stack <decltype (tree)> tree_stack;
		tree_stack.push (tree);
		stack <decltype (pair_tree) *> pair_tree_stack;
		pair_tree_stack.push (& pair_tree);

		while (false == tree_stack.empty ()) {
			auto t = tree_stack.top ();
			tree_stack.pop ();
			auto p = pair_tree_stack.top ();
			pair_tree_stack.pop ();

			if (nullptr != t->left) {
				p->left = new binary_tree_node (
					std::pair <std::string, std::string> (t->left->data, t->left->data)
				);
				tree_stack.push (t->left);
				pair_tree_stack.push (p->left);
			}

			if (nullptr != t->right) {
				p->right = new binary_tree_node (
					std::pair <std::string, std::string> (t->right->data, t->right->data)
				);
				tree_stack.push (t->right);
				pair_tree_stack.push (p->right);
			}
		}

		pair_tree.postorder_traverse ([] (auto * node, auto *) -> void {
			if (nullptr != node->left && nullptr != node->right) {
				if ('*' == node->data.first [0] || '/' == node->data.first [0]) {
					if ('+' == node->left->data.first [0] || '-' == node->left->data.first [0]) {
						node->left->data.second = '(' + node->left->data.second + ')';
					}

					if ('+' == node->right->data.first [0] || '-' == node->right->data.first [0]) {
						node->right->data.second = '(' + node->right->data.second + ')';
					}
				}

				node->data.second = node->left->data.second + ' ' + node->data.first + ' ' + node->right->data.second;
			}
		});

		return pair_tree.data.second;
	}
};

# endif // EXPRESSION_TREE_H_23
