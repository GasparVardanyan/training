# include <cstddef>
# include <iostream>

namespace static_member_functions {
	typedef int Fn (int);

	int MyFn (int i) { return 2 * i; }

	class Class {
		public:
			static int Static (int i) { return 3 * i; }
	};

	void run () {
		Fn * fn;

		fn = & MyFn;

		std::cout << fn (3) << std::endl;

		fn = & Class::Static;

		std::cout << fn (3) << std::endl;
	}
}

namespace member_functions {
	typedef int Fn (int);

	class Class {
		public:
			int A (int a) { return 2 * a; }
			int B (int b) { return 3 * b; }
	};

	void run () {
		Class c;
		Class * p = & c;

		// Fn Class::* fn;
		int (Class::* fn) (int);

		fn = & Class::A;

		std::cout << (c.*fn) (3) << std::endl;
		std::cout << (p->*fn) (3) << std::endl;

		fn = & Class::B;
		std::cout << (c.*fn) (3) << std::endl;
		std::cout << (p->*fn) (3) << std::endl;
	}
}

namespace member_variables {
	class Class {
		public:
			int x;
	};

	void run () {
		Class c { .x = 10 };
		std::cout << c.x << std::endl;
		Class * p = & c;

		const int * p_i;
		p_i = & c.x;
		std::cout << * p_i << std::endl;
		p_i = & p->x;
		std::cout << * p_i << std::endl;

		const int Class::* cp = & Class::x;

		int i = c.*cp;
		std::cout << i << std::endl;
		i = p->*cp;
		std::cout << i << std::endl;

		p_i = & (c.*cp);
		std::cout << * p_i << std::endl;
		p_i = & (p->*cp);
		std::cout << * p_i << std::endl;

		std::size_t o = offsetof (Class, x);
		i = * reinterpret_cast <int *> (reinterpret_cast <char *> (& c) + o);
		std::cout << i << std::endl;
	}
}

namespace static_member_variables {
	class Class {
		public:
			static inline int i = 10;
			static int j;
	};

	int Class::j = 20;

	void run () {
		int k = 3;
		int * p;
		p = & k;
		std::cout << * p << std::endl;
		p = & Class::i;
		std::cout << * p << std::endl;
		p = & Class::j;
		std::cout << * p << std::endl;
	}
}

int main () {
	static_member_functions::run ();
	std::cout << "====================" << std::endl;
	member_functions::run ();
	std::cout << "====================" << std::endl;
	member_variables::run ();
	std::cout << "====================" << std::endl;
	static_member_variables::run ();
}
