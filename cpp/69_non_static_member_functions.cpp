# include <iostream>

// cppcheck-suppress-begin functionStatic
// NOLINTBEGIN(readability-convert-member-functions-to-static)

namespace {

class Class {
public:
	void member_function () {}
	void defined_inside () {};
	void defined_outside ();
};

struct CVQualifiers {
	void func () { std::cout << "func()\n"; }
	void func () const { std::cout << "func() const\n"; }
	void cv_only () const volatile { std::cout << "cv_only () const volatile\n"; }
};

struct RefQualifiers {
	void func () & { std::cout << "func () &\n"; }
	void func () && { std::cout << "func () &&\n"; }
};

struct CVRefQualifiers {
	void func () const & { std::cout << "func () const &\n"; }
	void func () && { std::cout << "func () &&\n"; }
};

struct Base {
	virtual ~Base () {}
	virtual void func () { std::cout << "Base::func ()\n"; }
};

struct Derived : Base {
	virtual ~Derived () override {} // cppcheck-suppress uselessOverride
	virtual void func () override { std::cout << "Derived::func ()\n"; }
};

} // end anonymous namespace

void Class::defined_outside () {}

// NOLINTEND(readability-convert-member-functions-to-static)
// cppcheck-suppress-end functionStatic

int main () {
	Class instance;
	instance.member_function ();

	CVQualifiers cvq;
	cvq.func ();
	const CVQualifiers cvq2;
	cvq2.func ();

	cvq.cv_only ();
	cvq2.cv_only ();

	RefQualifiers rq;
	rq.func ();

	RefQualifiers {}.func ();

	const CVRefQualifiers cvrq;
	cvrq.func ();

	CVRefQualifiers {}.func ();

	Base b;
	Derived d;

	Base * bp = & b;
	bp->func ();
	bp = & d;
	bp->func ();
}
