# include <iostream>

# include "25_splay_tree.h"

int main () {
	std::cout << "HELLO" << std::endl;

	splay_tree <int> spt;
	spt.insert (10);
	spt.insert (20);
	spt.insert (30);

	std::cout << spt << std::endl;

	std::cout << spt.contains (20) << std::endl;
}
