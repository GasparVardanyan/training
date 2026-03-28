# include <iostream>

# include "25_splay_tree.h"

int main () {
	std::cout << "HELLO" << std::endl;

	splay_tree <int> spt;
	spt.insert (-50);
	spt.insert (50);
	spt.insert (15);
	spt.insert (20);
	spt.insert (10);

	std::cout << spt.contains (16) << std::endl;

	std::cout << spt << std::endl;
}
