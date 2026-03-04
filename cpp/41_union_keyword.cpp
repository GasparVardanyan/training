# include <iostream>

union foo;
class bar {
public:
	bar (foo & f) {
		(void) f;
	}
};

void baz ();
union baz;

union foo {
	long l;
	union baz * b;
};

int main () {

}
