# include <iostream>
# include <memory>

int main () {
	std::auto_ptr <int> aptr (new int (42));
	std::cout << * aptr << std::endl;
	std::cout << aptr.get () << std::endl;

	std::auto_ptr <int> aptr2 = aptr;
	std::cout << aptr.get () << " - " << aptr2.get () << std::endl;
}
