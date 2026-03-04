# include <iostream>

enum Direction {
	UP, LEFT, DOWN, RIGHT
};

Direction d = UP;
Direction d2 = Direction::DOWN;



enum E1 {
	E1V1, E1V2, E1V3
};

E1 e1 = E1V1;
E1 e12 = E1::E1V1;



enum E2 : int {
	E2V1, E2V2, E2V3
};

E2 e2 = E2V1;
E2 e22 = E2::E2V1;



enum class E3 {
	V1, V2, V3
};

E3 e3 = E3::V1;



enum struct E4 {
	V1, V2, V3
};

E4 e4 = E4::V1;



enum class E5 : unsigned char {
	V1, V2, V3
};

E5 e5 = E5::V1;



enum struct E6 : unsigned char {
	V1, V2, V3
};

E6 e6 = E6::V1;



enum Foo { FOO };
Foo Foo () { return FOO; }
enum Foo foo = Foo ();



enum class Bar { Bar };
Bar Bar (Bar Bar) { (void) Bar; return Bar::Bar; }
enum Bar b = Bar (Bar::Bar);



enum class E7;
enum E8 : int;
enum struct E9 : short;



int main () {

}
