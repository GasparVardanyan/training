# ifndef EXPRESSION_TREE_H_23
# define EXPRESSION_TREE_H_23

# include <string>

# include "22_config.h"
# include "22_notation_converter.h"
# include "23_binary_node.h"

struct ExpressionTree : protected NotationConverter {
	static binary_node <std::string> * PostfixToTree (const std::string & s) {
		MyQueue <Token> tokens = Tokenize (s);
		MyStack <binary_node <std::string> *> nodes;

		while (false == tokens.empty ()) {
			Token t = tokens.front ();
			tokens.pop ();

			if (TokenType::Operand == t.type) {
				nodes.push (new binary_node <std::string> (t.value));
			}
			else if (TokenType::Operator == t.type) {
				auto o2 = nodes.top ();
				nodes.pop ();
				auto o1 = nodes.top ();
				nodes.pop ();
				nodes.push (new binary_node <std::string> (t.value, o1, o2));
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

	static std::string TreeToInfix (const binary_node <std::string> * tree) {
		auto * tree_copy = new binary_node <std::string> (* tree);

		MyStack <binary_node <std::string> *> nodes;
		MyStack <binary_node <std::string> *> parents;

		tree_copy->level_order_traverse ([&nodes, &parents] (binary_node <std::string> * n, binary_node <std::string> * p) -> void {
			nodes.push (n);
			parents.push (p);
		});

		std::string s;

		while (false == nodes.empty ()) {
			auto c = nodes.top ();
			nodes.pop ();
			auto p = parents.top ();
			parents.pop ();

			bool needParen = false;

			if (nullptr != c->left && nullptr != c->right) {
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

		s = tree_copy->data;

		delete tree_copy;

		return s;
	}
};

# endif // EXPRESSION_TREE_H_23
