# include <iostream>
# include <mutex>
# include <string>

static void print (const std::string & str) {
	std::cout << "STR: [" << str << "]\n";
}

static void print (int num) {
	std::cout << "INT: [" << num << "]\n";
}

static void print (double dbl) {
	std::cout << "DBL: [" << dbl << "]\n";
}

namespace {

class Integer {
public:
	explicit Integer (int i) : i (i) {}

	inline void print () { // NOLINT(readability-make-member-function-const)
		std::cout << "int: " << i << '\n';
	}

	inline void print () const {
		std::cout << "const int: " << i << '\n';
	}

private:
	int i;
};

class ConstCorrect {
public:
	inline void good_func () const {
		std::cout << "I don't care whether the instance is const or not.\n";
	}
	inline void bad_func () {
		std::cout << "I can only be called on non-const and non-volatile instances.\n";
	}
};

class Integer2 {
public:
	explicit Integer2 (int i) : i (i) {}

	int get () const {
		const std::scoped_lock lock (mtx);
		return i;
	}

	void set (int i) {
		const std::scoped_lock lock (mtx);
		this->i = i;
	}

private:
	int i;
	mutable std::mutex mtx;
};

} // end anonymous namespace

int main () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

	print ("HELLO");
	print (42);
	print (42.2);

	print (static_cast <double> (5));
	print (static_cast <int> (5));

	Integer i (123);
	i.print ();

	const Integer j = i;
	j.print ();

	ConstCorrect cc;
	cc.good_func ();
	cc.bad_func ();

	const ConstCorrect & ccr = cc;
	ccr.good_func ();
	// ccr.bad_func (); // error

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
