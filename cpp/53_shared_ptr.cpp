# include <algorithm>
# include <cstddef>
# include <iostream>
# include <iterator>
# include <memory>
# include <numeric>
# include <tuple>

int main () {
	std::shared_ptr <int> i = std::make_shared <int> (10);
	std::shared_ptr <int> j;
	j = i;
	* j = 20;
	std::cout << * i << std::endl;

	const std::size_t arr_size = 10;

	std::shared_ptr <int []> arr1 = std::make_shared <int []> (arr_size);
	std::shared_ptr <int> arr2 (new int [arr_size] (0), std::default_delete <int []>());

	std::apply ([] (auto & ... arr) -> void {
		([& arr] {
			std::iota (arr.get (), arr.get () + arr_size, 1);
			std::copy (arr.get (), arr.get () + arr_size, std::ostream_iterator <int> (std::cout, ", "));
			std::cout << std::endl;
		} (), ... );
	}, std::tie (arr1, arr2));
}
