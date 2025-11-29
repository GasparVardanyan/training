# include <iostream>
# include <string>

bool True (const std::string & id)
{
	std::cout << "True: " << id << std::endl;
	return true;
}

bool False (const std::string & id)
{
	std::cout << "False: " << id << std::endl;
	return false;
}

inline void sep ()
{
	std::cout << "==============================" << std::endl;
}

int main ()
{
	bool result;

	std::cout << std::boolalpha;

	result = False ("A") || (False ("B") && False ("C"));
	std::cout << "result: " << result << std::endl;

	sep ();

	result = True ("A") || (False ("B") && False ("C"));
	std::cout << "result: " << result << std::endl;

	sep ();

	result = (True ("A") || False ("B")) && False ("C");
	std::cout << "result: " << result << std::endl;

	std::cout << std::noboolalpha;
}
