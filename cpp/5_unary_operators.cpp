# include <iostream>

int main ()
{
	int a = 1;

	std::cout << (++a) << std::endl; // 2
	std::cout << (a--) << std::endl; // 2

	int minusa = -a;
	std::cout << minusa << std::endl; // -1

	bool b = true;

	std::cout << std::boolalpha;
	std::cout << (!b) << std::endl; // false
	std::cout << std::noboolalpha;

	a = 4;
	std::cout << (a++ / 2) << std::endl; // 2
	std::cout << a << std::endl; // 5
	std::cout << (++a / 2) << std::endl; // 3
	std::cout << a << std::endl; // 6

	std::cout << "==============================" << std::endl;

	int arr [4] = {1, 2, 3, 4};

	int * ptr1 = & arr [0]; // arr
	int * ptr2 = ptr1++; // arr+1, arr

	std::cout << *ptr1++ << std::endl; // 2, ptr1 arr+2

	int e = arr [0]++;
	std::cout << e << std::endl; // 1
	std::cout << (* ptr2) << std::endl; // 2

	int i = 0;
	int x = 0;
	// x = i++ + i++; // Multiple unsequenced modifications to 'i'
	std::cout << x << std::endl;
	std::cout << i << std::endl;
}
