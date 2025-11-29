# include <iostream>
# include <iomanip>

void matrices ()
{
	int arr [5] = {0}; // NOTE: not a valid C
	if (arr [0]) {}

	int const n_rows = 3;
	int const n_cols = 7;

	int m [n_rows] [n_cols] = {
		{  1,   2,   3,   4,   5,   6,   7, },
		{  8,   9,   10,  11,  12,  13,  14, },
		{  15,  16,  17,  18,  19,  20,  21, },
	};

	for (int y = 0; y < n_rows; y++) {
		for (int x = 0; x < n_cols; x++) {
			std::cout << std::setw (4) << m [y] [x] << std::endl;
		}
	}
}

int main ()
{
	matrices ();
}
