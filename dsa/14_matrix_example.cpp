# include <iostream>

# include "14_matrix.h"

int main ()
{
	Matrix <int> m {4, 3};
	std::cout << "hello " << m.numrows () << ", " << m.numcols () << std::endl;
}
