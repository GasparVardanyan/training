# include <cstdio>
# include <iostream>

struct FileAttributes
{
	unsigned int ReadOnly : 1;
	unsigned int Hidden : 1;
};

struct Date
{
	unsigned int Year : 13;
	unsigned int Month : 4;
	unsigned int Day : 5;
};

int main ()
{
	std::cout << sizeof (FileAttributes) << std::endl;
	std::cout << sizeof (Date) << std::endl;

	Date d;
	d.Year = 2002;
	d.Month = 2;
	d.Day = 20;

	std::printf ("%2u - %02u - %u\n", d.Day, d.Month, d.Year);
	std::fflush (stdout);
}
