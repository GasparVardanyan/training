struct Outer {
	struct Inner;
	struct Inner2;

	Inner2 * i2p = nullptr;

	friend int f1 (Outer, Inner);

	struct Inner {
		// friend struct Outer;
		friend int f2 (Outer, Inner);

		int func (Outer o) {
			// int a = m_x; // error
			int b = sizeof m_x;
			int c = o.m_x;
			return b * c * m_y;
		}

	private:
		int m_y = 2;
	};

	Inner i;

	int func () {
		// int a = m_y; // error
		// int b = sizeof (m_y); // error
		// int c = i.m_y; // error
		// int d = sizeof Inner::m_y; // error

		(void) i;
		return 0;
	}

private:
	struct Inner3 {};
	struct Inner4 {};

public:
	Inner3 i3 () { return {}; }
	using Inner4 = Inner4;

private:
	int m_x = 0;
};

Outer o;
Outer::Inner i = o.i;

struct Outer::Inner2 {
	int x;
};

Outer::Inner2 i2 { .x = 20 };



int f1 (Outer o, Outer::Inner i) {
	int a = o.m_x;
	// int b = i.m_y; // error

	(void) i;
	return a;
}

int f2 (Outer o, Outer::Inner i) {
	// int a = o.m_x; // error
	int b = i.m_y;

	(void) o;
	return b;
}

struct BaseOuter {
	BaseOuter(const BaseOuter &) = delete;
	BaseOuter(BaseOuter &&) = default;
	BaseOuter &operator=(const BaseOuter &) = delete;
	BaseOuter &operator=(BaseOuter &&) = default;
	virtual ~BaseOuter () = default;

	struct BaseInner_ {
		  BaseInner_(const BaseInner_ &) = delete;
		  BaseInner_(BaseInner_ &&) = default;
		  BaseInner_ &operator=(const BaseInner_ &) = delete;
		  BaseInner_ &operator=(BaseInner_ &&) = default;
		  virtual ~BaseInner_ () = default;

		  virtual void do_something();
		  virtual void do_something_else();
	} b_in;

public:
	typedef BaseInner_ Inner;
};

int main () {
	o.i3 ();
	// Outer::Inner3 x = o.i3 (); // error

	auto y = o.i3 ();				(void) y;
	using T = decltype (o.i3 ());

	T z = o.i3 ();							(void) z;

	Outer::Inner4 i4;						(void) i4;
}
