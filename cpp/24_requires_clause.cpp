# include <concepts>
# include <iostream>
# include <string>
# include <type_traits>
# include <vector>



// https://akrzemi1.wordpress.com/2020/03/26/requires-clause/



namespace requires_clause {
template <typename T>
requires std::is_standard_layout_v <T> && std::is_trivial_v <T>
void fun1 (T) {}

template <typename T>
requires std::is_standard_layout_v <T> && (! std::is_trivial_v <T>)
void fun2 (T) {}

template <typename T>
requires std::is_trivial_v <T>
void fun3 (T) {
	std::cout << "fun3 - 1" << std::endl;
}

template <typename T>
void fun3 (T) {
	std::cout << "fun3 - 2" << std::endl;
}

template <typename T>
struct S1 { static void f () {
	std::cout << "S1 - 1" << std::endl;
} };

template <typename T>
requires std::is_trivial_v <T>
struct S1 <T> { static void f () {
	std::cout << "S1 - 2" << std::endl;
} };
}



namespace requires_clause::two_step_satisfaction {
template <typename T>
requires std::is_trivial_v <typename T::value_type>
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
void fun1 (T) {
	std::cout << "fun1 - 2" << std::endl;
}

template <typename T>
struct S1 { static void f () {
	std::cout << "S1 - 1" << std::endl;
} };

template <typename T>
requires std::is_trivial_v <typename T::value_type>
struct S1 <T> { static void f () {
	std::cout << "S1 - 2" << std::endl;
} };



template <typename T>
struct is_small {
	enum { value = sizeof (T) <= sizeof (int) };
};

template <typename T>
requires (static_cast <bool> (is_small <T>::value))
void fun2 (T) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T>
void fun2 (T) {
	std::cout << "fun2 - 2" << std::endl;
}
}



namespace requires_clause::valid_redicates {
template <typename T>
requires std::is_trivial_v <T> unsigned short int f1 ();

template <typename T>
requires std::is_trivial_v <T> unsigned
short int f1 ();

template <typename T>
requires std::is_trivial_v <T> unsigned short
int f1 ();

// template <typename T>
// requires (bool) & T::operator unsigned short int f2 ();

template <typename T>
requires ((bool) & T::operator unsigned) short int f2 ();

template <typename T>
requires ((bool) & T::operator unsigned short) int f2 ();

// template <typename T>
// requires (bool) & T::operator unsigned void f2 ();



// template <typename T>
// requires !std::is_trivial_v <T>
// void f4 ();

template <typename T>
requires (!std::is_trivial_v <T>)
void f4 ();



template <typename T>
// T::value_type is well formed and is integral
requires std::is_integral_v <typename T::value_type>
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
void fun1 (T) {
	std::cout << "fun1 - 2" << std::endl;
}

template <typename T>
// T::value_type IS well formed and IS NOT integral
requires (!std::is_integral_v <typename T::value_type>)
void fun2 (T) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T>
void fun2 (T) {
	std::cout << "fun2 - 2" << std::endl;
}



// same with a custom traits

namespace custom_trait {
template <typename T>
inline constexpr bool vt_valid_and_integral = std::is_integral_v <typename T::value_type>;

template <typename T>
// T::value_type is well formed and is integral
requires vt_valid_and_integral <T>
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
void fun1 (T) {
	std::cout << "fun1 - 2" << std::endl;
}

template <typename T>
// T::value_type IS well formed and IS NOT integral
requires (!vt_valid_and_integral <T>)
void fun2 (T) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T>
void fun2 (T) {
	std::cout << "fun2 - 2" << std::endl;
}
}

namespace custom_concept {
template <typename T>
concept vt_valid_and_integral = std::is_integral_v <typename T::value_type>;

template <typename T>
// T::value_type is well formed and is integral
requires vt_valid_and_integral <T>
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
void fun1 (T) {
	std::cout << "fun1 - 2" << std::endl;
}

template <typename T>
// T::value_type IS NOT well formed or IS NOT integral
requires (!vt_valid_and_integral <T>)
void fun2 (T) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T>
void fun2 (T) {
	std::cout << "fun2 - 2" << std::endl;
}
}

namespace custom_trait2 {
template <typename T, typename = void>
inline constexpr bool vt_valid_and_integral = false;

template <typename T>
inline constexpr bool vt_valid_and_integral <T, std::void_t <typename T::value_type>>
	= std::is_integral_v <typename T::value_type>
;

template <typename T>
// T::value_type is well formed and is integral
requires vt_valid_and_integral <T>
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
void fun1 (T) {
	std::cout << "fun1 - 2" << std::endl;
}

template <typename T>
// T::value_type IS NOT well formed or IS NOT integral
requires (!vt_valid_and_integral <T>)
void fun2 (T) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T>
void fun2 (T) {
	std::cout << "fun2 - 2" << std::endl;
}
}
namespace custom_trait3 {
template <typename T, typename = void>
inline constexpr bool vt_valid_and_integral = false;

template <typename T>
inline constexpr bool vt_valid_and_integral <T, typename T::value_type>
	= std::is_integral_v <typename T::value_type>
;

template <typename T>
// T::value_type is well formed and is integral
requires vt_valid_and_integral <T>
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
void fun1 (T) {
	std::cout << "fun1 - 2" << std::endl;
}

template <typename T>
// T::value_type IS NOT well formed or IS NOT integral
requires (!vt_valid_and_integral <T>)
void fun2 (T) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T>
void fun2 (T) {
	std::cout << "fun2 - 2" << std::endl;
}
}
}



