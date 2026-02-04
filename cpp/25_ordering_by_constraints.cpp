# include <concepts>
# include <iostream>
# include <type_traits>



// https://akrzemi1.wordpress.com/2020/05/07/ordering-by-constraints/



namespace ordering_by_constraints {
template <typename T, typename U>
requires std::is_integral_v <T>
void fun1 (T, U) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T, typename U>
requires std::is_integral_v <T> && std::is_integral_v <U>
void fun1 (T, U) {
	std::cout << "fun1 - 2" << std::endl;
}

template <typename T, typename U>
requires std::integral <T>
void fun2 (T, U) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T, typename U>
requires std::integral <T> && std::integral <U>
void fun2 (T, U) {
	std::cout << "fun2 - 2" << std::endl;
}

template <typename T, typename U>
requires std::integral <T>
void fun3 (T, U) {
	std::cout << "fun3 - 1" << std::endl;
}

template <typename T, typename U>
requires (bool (std::integral <T> && std::integral <U>))
void fun3 (T, U) {
	std::cout << "fun3 - 2" << std::endl;
}

template <std::integral T, typename U>
void fun4 (T, U) {
	std::cout << "fun4 - 1" << std::endl;
}

template <std::integral T, std::integral U>
void fun4 (T, U) {
	std::cout << "fun4 - 2" << std::endl;
}

template <typename T>
void fun5 (std::integral auto, T) {
	std::cout << "fun5 - 1" << std::endl;
}

void fun5 (std::integral auto, std::integral auto) {
	std::cout << "fun5 - 2" << std::endl;
}

void fun6 (std::integral auto, auto) {
	std::cout << "fun6 - 1" << std::endl;
}

void fun6 (std::integral auto, std::integral auto) {
	std::cout << "fun6 - 2" << std::endl;
}

template <typename T>
concept trivial = std::is_trivial_v <T>;

template <typename T>
requires std::integral <T>
void fun7 (T) {
	std::cout << "fun7 - 1" << std::endl;
}

template <typename T>
void fun7 (T) {
	std::cout << "fun7 - 2" << std::endl;
}

template <typename T>
requires std::integral <T>
void fun8 (T) {
	std::cout << "fun8 - 1" << std::endl;
}

template <typename T>
requires std::integral <T> && trivial <T> // more constraining than std::integral <T>
void fun8 (T) {
	std::cout << "fun8 - 2" << std::endl;
}

template <typename T>
requires std::integral <T>
void fun9 (T) {
	std::cout << "fun9 - 1" << std::endl;
}

template <typename T>
requires std::integral <T> || trivial <T> // less constraining than std::integral <T>
void fun9 (T) {
	std::cout << "fun9 - 2" << std::endl;
}
}



namespace ordering_by_constraints::subsumption {
struct big_int {};

template <typename T>
concept Scalar = std::is_scalar_v <T>;

template <typename T>
struct mathematical_traits {
	constexpr static bool customized = false;
};

template <>
struct mathematical_traits <big_int> {
	constexpr static bool customized = true;
};

template <typename T>
concept CustomMath = mathematical_traits <T>::customized;

template <typename T>
concept Mathematical = Scalar <T> || CustomMath <T>;

template <Mathematical T, Mathematical U>
// conjuctive normal form :
// Mathematical T, Mathematical U <=>
// <=> typename T, typename U, requires Mathematical <T> && Mathematical <U> <=>
// <=> typename T, typename U, requires
//     (Scalar <T> || CustomMath <T>) && (Scalar <U> || CustomMath <U>)
//
// disjunctieve normal form :
// (ST || CT) && (SU || CU) =
// = ST && SU || ST && CU || CT && SU || CT && CU

void fun1 (const T &, const U &) {
	std::cout << "Q" << std::endl;
}

template <typename T, typename U>
requires (Scalar <T> && Scalar <U>) || (CustomMath <T> && CustomMath <U>) // disjunctive normal form
// conjuctive normal form :
// (ST && SU) || (CT && CU) =
// = (ST || CT) && (ST || CU) && (SU || CT) && (SU || CU)
void fun1 (const T &, const U &) {
	std::cout << "P" << std::endl;
}

template <typename T, typename U>
concept __SameAs = std::is_same_v <T, U>;

template <typename T, typename U>
requires __SameAs <T, U>
void fun2 (T, U) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T, typename U>
requires __SameAs <U, T> && std::is_integral_v <T>
void fun2 (T, U) {
	std::cout << "fun2 - 2" << std::endl;
}



template <typename T, typename U>
concept SameAs = __SameAs <T, U> && __SameAs <U, T>;

template <typename T, typename U>
requires SameAs <T, U>
void fun3 (T, U) {
	std::cout << "fun3 - 1" << std::endl;
}

template <typename T, typename U>
requires SameAs <U, T> && std::is_integral_v <T>
void fun3 (T, U) {
	std::cout << "fun3 - 2" << std::endl;
}



template <typename T, typename U>
concept SameAs2 = requires {
	requires __SameAs <T, U>;
	requires  __SameAs <U, T>;
};

template <typename T, typename U>
requires SameAs2 <T, U>
void fun4 (T, U) {
	std::cout << "fun4 - 1" << std::endl;
}

template <typename T, typename U>
requires SameAs2 <U, T> && std::is_integral_v <T>
void fun4 (T, U) {
	std::cout << "fun4 - 2" << std::endl;
}
}



int main () {
	{
		std::cout << "ordering_by_constraints" << std::endl;

		using namespace ordering_by_constraints;

		fun1 (1, 1.1);
		// fun1 (1, 1); // ambiguous
		fun2 (1, 1.1);
		fun2 (1, 1);
		fun3 (1, 1.1);
		// fun3 (1, 1); // ambiguous
		fun4 (1, 1.1);
		fun4 (1, 1);
		fun5 (1, 1.1);
		// fun5 (1, 1); // ambiguous
		fun6 (1, 1.1);
		fun6 (1, 1);

		fun7 (1); // 1
		fun7 (1.1); // 2

		fun8 (1); // 2
		// fun8 (1.1); // ambiguous

		fun9 (1); // 1
		fun9 (1.1); // 2
	}

	{
		std::cout << "ordering_by_constraints::subsumption" << std::endl;

		using namespace ordering_by_constraints::subsumption;

		// P subsumes Q and Q doesn't subsume P, so P is more constained than Q
		fun1 (1, 1); // P
		fun1 (big_int {}, big_int {}); // P
		fun1 (big_int {}, 1); // Q
		fun1 (1, big_int {}); // Q

		fun2 (1.1, 1.1);
		// fun2 (1, 1); // ambiguous
		fun3 (1.1, 1.1);
		fun3 (1, 1);
		fun4 (1.1, 1.1);
		// fun4 (1, 1); // ambiguous
	}
}
