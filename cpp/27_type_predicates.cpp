# include <iostream>
# include <type_traits>



// https://akrzemi1.wordpress.com/2017/12/02/your-own-type-predicate/



struct true_type {
	static constexpr bool value = true;
};

struct false_type {
	static constexpr bool value = false;
};

template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same <T, T> : true_type {};

template <typename T, typename U>
inline constexpr bool is_same_v = is_same <T, U>::value;



template <typename T, int S = sizeof (T)>
struct one_byte : false_type {};

template <typename T>
struct one_byte <T, 1> : true_type {};



template <bool C, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if <true, T> {
	using type = T;
};

template <bool C, typename T = void>
using enable_if_t = enable_if <C, T>::type;



template <typename T, typename = void>
struct one_byte2 : false_type {};

template <typename T>
struct one_byte2 <T, enable_if_t <sizeof (T) == 1>> : true_type {};

template <typename ...>
using void_t = void;



template <typename T>
T declval () noexcept {
	static_assert (false);
}



template <typename T, typename = void>
struct is_acceptable : false_type {};

template <typename T>
struct is_acceptable <
	T,
	void_t <
		typename T::result_type,
		enable_if_t <std::is_invocable_v <decltype (T::set_limit), int>>,
		enable_if_t <noexcept (declval <T> ().get_result ())>,
		enable_if_t <
			is_same_v <
				const typename T::result_type &,
				decltype (declval <T> ().get_result ())
			>
		>
	>
>
	: std::true_type {};

template <typename T>
inline constexpr bool is_acceptable_v = is_acceptable <T>::value;



struct is_acceptible_c {
	template <typename T>
	auto require (T t, int i) -> void_t <
		typename T::result_type,
		decltype (T::set_limit (i)),
		enable_if_t <noexcept (t.get_result ())>,
		decltype (t.get_result ()),
		enable_if_t <
			is_same_v <const typename T::result_type &, decltype (t.get_result ())>
		>
	>;
};

template <typename T, typename = void>
struct is_acceptable2 : false_type {};

template <typename T>
struct is_acceptable2 <T, void_t <decltype (& is_acceptible_c::require <T>)>>
	: true_type {};

template <typename T>
inline constexpr bool is_acceptable2_v = is_acceptable2 <T>::value;

struct S1 {

};

struct S2 {
	using result_type = int;
	static void set_limit (int) {}
	const result_type & get_result () noexcept {
		static result_type r;
		return (r);
	}
};



int main () {
	std::cout << is_same <int, double>::value << std::endl;
	std::cout << is_same <int, int>::value << std::endl;
	std::cout << one_byte <int>::value << std::endl;
	std::cout << one_byte <char>::value << std::endl;
	std::cout << one_byte2 <int>::value << std::endl;
	std::cout << one_byte2 <char>::value << std::endl;
	std::cout << is_acceptable_v <S1> << std::endl;
	std::cout << is_acceptable_v <S2> << std::endl;
	std::cout << is_acceptable2_v <S1> << std::endl;
	std::cout << is_acceptable2_v <S2> << std::endl;
}
