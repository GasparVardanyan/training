# include <iostream>
# include <string>

namespace {

struct Example {
private:
	static inline int s_num_instances = 0; // Definition and initialization
	static int s_myInt; // Declaration
	int m_i; // Declaration

public:
	static std::string s_static_str; // Definition
	static inline int static_func () { return {}; }

	inline Example () {
		++s_num_instances;
		(void) s_myInt;
	}

	inline void set_str (const std::string & str);

};

void Example::set_str (const std::string & str)
{
	(void) str;
	(void) m_i;
}

int Example::s_myInt; // Definition
std::string Example::s_static_str = "hello"; // Definition and initialization



struct ForwardDeclared;

struct ExIncomplete {
	static ForwardDeclared fd;
	static ExIncomplete i_contain_myself;
	static int arr [];

	void v () {
		(void) fd;
		(void) i_contain_myself;
		(void) arr;
	}
};

struct ForwardDeclared {};

ForwardDeclared ExIncomplete::fd;
ExIncomplete ExIncomplete::i_contain_myself;
int ExIncomplete::arr [10];



struct ExConst {
	enum class E { VAL = 5 };

	static const int ci = 5;
	static const E ce = E::VAL;

	// static const double cd1 = 5; // error
	static const inline double cd2 = 5;
	static constexpr double cd3 = 5;
	static const double cd4;

	void v () {
		(void) ci;
		(void) ce;
		(void) cd2;
		(void) cd3;
		(void) cd4;
	}
};

const double ExConst::cd4 = 5;



struct ConstexprConstructible1 {
};

struct ConstexprConstructible2 {
	constexpr ConstexprConstructible2 () {}
};

struct ExConstexpr {
	constexpr static int ci = 5;
	constexpr static double cd = 5;
	constexpr static int carr [] = {1, 2, 3};
	constexpr static ConstexprConstructible1 c1 {};
	constexpr static ConstexprConstructible2 c2 {};

	// constexpr static int bad_ci; // error, constexpr variables must be initialized

	void v () {
		(void) ci;
		(void) cd;
		(void) carr;
		(void) c1;
		(void) c2;
	}
};

// constexpr int ExConstexpr::bad_ci = 2;	// this doesn't help too! constexpr
											// declarations are definitions



struct ExODR {
	static const int i1 = 5;
	static const inline int i2 = 10;
	static constexpr int i3 = 100;
};

const int ExODR::i1; // (1)
static const int * ip1 = & ExODR::i1; // would fail to compile without (1)
static const int * ip2 = & ExODR::i2;
static const int * ip3 = & ExODR::i3;



struct ExMuta {
	int immuta = 0;
	mutable int muta = 1;
	static inline int i;

	void v () {
		const ExMuta em;
		// em.immuta = 100; // error
		em.muta = 100;
		em.i = 1000;
	}
};



struct ExPointer {
	void nsfunc () {}
	static void sfunc () {}
};

// void (* nsptr) () = & ExPointer::nsfunc; // error
void (ExPointer::* nsptr) () = & ExPointer::nsfunc;
// void (ExPointer::* sptr) () = & ExPointer::sfunc; // error
void (* sptr) () = & ExPointer::sfunc;

void (& sref) () = ExPointer::sfunc;
// void (ExPointer::& nsref) () = ExPointer::nsfunc;	// error, can't take reference
														// to non-static member functions
void (ExPointer::* & nsreftoptr) () = nsptr;



struct ExCVRefQualifiersAndVirtual {
	static void func () {}
	// static void cfunc () const {} // error
	// static void vfunc () volatile {} // error
	// static void cvfunc () const volatile {} // error
	// static void lrfunc () & {} // error
	// static void rrfunc () && {} // error
	// static virtual void vfunc () {} // error
};



struct {
	static void v () {}
	// static inline int i = 0;	// error. unnamed classes can't have static
								// member variables
} Foo;

void foo () {
	(void) Foo;
	struct S {
		static void v () {}
		// static inline int i = 0;	// error. local classes can't have static
									// member variables
	};
}

} // end anonymous namespace



int main () {
	Example one, two, three;
	std::cout << Example::s_static_str << '\n';
	std::cout << one.s_static_str << '\n';
	(void) ip1;
	(void) ip2;
	(void) ip3;
	(void) nsptr;
	(void) sptr;
	(void) sref;
	(void) nsreftoptr;

	foo ();
}
