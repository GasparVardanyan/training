# include <concepts>
# include <functional>
# include <iostream>
# include <type_traits>



// https://akrzemi1.wordpress.com/2025/05/24/concepts-vs-type-traits/



template <typename T>
constexpr bool has_fun = requires (T v) {
	v.fun ();
};

template <typename T>
requires std::is_integral_v <T>
void fun (T);

template <typename T>
concept Small = sizeof (T) <= sizeof (int);

template <Small T>
void fun (T);

void fun (Small auto);

void fun (Small <> auto);

void fun (Small auto s);
// void fun (auto Small s); // here Small is an identifier, not a constraint



template <typename T, typename U>
concept __EquallySized = sizeof (T) == sizeof (U);
template <typename T, typename U>
concept EquallySized = __EquallySized <T, U> && __EquallySized <U, T>;

template <Small T, EquallySized <T> U>
void fun (T, U);

void fun (Small auto x, EquallySized <decltype (x)> auto y);

template <typename T, std::size_t S>
concept FixedSize = sizeof (T) == S;

// NOTE: we pass concept argument except the first
// one, which is the type being constrained
template <Small T, FixedSize <sizeof (T)> U>
void fun (T, U);

void fun (Small auto x, FixedSize <sizeof (decltype (x))> auto y);



template <typename ... TS>
requires (std::is_integral_v <TS> && ...)
int sum (TS ... vs) {
	return 0 + (vs + ...);
}

template <typename I, typename ... TS>
requires std::invocable <I, TS ...>
decltype (auto) MyInvoke (I && func, TS && ... args)
	noexcept (std::is_nothrow_invocable_v <I, TS...>)
{
	return std::forward <I> (func) (std::forward <TS> (args) ...);
}

template <typename X, typename Y, std::invocable <X, Y> F>
void apply (X, Y, F);

int make_x () { return 1; }
int make_y () { return 1; }

void fun () {
	auto x1 = make_x ();
	static_assert (Small <decltype (x1)>);
	(void) x1;

	Small auto x2 = make_x ();
	(void) x2;

	decltype (auto) y1 = make_y ();
	static_assert (EquallySized <decltype (y1), int>);
	(void) y1;

	EquallySized <int> decltype (auto) y2 = make_y ();
	(void) y2;
}

template <typename T>
requires requires (T t) {
	{ t.func () } -> EquallySized <int>;
}
void funA ();

template <typename T>
requires requires (T t) {
	requires EquallySized <decltype ((t.func ())), int>;
}
void funB ();


template <typename T, typename U>
requires Small <T>
void f1 (T, U) {
	std::cout << "f1 - 1" << std::endl;
}

template <typename T, typename U>
requires Small <T> && EquallySized <T, U>
void f1 (T, U) {
	std::cout << "f1 - 2" << std::endl;
}

void f2 (auto t, auto u) requires Small <decltype (t)> {
	(void) t;
	(void) u;
	std::cout << "f2 - 1" << std::endl;
}

void f2 (auto t, auto u)
	// non-Standard behaviour with clang
	requires Small <decltype (t)> && EquallySized <decltype (t), decltype (u)>
{
	(void) t;
	(void) u;
	std::cout << "f2 - 2" << std::endl;
}

void f3 (Small auto, auto) {
	std::cout << "f3 - 1" << std::endl;
}

void f3 (Small auto t, EquallySized <decltype (t)> auto) {
	std::cout << "f3 - 2" << std::endl;
}



int main () {
	std::cout << std::invoke (sum <int, int>, 5, 2) << std::endl;
	std::cout << MyInvoke (sum <int, int>, 5, 2) << std::endl;

	f1 (5, 5.1);
	f1 (5, 5);
	f2 (5, 5.1);
	// f2 (5, 5);
	f3 (5, 5.1);
	f3 (5, 5);
	// std::cout << sum (5, 2, 6) << std::endl;
}
