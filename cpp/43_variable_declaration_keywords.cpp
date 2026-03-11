# include <iostream>
# include <type_traits>

int main () {
	int x;
	static_assert (std::is_same_v <decltype (x), int>);
	static_assert (std::is_same_v <decltype ((x)), int &>);

	struct S { int x; };

	static_assert (std::is_same_v <decltype (std::declval <S> ().x), int>);
	static_assert (std::is_same_v <decltype ((std::declval <S> ().x)), int &&>);
	static_assert (std::is_same_v <decltype (std::declval <S> ()), S &&>);
	static_assert (std::is_same_v <std::decay_t <decltype ((std::declval <S> ().x))>, int>);
	static_assert (std::is_same_v <std::decay_t <decltype (std::declval <S> ())>, S>);

	S s;
	static_assert (std::is_same_v <decltype (s.x), int>);
	static_assert (std::is_same_v <decltype ((s.x)), int &>);

	const S t {};
	const int y {};

	static_assert (std::is_same_v <decltype (y), const int>);
	static_assert (std::is_same_v <decltype (t.x), int>);
	static_assert (std::is_same_v <decltype ((y)), const int &>);
	static_assert (std::is_same_v <decltype (t.x), int>);
	static_assert (std::is_same_v <decltype ((t.x)), const int &>);

	auto y1 = y;
	decltype (y) y2 = y;
	decltype (auto) y3 = y;

	static_assert (std::is_same_v <decltype (y1), int>);
	static_assert (std::is_same_v <decltype (y2), const int>);
	static_assert (std::is_same_v <decltype (y3), const int>);
}
