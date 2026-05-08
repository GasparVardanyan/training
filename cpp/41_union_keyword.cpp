union foo;

class bar {
public:
	bar (foo & f) {
		(void) f;
	}
};

void baz ();

union foo {
	long l;
	union baz * b;
};

int main () {

}
