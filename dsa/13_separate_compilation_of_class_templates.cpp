# include <iostream>

# include "13_MemoryCell1.h"
# include "13_MemoryCell2.h"

int main ()
{
	MemoryCell1<int> cell {12};
	std::cout << cell.read () << std::endl;
	cell.write (13);
	std::cout << cell.read () << std::endl;

	MemoryCell2<int> cell2 {12};
	std::cout << cell2.read () << std::endl;
	cell2.write (13);
	std::cout << cell2.read () << std::endl;
}