namespace requires_clause::conjuction_and_disjunction {
template <typename T, typename U>
requires
	   std::is_integral_v <typename T::value_type>
	|| std::is_integral_v <typename U::value_type> // constraint disjunction
void fun1 (T, U) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T, typename U>
void fun1 (T, U) {
	std::cout << "fun1 - 2" << std::endl;
}

template <typename T, typename U>
requires (bool (
	   std::is_integral_v <typename T::value_type>
	|| std::is_integral_v <typename U::value_type> // logical or
))
void fun2 (T, U) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T, typename U>
void fun2 (T, U) {
	std::cout << "fun2 - 2" << std::endl;
}

template <typename T, typename U>
requires (
	   !std::is_integral_v <typename T::value_type>
	|| !std::is_integral_v <typename U::value_type> // constraint disjunction
)
void fun3 (T, U) {
	std::cout << "fun3 - 1" << std::endl;
}

template <typename T, typename U>
void fun3 (T, U) {
	std::cout << "fun3 - 2" << std::endl;
}

template <typename T, typename U>
requires (bool (
	   !std::is_integral_v <typename T::value_type>
	|| !std::is_integral_v <typename U::value_type> // logical or
))
void fun4 (T, U) {
	std::cout << "fun4 - 1" << std::endl;
}

template <typename T, typename U>
void fun4 (T, U) {
	std::cout << "fun4 - 2" << std::endl;
}

template <typename T, typename U>
requires (!std::is_trivial_v <T>) && (
	   std::is_integral_v <typename T::value_type>
	|| std::is_integral_v <typename U::value_type> // constraint disjunction
)
void fun5 (T, U) {
	std::cout << "fun5 - 1" << std::endl;
}

template <typename T, typename U>
void fun5 (T, U) {
	std::cout << "fun5 - 2" << std::endl;
}

template <typename T, typename U>
requires (!std::is_trivial_v <T>) && (bool (
	   std::is_integral_v <typename T::value_type>
	|| std::is_integral_v <typename U::value_type> // logical or
))
void fun6 (T, U) {
	std::cout << "fun6 - 1" << std::endl;
}

template <typename T, typename U>
void fun6 (T, U) {
	std::cout << "fun6 - 2" << std::endl;
}

template <typename T, typename U>
requires (
	   !std::is_integral_v <typename T::value_type>
	|| !std::is_integral_v <typename U::value_type> // constraint disjunction
)
void fun7 (T, U) {
	std::cout << "fun7 - 1" << std::endl;
}

template <typename T, typename U>
void fun7 (T, U) {
	std::cout << "fun7 - 2" << std::endl;
}

template <typename T, typename U>
requires (!(
	   std::is_integral_v <typename T::value_type>
	&& std::is_integral_v <typename U::value_type> // does logical/constraint distinction make any sense here?
))
// requires (
// 	   !std::is_integral_v <typename T::value_type>
// 	&& !std::is_integral_v <typename U::value_type> // constraint conjuction
// )
void fun8 (T, U) {
	std::cout << "fun8 - 1" << std::endl;
}

template <typename T, typename U>
void fun8 (T, U) {
	std::cout << "fun8 - 2" << std::endl;
}

template <typename ... TS>
requires (std::is_integral_v <typename TS::value_type> || ...) // logical or
void fun9 (TS ...) {
	std::cout << "fun9 - 1" << std::endl;
}

template <typename ... TS>
void fun9 (TS ...) {
	std::cout << "fun9 - 2" << std::endl;
}

template <typename T>
concept integral_type = std::is_integral_v <T>;

template <typename ... TS>
requires (integral_type <typename TS::value_type> || ...) // logical or
void fun10 (TS ...) {
	std::cout << "fun10 - 1" << std::endl;
}

template <typename ... TS>
void fun10 (TS ...) {
	std::cout << "fun10 - 2" << std::endl;
}

template <typename T>
concept integral_value_type = std::is_integral_v <typename T::value_type>;

template <typename ... TS>
requires (integral_value_type <TS> || ...) // logical or, but concepts don't fail on ill-formed expressions
void fun11 (TS ...) {
	std::cout << "fun11 - 1" << std::endl;
}

template <typename ... TS>
void fun11 (TS ...) {
	std::cout << "fun11 - 2" << std::endl;
}
}



