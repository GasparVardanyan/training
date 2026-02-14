# include <iostream>
# include <type_traits>
# include <utility>



//   __       _ _                       _       _ _          _   _
//  / _|_   _| | |  ___ _ __   ___  ___(_) __ _| (_)______ _| |_(_) ___  _ __
// | |_| | | | | | / __| '_ \ / _ \/ __| |/ _` | | |_  / _` | __| |/ _ \| '_ \
// |  _| |_| | | | \__ \ |_) |  __/ (__| | (_| | | |/ / (_| | |_| | (_) | | | |
// |_|  \__,_|_|_| |___/ .__/ \___|\___|_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|
//                     |_|
//

template <typename T>
struct S;

template <>
struct S <int> {
	static constexpr int i = 0;
};



//                   _   _       _                       _       _ _          _   _
//  _ __   __ _ _ __| |_(_) __ _| |  ___ _ __   ___  ___(_) __ _| (_)______ _| |_(_) ___  _ __
// | '_ \ / _` | '__| __| |/ _` | | / __| '_ \ / _ \/ __| |/ _` | | |_  / _` | __| |/ _ \| '_ \
// | |_) | (_| | |  | |_| | (_| | | \__ \ |_) |  __/ (__| | (_| | | |/ / (_| | |_| | (_) | | | |
// | .__/ \__,_|_|   \__|_|\__,_|_| |___/ .__/ \___|\___|_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|
// |_|                                  |_|
//

template <typename T1, typename T2>
struct S1 {
	int x = 10;
};

template <typename T>
struct S1 <T, T> {
	int x = 20;
};



//   __            _             _       _
//  / _| __ _  ___| |_ ___  _ __(_) __ _| |
// | |_ / _` |/ __| __/ _ \| '__| |/ _` | |
// |  _| (_| | (__| || (_) | |  | | (_| | |
// |_|  \__,_|\___|\__\___/|_|  |_|\__,_|_|
//

template <std::size_t N, typename S = std::make_integer_sequence <std::size_t, N>>
struct factorial;

template <std::size_t N, std::size_t ... SS>
struct factorial <N, std::integer_sequence <std::size_t, SS...>> {
	static constexpr std::size_t value = ((SS + 1) * ... * 1);
};



//  __  __       _____          _             _       _
// |  \/  |_   _|  ___|_ _  ___| |_ ___  _ __(_) __ _| |
// | |\/| | | | | |_ / _` |/ __| __/ _ \| '__| |/ _` | |
// | |  | | |_| |  _| (_| | (__| || (_) | |  | | (_| | |
// |_|  |_|\__, |_|  \__,_|\___|\__\___/|_|  |_|\__,_|_|
//         |___/

namespace My {

template <typename T>
concept IntegralType = std::is_integral_v <T>;

template <IntegralType T, T V>
struct IntegralConstant {
	static constexpr T value = V;
};

template <typename>
struct IsIntegralConstantType : IntegralConstant <bool, 0> {};

template <IntegralType T, T V>
struct IsIntegralConstantType <IntegralConstant <T, V>> : IntegralConstant <bool, 1> {};

template <typename T>
concept IntegralConstantType = IsIntegralConstantType <T>::value;

template <IntegralConstantType ... VS>
struct IntegralSequence {};

template <IntegralType T, std::size_t N, IntegralConstantType ... VS>
struct MakeIntegralSequenceImpl : MakeIntegralSequenceImpl <T, N - 1, IntegralConstant <T, N - 1>, VS ...> {};

template <IntegralType T, IntegralConstantType ... VS>
struct MakeIntegralSequenceImpl <T, 0, VS ...> {
	using type = IntegralSequence <VS ...>;
};

template <IntegralType T, std::size_t N>
using MakeIntegralSequence = MakeIntegralSequenceImpl <T, N>::type;

template <typename T>
concept UnsignedType = std::is_unsigned_v <T>;

template <std::size_t N, UnsignedType T = unsigned, typename CS = MakeIntegralSequence <T, N>>
struct Factorial;

template <std::size_t N, UnsignedType T, IntegralConstantType ... CS>
struct Factorial <N, T, IntegralSequence <CS ...>> {
	static constexpr T value = ((1 + CS::value) * ... * 1);
};

}



int main () {
	S <int> x;
	std::cout << x.i << std::endl;

	S1 <int, int> y;
	std::cout << y.x << std::endl;

	std::cout << factorial <4, std::integer_sequence <std::size_t, 1-1, 2-1, 3-1, 10-1>>::value << std::endl;
	std::cout << factorial <8, std::integer_sequence <std::size_t, 1-1, 2-1, 3-1, 10-1>>::value << std::endl;
	std::cout << factorial <8, std::integer_sequence <std::size_t>>::value << std::endl;
	std::cout << factorial <4>::value << std::endl;
	std::cout << factorial <8>::value << std::endl;

	std::cout << My::Factorial <0>::value << std::endl;
	std::cout << My::Factorial <1>::value << std::endl;
	std::cout << My::Factorial <2>::value << std::endl;
	std::cout << My::Factorial <3>::value << std::endl;
	std::cout << My::Factorial <4>::value << std::endl;
	std::cout << My::Factorial <5>::value << std::endl;
	std::cout << My::Factorial <6>::value << std::endl;
	std::cout << My::Factorial <7>::value << std::endl;
	std::cout << My::Factorial <8>::value << std::endl;
	std::cout << My::Factorial <9>::value << std::endl;
}
