# include <iostream>

# include "23_binary_node.h"
# include "23_expression_tree.h"
# include "23_binary_search_tree.h"

template <typename T>
using node = binary_node <T>;

int main () {
	std::string res = "(6 * (5 + 3 + 2)) * 10 / 2 - 42";
	std::cout << res << std::endl;

	res = NotationConverter::InfixToPostfix_ShuntingYard (res);
	res.resize (res.find ('='));

	std::cout << res << std::endl;

	auto tree = ExpressionTree::PostfixToTree (res);

	std::string s = ExpressionTree::TreeToInfix (tree);

	std::cout << s << std::endl;

	delete tree;


	binary_search_tree <int> bst1;
	binary_search_tree <vector <std::string>, [] (const vector <std::string> & a, const vector <std::string> & b) -> bool { return a.size () < b.size (); } > bst2;
	(void) bst1;
	(void) bst2;
}