namespace requires_clause::overload_ordering {
template <typename T>
requires std::unsigned_integral <T>
void fun1 (T) {
	std::cout << "fun1 - 0" << std::endl;
}

template <typename T>
// even though integral is defined as:
// template<typename _Tp>
//   concept integral = is_integral_v<_Tp>;
//
// std::is_integral_v doesn't work here because traits are evaluated directly
// while constraints are being compared
//
// requires std::is_integral_v <T>
requires std::integral <T>
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
void fun1 (T) {
	std::cout << "fun1 - 2" << std::endl;
}



template <typename T>
concept integral = std::is_integral_v <T>;

template<typename T>
// doesn't work:
// concept unsigned_integral = std::is_integral_v <T> && !std::is_signed_v<T>;
concept unsigned_integral = integral <T> && !std::is_signed_v<T>;


template <typename T>
requires unsigned_integral <T>
void fun2 (T) {
	std::cout << "fun2 - 0" << std::endl;
}

template <typename T>
requires integral <T>
void fun2 (T) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T>
void fun2 (T) {
	std::cout << "fun2 - 2" << std::endl;
}

template <typename T>
requires /* std::is_integral_v <T> && */ std::is_unsigned_v <T>
void fun3 (T) {
	std::cout << "fun3 - 0" << std::endl;
}

template <typename T>
requires std::is_integral_v <T> && std::is_signed_v <T>
void fun3 (T) {
	std::cout << "fun3 - 1" << std::endl;
}

template <typename T>
void fun3 (T) {
	std::cout << "fun3 - 2" << std::endl;
}
}

namespace requires_clause::concept_vs_direct_requirement {
template <typename T>
// well formed and integral
concept MyIntegral = std::is_integral_v <typename T::value_type>;

template <typename T>
// concept returns false = !(well formed and integral) = not well formed or not integral
requires (false == MyIntegral <T>)
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
void fun1 (T) {
	std::cout << "fun1 - 1" << std::endl;
}

template <typename T>
// well formed and integral
requires (false == std::is_integral_v <typename T::value_type>)
void fun2 (T) {
	std::cout << "fun2 - 1" << std::endl;
}

template <typename T>
void fun2 (T) {
	std::cout << "fun2 - 2" << std::endl;
}

template <typename T>
// well formed and not integral
concept MyNotIntegral = (false == std::is_integral_v <typename T::value_type>);

template <typename T>
// well formed and integral
requires MyNotIntegral <T>
void fun3 (T) {
	std::cout << "fun3 - 1" << std::endl;
}

template <typename T>
void fun3 (T) {
	std::cout << "fun3 - 2" << std::endl;
}

template <typename T, typename = void>
inline constexpr bool MyIntegralTrait = false;

// well formed and integral
template <typename T>
inline constexpr bool MyIntegralTrait <T, std::void_t <typename T::value_type>>
	= std::is_integral_v <typename T::value_type>
;

template <typename T>
// trait returns false = !(well formed and integral) = not well formed or not integral
requires (false == MyIntegralTrait <T>)
void fun4 (T) {
	std::cout << "fun4 - 1" << std::endl;
}

template <typename T>
void fun4 (T) {
	std::cout << "fun4 - 2" << std::endl;
}

template <typename T, typename = void>
inline constexpr bool MyNotIntegralTrait = false;

template <typename T>
inline constexpr bool MyNotIntegralTrait <T, std::void_t <typename T::value_type>>
	= false == std::is_integral_v <typename T::value_type>
;

template <typename T>
// well formed and not integral
requires MyNotIntegralTrait <T>
void fun5 (T) {
	std::cout << "fun5 - 1" << std::endl;
}

template <typename T>
void fun5 (T) {
	std::cout << "fun5 - 2" << std::endl;
}

}



