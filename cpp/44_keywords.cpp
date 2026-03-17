# include <concepts>
# include <cstring>
# include <exception>
# include <initializer_list>
# include <iostream>
# include <stdexcept>
# include <type_traits>
# include <vector>



void keyword_asm () {
	asm volatile (
		"syscall"
		:
		: "a"(60), "D"(42)
		: "rcx", "r11", "memory"
	);
}



void keyword_void_def_c (...) {
	std::cout << __FUNCTION__ << std::endl;
}
void keyword_void_def_cpp (void) {
	std::cout << __FUNCTION__ << std::endl;
}
void keyword_void_def () {
	std::cout << __FUNCTION__ << std::endl;
}

void keyword_void () {
	keyword_void_def_c ();
	keyword_void_def_c (1);
	keyword_void_def_c (1, 2);
	keyword_void_def_cpp ();
	// keyword_void_def_cpp (1);
	keyword_void_def ();
	// keyword_void_def (1);

	int my_int = 10;
	double my_double = 20.30;

	void * my_ptr;
	my_ptr = & my_int;
	std::cout << * static_cast <int *> (my_ptr) << std::endl;
	my_ptr = & my_double;
	std::cout << * static_cast <double *> (my_ptr) << std::endl;

	my_ptr = (void *) & keyword_void_def;

	(* reinterpret_cast<void (*)()>(my_ptr))();

	class C {
	public:
		int memb;
		inline void print () {
			std::cout << "memb: " << memb << std::endl;
		}
	};

	C c;
	c.memb = 123;

	my_ptr = & c.memb;
	std::cout << * static_cast <int *> (my_ptr) << std::endl;

	void (C::* my_ptr2) () = & C::print;
	(c.* my_ptr2) ();
}



