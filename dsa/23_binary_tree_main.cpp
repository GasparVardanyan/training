# include <functional>
# include <iostream>
# include <sstream>

# include "23_binary_tree_node.h"
# include "23_expression_tree.h"
# include "23_binary_search_tree.h"
# include "24_avl_tree.h"

template <typename T>
using node = binary_tree_node <T>;

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

	// avl_tree<int> bst;
	binary_search_tree <int> bst;
	int x = 10;
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (x);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (20);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (5);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (25);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (15);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (5);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (2);
	std::cout << bst.contains (10)
		<< " " << bst.contains (20)
		<< " " << bst.contains (5)
		<< " " << bst.contains (25)
		<< " " << bst.contains (15) << std::endl;
	bst.insert (7);
	bst.insert (1);
	bst.insert (3);

	bst.remove (5); // 10, 20, 25, 15, 5
	std::stringstream ss;
	ss << "TREE:\n" << bst << std::endl;
	std::cout << ss.str () << std::endl;
	for (int i : (vector <int>) bst) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	std::cout << bst.findMin ()->data << " - " << bst.findMax ()->data << std::endl;
	// const int y = 20;
	// bst1.insert(y);
}