int main () {
	{
		std::cout << "requires_clause" << std::endl;

		using namespace requires_clause;

		fun1 (1);
		// fun (std::string {})
		fun3 (std::string {});

		S1 <int>::f ();
		S1 <std::string>::f ();
	}

	{
		std::cout << "requires_clause::two_step_satisfaction" << std::endl;

		using namespace requires_clause::two_step_satisfaction;

		fun1 (1);
		fun1 (std::vector <int> {});

		S1 <int>::f ();
		S1 <std::vector <int>>::f ();

		fun2 (1);
		fun2 (1LL);
	}

	{
		std::cout << "requires_clause::valid_redicates" << std::endl;

		using namespace requires_clause::valid_redicates;

		// NOTE: T::value_type specialization here is in the "immediate context", so SFINAE applies
		fun1 (1); // T::value_type isn't well formed
		fun1 (std::vector <int> {}); // T::value_type is well formed and is integral
		fun1 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		// NOTE: T::value_type specialization here is in the "immediate context", so SFINAE applies
		fun2 (1); // T::value_type isn't well formed
		fun2 (std::vector <int> {}); // T::value_type is well formed and is integral
		fun2 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		std::cout << "====================" << std::endl;

		// NOTE: T::value_type specialization here isn't in the "immediate context", so SFINAE doesn't apply
		// custom_trait::fun1 (1); // T::value_type isn't well formed
		custom_trait::fun1 (std::vector <int> {}); // T::value_type is well formed and is integral
		custom_trait::fun1 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		// NOTE: T::value_type specialization here isn't in the "immediate context", so SFINAE doesn't apply
		// custom_trait::fun2 (1); // T::value_type isn't well formed
		custom_trait::fun2 (std::vector <int> {}); // T::value_type is well formed and is integral
		custom_trait::fun2 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		std::cout << "====================" << std::endl;

		// NOTE: T::value_type specialization here isn't in the "immediate context", but SFINAE applies to concepts
		custom_concept::fun1 (1); // T::value_type isn't well formed
		custom_concept::fun1 (std::vector <int> {}); // T::value_type is well formed and is integral
		custom_concept::fun1 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		// NOTE: T::value_type specialization here isn't in the "immediate context", but SFINAE applies to concepts
		custom_concept::fun2 (1); // T::value_type isn't well formed
		custom_concept::fun2 (std::vector <int> {}); // T::value_type is well formed and is integral
		custom_concept::fun2 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		// but now fun2 (1) and custom_concept::fun2 (1) differ

		std::cout << "====================" << std::endl;

		// NOTE: T::value_type specialization here isn't in the "immediate context", but template partial specialization picks the right template
		custom_trait2::fun1 (1); // T::value_type isn't well formed
		custom_trait2::fun1 (std::vector <int> {}); // T::value_type is well formed and is integral
		custom_trait2::fun1 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		// NOTE: T::value_type specialization here isn't in the "immediate context", but template partial specialization picks the right template
		custom_trait2::fun2 (1); // T::value_type isn't well formed
		custom_trait2::fun2 (std::vector <int> {}); // T::value_type is well formed and is integral
		custom_trait2::fun2 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		std::cout << "====================" << std::endl;

		// NOTE: T::value_type specialization here isn't in the "immediate context", and even T having value_type
		// doesn't make the partial specialization more specialized for T than the primary: when T::value_type is
		// well formed and is integral, it's not void and doesn't match the primary template where the second template
		// parameter is defaulted to void, and when T::value_type is void, the partial specialization matches but
		// since void isn't integral, result is false, so custom_trait3::fun1 never satisfies the requirement.
		// This will match: custom_trait3::vt_valid_and_integral <std::vector <int>, int>, but fun1 uses the
		// second parameter's default type void and since partial specialziation uses std::vector <int>::value_type which
		// is int, not void, the partial specialization doesn't match.
		// In case of custom_trait2::fun1, std::void_t <std::vector <int>, int>> = void, so the partial specialization
		// is matched and since not all types have value_type type, this is more specialized than the primary
		custom_trait3::fun1 (1); // T::value_type isn't well formed
		custom_trait3::fun1 (std::vector <int> {}); // T::value_type is well formed and is integral
		custom_trait3::fun1 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral
		custom_trait3::fun2 (1); // T::value_type isn't well formed
		custom_trait3::fun2 (std::vector <int> {}); // T::value_type is well formed and is integral
		custom_trait3::fun2 (std::vector <std::string> {}); // T::value_type is well formed but isn't integral

		std::cout << "====================" << std::endl;

		fun2 (1);
		// since int::value_type isn't well formed, first template matches
		custom_concept::fun2 (1);
	}

	{
		std::cout << "requires_clause::conjuction_and_disjunction" << std::endl;
		using namespace requires_clause::conjuction_and_disjunction;

		fun1 (std::vector <int> {}, std::vector <int> {}); // 1
		fun1 (std::vector <int> {}, std::vector <float> {}); // 1
		fun1 (std::vector <float> {}, std::vector <int> {}); // 1
		fun1 (std::vector <float> {}, std::vector <float> {}); // 2
		fun1 (5, std::vector <float> {}); // 2
		fun1 (5, std::vector <int> {}); // 1

		std::cout << "====================" << std::endl;

		fun2 (std::vector <int> {}, std::vector <int> {}); // 1
		fun2 (std::vector <int> {}, std::vector <float> {}); // 1
		fun2 (std::vector <float> {}, std::vector <int> {}); // 1
		fun2 (std::vector <float> {}, std::vector <float> {}); // 2
		fun2 (5, std::vector <float> {}); // 2
		fun2 (5, std::vector <int> {}); // 2

		std::cout << "====================" << std::endl;

		fun1 (std::vector <int> {}, 1); // 1
		fun2 (std::vector <int> {}, 1); // 2

		fun3 (std::vector <float> {}, 1); // 1
		fun4 (std::vector <float> {}, 1); // 2

		fun5 (std::vector <int> {}, 1); // 1
		fun6 (std::vector <int> {}, 1); // 2

		fun7 (std::vector <float> {}, 1); // 1
		fun8 (std::vector <float> {}, 1); // 2

		fun9 (std::vector <int> {}, std::vector <float> {}); // 1
		fun9 (std::vector <int> {}, 1); // 2
		fun10 (std::vector <int> {}, std::vector <float> {}); // 1
		fun10 (std::vector <int> {}, 1); // 2
		fun11 (std::vector <int> {}, std::vector <float> {}); // 1
		fun11 (std::vector <int> {}, 1); // 1
	}

	{
		std::cout << "requires_clause::overload_ordering" << std::endl;
		using namespace requires_clause::overload_ordering;

		fun1 (1u);
		fun1 (1);
		fun1 (1.1);

		fun2 (1u);
		fun2 (1);
		fun2 (1.1);

		fun3 (1u);
		fun3 (1);
		fun3 (1.1);
	}

	{
		std::cout << "requires_clause::concept_vs_direct_requirement" << std::endl;
		using namespace requires_clause::concept_vs_direct_requirement;

		fun1 (1); // 1
		fun2 (1); // 2
		fun3 (1); // 2
		fun4 (1); // 1
		fun5 (1); // 2
	}
}
