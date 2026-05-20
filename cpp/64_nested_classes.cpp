# include <iostream>
# include <memory>
# include <type_traits>

namespace {

namespace nested_classes {

struct Outer {
	struct Inner;

	Inner * ip;

	struct Inner {
		void do_something1 () {}
		void do_something2 ();
	};

	struct Inner2;

	// void Inner::do_something2 () {} // error

	Inner in;
	Inner2 * i2p;
};

static void run ();

} // end namespace nested_classes

struct nested_classes::Outer::Inner2 {};
void nested_classes::Outer::Inner::do_something2 () {}

void nested_classes::run () {
	Outer o {};
	Outer::Inner & i = o.in;
	o.ip = & i;
}



namespace nested_class_member_access1 {

struct Outer {
	struct Inner;

	friend void print1 (const Outer & o, const Inner & i);
	struct Inner {
		friend void print2 (const Outer & o, const Inner & i);

		void set (Outer & outer, int value) {
			outer.m_value = value;
		}

		int get (const Outer & outer) {
			return outer.m_value;
		}

		void v () {
			std::cout << sizeof (m_value_inner) << '\n';
		}

	private:
		int m_value_inner = 21;
	};

	// void set (Inner & inner, int value) { // error
	// 	inner.m_int = value;
	// }

	// int get (const Inner & inner) { // error
	// 	return inner.m_int;
	// }

private:
	int m_value;
};

void print1 (const Outer & o, const Outer::Inner & i) {
	std::cout << "print1: " << o.m_value << '\n';
	// std::cout << "print1: " << i.m_value_inner << '\n'; // error
	(void) i;

}

void print2 (const Outer & o, const Outer::Inner & i) {
	// std::cout << "print1: " << o.m_value << '\n';
	std::cout << "print1: " << i.m_value_inner << '\n'; // error
	(void) o;
}

static void run () {
	Outer o {};
	Outer::Inner i {};
	i.set (o, 222);
	std::cout << i.get (o) << '\n';
	i.v ();
	print1 (o, i);
	print2 (o, i);
}

} // end namespace nested_class_member_access1



namespace nested_class_member_access2 {

struct Outer {
private:
	struct Inner;

public:
	using Inner_ = Inner;
	static Inner_ in () { return {}; }

private:
	struct Inner {
		void func () { std::cout << "Outer::Inner::func()\n"; }
	};
};

static_assert (std::is_same_v <Outer::Inner_, decltype (Outer::in ())>);

void run () {
	Outer o {};
	auto oi = o.in ();
	oi.func ();
	Outer::in ().func ();

	decltype (Outer::in ()) oi2;
	oi2 = Outer::in ();
	Outer::Inner_ oi3 = oi2;
	oi3.func ();
}

} // end namespace nested_class_member_access2



namespace nested_class_derivation {
struct Base {};

struct Outer {
	struct Inner {};
};

struct Derived : Outer::Inner {};



struct BaseOuter {
	BaseOuter (const BaseOuter &) = delete;
	BaseOuter & operator= (BaseOuter &) = delete;
	BaseOuter (const BaseOuter &&) = delete;
	BaseOuter & operator= (BaseOuter &&) = delete;
	virtual ~BaseOuter () = default;

	BaseOuter () = default;

private:
	struct BaseInner_ {
		BaseInner_ (const BaseInner_ &) = delete;
		BaseInner_ & operator= (BaseInner_ &) = delete;
		BaseInner_ (const BaseInner_ &&) = delete;
		BaseInner_ & operator= (BaseInner_ &&) = delete;
		virtual ~BaseInner_ () = default;

		BaseInner_ () = default;

		virtual void do_something () { std::cout << "do_something 1\n"; };
		virtual void do_something_else () { std::cout << "do_something_else 1\n"; }
	} b_in;

public:
	using Inner = BaseInner_;
	virtual Inner & getInner () { return b_in; }
};

struct DerivedOuter : public BaseOuter {
private:
	struct DerivedInner_ : BaseOuter::Inner {
		virtual void do_something () override { std::cout << "do_something 2\n"; }
		virtual void do_something_else () override { std::cout << "do_something_else 2\n"; };
	} d_in;

public:
	using Inner = DerivedInner_;
	virtual Inner & getInner () override { return d_in; }
};

void run () {
	auto b = std::make_unique <BaseOuter> ();
	BaseOuter::Inner & bin = b->getInner ();
	bin.do_something ();
	b->getInner ().do_something_else ();

	auto d = std::make_unique <DerivedOuter> ();
	BaseOuter::Inner & din = d->getInner ();
	din.do_something ();
	din.do_something_else ();
}

} // end namespace nested_class_derivation

} // end anonymous namespace



int main () {
	nested_classes::run ();
	nested_class_member_access1::run ();
	nested_class_member_access2::run ();
	nested_class_derivation::run ();
}
