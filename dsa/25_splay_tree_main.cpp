# include <iostream>

# include "25_splay_tree.h"

int main () {
	splay_tree <int> spt;
	spt.insert (2);
	spt.insert (3);
	spt.insert (4);
	spt.insert (5);
	spt.insert (6);
	spt.insert (7);
	spt.insert (1);

	std::cout << spt << std::endl;

	spt.make_empty ();

	for (int i = 1; i <= 32; i++) {
		spt.insert (i);
	}

	for (int i = 1; i <= 9; i++) {
		spt.contains (i);
	}

	std::cout << spt << std::endl;
}
