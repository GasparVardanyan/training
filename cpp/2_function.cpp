# include <iostream>
# define X

int add2 (int i); // The function is of the type (int) -> (int)
int add2 X(int); // The argument name is optional

int add2 (int, int);

void X do_something ();

int multiply (int a X, int b = 7);
int multiply (int x);

int amb (int = 10);
int amb ();

int X main X ()
X{
	std::cout << X add2 (2) << std::endl;
	std::cout << add2 X (2, 5) << std::endl;
	std::cout << multiply(4, X 14) << std:: X endl;

	amb (10);
	// amb (); // error: ambigious call

	return 0;
}X

int add2 (int i)
{
	int j = i + 2;
	return j;
}

int add2 (int i, int j)
{
	int k = i + j + 2;
	return k;
}

int multiply (int a, int b)
{
	return a * b;
}

int amb (int x) {
	std::cout << "X" << std::endl;
	return x + x;
}

int amb () {
	std::cout << "Y" << std::endl;
	return 2;
}
