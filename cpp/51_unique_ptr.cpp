# include <algorithm>
# include <iostream>
# include <iterator>
# include <memory>
# include <numeric>
# include <type_traits>
# include <utility>

struct S {
	int value;
	S (int value = 0) : value (value) {
		std::cout << "S ()" << std::endl;
	}

	~S () {
		std::cout << "~S ()" << std::endl;
	}
};

void unique_ptr_move () {
	std::unique_ptr <S> ptr; {
		auto ptr2 = std::make_unique <S> (5);
		std::cout << ptr2->value << std::endl;
		ptr = std::move (ptr2);
		std::cout << (nullptr == ptr2) << std::endl;
	}
	std::cout << ptr->value << std::endl;
	* ptr = S (20);
	std::cout << ptr->value << std::endl;
}

void unique_ptr_of_array () {
	std::size_t arr_size = 15;
	std::unique_ptr <int []> ptr = std::make_unique <int []> (arr_size);

	std::iota (ptr.get (), ptr.get () + arr_size, 1);

	std::copy (
		ptr.get (),
		ptr.get () + arr_size,
		std::ostream_iterator <int> (std::cout, ", ")
	);
	std::cout << std::endl;
}

std::unique_ptr <int> get_int () {
	std::unique_ptr <int> ip = std::make_unique <int> (100);
	* ip = 1000;
	std::cout << "returning " << * ip << std::endl;
	return ip;
}

// template <typename T, typename ... Args>

int main () {
	unique_ptr_move ();
	std::cout << "====================" << std::endl;
	unique_ptr_of_array ();
	std::cout << "====================" << std::endl;

	std::unique_ptr <int> up = get_int ();
	std::cout << "got " << * up << std::endl;
}