void keyword_virtual () {

	class A {
	public:
		inline void print1 () {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
		inline virtual void print2 () {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
	};
	class B : virtual public A {
	public:
		inline void print1 () {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
		inline virtual void print2 () override {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
	};

	B b;
	A * ap = & b;

	ap->print1 ();
	ap->print2 ();

	class C : virtual public A {
	public:
		inline void print1 () {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
		inline virtual void print2 () override {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
	};

	class D : public B, public C {
	public:
		inline virtual void print2 () override {
			this->C::print2 ();
			// static_cast <C *> (this)->print2 (); // infinite recursion ))
		}
	};

	D d;
	ap = & d;
	ap->print1();
	ap->print2();
}



void keyword_this () {
	struct S {
		int val;
		inline void print () {
			std::cout << val << std::endl;
			std::cout << this->val << std::endl;
			std::cout << (* this).val << std::endl;
		}
	};

	S s = {.val = 111};

	s.print ();
}



void keywords_try_throw_catch () {
	class E : public std::exception {
	public:
		inline virtual const char * what () const noexcept override {
			return "my exception E";
		}
	};
	try {
		throw E {};
	}
	catch (const E & e) {
		std::cerr << "E: " << e.what () << std::endl;
	}
	catch (...) {
		std::cerr << "exception thrown" << std::endl;
		throw;
	}
}



namespace keyword_friend {
class Printer2 {
public:
	template <typename T>
	inline static void print1 (const T & c) {
		std::cout << c.m_value << std::endl;
	}
	template <typename T>
	inline static void print2 (const T & c) {
		std::cout << c.m_value << std::endl;
	}
};

class C {
	friend void print (const C & c);
	friend class Printer;

	friend void Printer2::print1 <C> (const C &);

public:
	explicit C (int value)
		: m_value (value)
	{}
private:
	int m_value;
};

void print (const C & c) {
	std::cout << c.m_value << std::endl;
}

class Printer {
public:
	inline static void print (const C & c) {
		std::cout << c.m_value << std::endl;
	}
};

template <typename T>
class C2 : public C {
	friend T;
public:
	C2 (int value)
		: C (value), m_value2 (value)
	{}

private:
	int m_value2;
};

class Printer3 {
public:
	static void print (const C2 <Printer3> & c2) {
		// std::cout << c2.m_value << std::endl; // error
		std::cout << c2.m_value2 << std::endl;
	}
};

void run () {
	C c (123);
	print (c);
	Printer::print (c);
	Printer2::print1 (c);
	// Printer2::print2 (c); // error
	C2 <Printer3> c2 (123);
	Printer3::print (c2);
}
}



namespace keyword_typename {
template <typename T>
auto decay_copy (T && r) -> typename std::decay <T>::type;

template <typename V, template <typename T> typename U>
requires requires (U <V> x) {
	{ x.size () } -> std::convertible_to <std::size_t>;
}
std::size_t get_size (const U <V> & c) {
	return c.size ();
}

void run () {
	int x = 111;
	std::cout << decay_copy (x) << std::endl;
	std::cout << get_size (std::vector <int> {1, 2, 3}) << std::endl;
}

template <typename T>
auto decay_copy (T && r) -> typename std::decay <T>::type {
	return std::forward <T> (r);
};
}



void keyword_explicit () {
	class A {
	public:
		A (int) {}
		A (std::initializer_list <int>) {};
		operator int () { return 0; }
	};

	class B {
	public:
		explicit B (int) {}
		explicit B (std::initializer_list <int>) {};
		explicit operator int () { return 0; }
	};

	class C {
	public:
		static A a1 () { return A (1); }
		static A a2 () { return A {1, 2, 3}; }
		static A a3 () { return 1; }
		static A a4 () { return {1, 2, 3}; }
		static B b1 () { return B (1); }
		static B b2 () { return B {1, 2, 3}; }
		// static B b3 () { return 1; } // error
		// static B b4 () { return {1, 2, 3}; } // error
	};

	C::a1 ();
	C::a2 ();
	C::a3 ();
	C::a4 ();
	C::b1 ();
	C::b2 ();

	int x;
	A a (1);
	B b (1);

	x = a;
	x = static_cast <int> (a);

	// x = b; // error
	x = static_cast <int> (b);

	(void) x;
}



namespace keyword_sizeof {
template <typename ... Args>
constexpr std::size_t count (Args && ...) {
	return sizeof ... (Args);
}

void run () {
	unsigned a [100];
	std::cout << sizeof a << std::endl;
	std::memset (a, 0, sizeof a);

	std::cout << count (1) << std::endl;
	std::cout << count (1, 2) << std::endl;
	std::cout << count (1, 2, 3) << std::endl;
	std::cout << sizeof (unsigned) << std::endl;
}
}



namespace keyword_noexcept {
void foo () { throw std::runtime_error ("err"); }
void bar () {}
struct S {};

void f1 () {}
void f2 () noexcept {}
void f3 () noexcept (false) {}
void f4 () noexcept (true) {}
void f5 () noexcept (noexcept (f1 ())) {}
void f6 () noexcept (noexcept (f2 ())) {}

void g1 () {}
void g2 () noexcept {}

void run () {
	std::cout << noexcept (foo ()) << std::endl;
	std::cout << noexcept (bar ()) << std::endl;
	std::cout << noexcept (1 + 1) << std::endl;
	std::cout << noexcept (S ()) << std::endl;
	std::cout << noexcept (f1 ()) << std::endl;
	std::cout << noexcept (f2 ()) << std::endl;
	std::cout << noexcept (f3 ()) << std::endl;
	std::cout << noexcept (f4 ()) << std::endl;
	std::cout << noexcept (f5 ()) << std::endl;
	std::cout << noexcept (f6 ()) << std::endl;

	void (* p1) () = & g1;
	void (* p2) () noexcept = & g2;

	std::cout << noexcept (p1 ()) << std::endl;
	std::cout << noexcept (p2 ()) << std::endl;

	p1 = & g2; // noexcept (true) to noexcept (false) conversion is allowed
	// p2 = & g1; // error:  noexcept (false) to noexcept (true) conversion is'nt allowed
}
}



int main () {
	keyword_void ();
	std::cout << "\n====================\n" << std::endl;
	keyword_virtual ();
	std::cout << "\n====================\n" << std::endl;
	keyword_this ();
	std::cout << "\n====================\n" << std::endl;
	try { keywords_try_throw_catch (); } catch (...) {}
	std::cout << "\n====================\n" << std::endl;
	keyword_friend::run ();
	std::cout << "\n====================\n" << std::endl;
	keyword_typename::run ();
	std::cout << "\n====================\n" << std::endl;
	keyword_explicit ();
	std::cout << "\n====================\n" << std::endl;
	keyword_sizeof::run ();
	std::cout << "\n====================\n" << std::endl;
	keyword_noexcept::run ();
	std::cout << "\n====================\n" << std::endl;

	keyword_asm ();
}
