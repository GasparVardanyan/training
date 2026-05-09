# include <algorithm>
# include <array>
# include <iostream>
# include <iterator>
# include <memory>
# include <numeric>
# include <cstddef>
# include <tuple>
# include <type_traits>
# include <utility>

namespace pre_cpp17 {
	template <typename T>
	struct MakeSharedArrayImpl;

	template <typename T, std::size_t S>
	struct MakeSharedArrayImpl <T [S]> {
		constexpr std::shared_ptr <T> operator() ()
			noexcept (std::is_nothrow_constructible_v <
				std::shared_ptr <T>,
				std::shared_ptr <std::array <T, S>> &,
				decltype (std::declval <std::array <T, S>> ().data ())
			>)
		{
			auto r = std::make_shared <std::array <T, S>> ();
			if (nullptr == r) {
				return {};
			}
			else {
				// NOTE: aliasing constructor
				// shares ownership with r, but wraps r->data ()
				return std::shared_ptr <T> (r, r->data ());
			}
		}
	};

	template <typename Arr>
	std::shared_ptr <std::remove_extent_t <Arr>> MakeSharedArray () {
		return MakeSharedArrayImpl <Arr> {} ();
	}
}



const std::size_t arr_size = 10;



void shared_arrays () {
	// shared pointers to arrays
	std::shared_ptr <int []> arr1 = std::make_shared <int []> (arr_size);
	std::shared_ptr <int []> arr2 (new int [arr_size]);

	// shared pointers of ints
	std::shared_ptr <int> arr3 (new int [arr_size], std::default_delete <int []>());
	std::shared_ptr <int> arr4 = pre_cpp17::MakeSharedArray <int [arr_size]> ();

	std::cout << "arrays:" << std::endl;
	std::apply ([] (auto & ... arrays) -> void {
		([& arrays] {
			std::iota (arrays.get (), arrays.get () + arr_size, 1);
			std::cout << '\t';
			std::copy (arrays.get (), arrays.get () + arr_size, std::ostream_iterator <int> (std::cout, ", "));
			std::cout << std::endl;
		} (), ... );
	}, std::tie (arr1, arr2, arr3, arr4));

	std::cout << "array subscription:" << std::endl;
	std::apply ([] (auto & ... arrays) -> void {
		([&arrays] {
			std::cout << '\t';
			for (std::size_t i = 0; i < arr_size; i++) {
				std::cout << arrays [i] << ", ";
			}
			std::cout << std::endl;
		} (), ...);
	}, std::tie (arr1, arr2));
}



void aliasing () {
	struct S {
		int v;
	};
	std::shared_ptr <int> sha2;

	{
		std::shared_ptr <S> sha1 (new S {.v = 10});
		std::cout << "sha1 use count: " << sha1.use_count () << std::endl;
		std::cout << "sha2 use count: " << sha2.use_count () << std::endl;
		sha2 = std::shared_ptr <int> (sha1, & sha1->v);
		std::cout << "sha1 use count: " << sha1.use_count () << std::endl;
		std::cout << "sha2 use count: " << sha2.use_count () << std::endl;
	}
	std::cout << "sha2 use count: " << sha2.use_count () << std::endl;

	std::cout << * sha2 << std::endl;
}



void ownership_transfer () {
	std::shared_ptr <int> i = std::make_shared <int> (10);
	std::shared_ptr <int> j;
	j = i;
	* j = 20;
	std::cout << * i << std::endl;

	std::shared_ptr <int> k = std::move (i);

	std::cout << "i: " << i << ", j: " << j << ", k: " << k << std::endl;
}

void shared_from_this () {
	struct S : std::enable_shared_from_this <S> {
		int value = 10;
	};

	S * s = new S;

	std::shared_ptr <S> sh1 (s);
	std::cout << "sh1 use count: " << sh1.use_count () << std::endl;
	// std::shared_ptr <S> sh2 (s); // causes double free
	std::shared_ptr <S> sh3 = s->shared_from_this ();
	std::cout << "sh1 use count: " << sh1.use_count () << std::endl;
	std::cout << "sh3 use count: " << sh3.use_count () << std::endl;
}

int main () {
	shared_arrays ();
	aliasing ();
	shared_from_this ();
}
