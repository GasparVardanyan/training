# include <iostream>
# include <ostream>

namespace hello {
	struct Vector1 {
		int x;
		int y;
		int z;
	};

	class Vector2 {
	public:
		int x;
		int y;
		int z;
	};

	void func () {
		Vector1 v1;
		Vector2 v2;

		struct Vector1 v3;
		class Vector2 v4;

		(void) v1;
		(void) v2;
		(void) v3;
		(void) v4;
	}
}

namespace final_classes {
	struct A {};
	struct B final {};
	struct C : A {};
	// struct D : B {}; // error: Base 'B' is marked 'final'
	struct E final : A {};
}

namespace access_specifiers {
	struct MyStruct {
		int x;
	};

	class MyClass {
		int x;
		void f () {
			(void) x; // suppress the error of the unused member caused by -Werror
		}
	};

	class MyClass1 {
	public:
		int x;
	};

	struct MyStruct1 {
		int x () const noexcept { return m_x; }
		void setX (int x) noexcept { m_x = x; }

	private:
		int m_x;
	};

	struct Plus2Base {
		int value () noexcept { return calculateValue () + 2; }
	protected:
		virtual int calculateValue () noexcept = 0;
	};

	struct FortyTwo : Plus2Base {
	protected:
		int calculateValue () noexcept final override {
			return 40;
		}
	};

	class MyClass2 {
		friend void setX (MyClass2 & c, int x);
		friend int getX (const MyClass2 & c);
	private:
		int x;
	};

	void setX (MyClass2 & c, int x) {
		c.x = x;
	}

	int getX (const MyClass2 & c) {
		return c.x;
	}

	void myFunc () {
		MyStruct s;
		MyClass c;			(void) c;
		MyClass1 c1;
		s.x = 10;
		// c.x = 20; // error: 'x' is a private member of 'access_specifiers::MyClass'
		c1.x = 10;

		MyStruct1 s1;
		s1.setX (10);

		std::cout << s1.x () << std::endl;

		MyClass2 c2;
		setX (c2, 222);
		std::cout << getX (c2) << std::endl;

		std::cout << FortyTwo {}.value () << std::endl;
	}
}

namespace inheritance {
	struct A {
	public:
		int p1;
	protected:
		int p2;
	private:
		int p3; 			void _p3(){(void)p3;}
	};

	struct B1 : public A {
		void f () {
			p1 = 10;
			p2 = 20;
			// p3 = 30; // error
		}
	};

	struct B2 : protected A {
		void f () {
			p1 = 10;
			p2 = 20;
			// p3 = 30; // error
		}
	};

	struct B3 : private A {
		void f () {
			p1 = 10;
			p2 = 20;
			// p3 = 30; // error
		}
	};

	void f () {
		B1 b1;
		b1.p1 = 10;
		// b1.p2 = 10; // error
		// b1.p3 = 10; // error
		B2 b2;
		// b2.p1 = 10; // error
		// b2.p2 = 10; // error
		// b2.p3 = 10; // error
		B3 b3;
		// b2.p1 = 10; // error
		// b2.p2 = 10; // error
		// b2.p3 = 10; // error

		(void) b2;
		(void) b3;
	}
}

namespace friendship {
	class Animal {
	private:
		double weight = 22;
		// double height;

		friend void printWeight (Animal);
		friend class AnimalPrinter;

		friend std::ostream & operator<< (std::ostream & os, Animal animal) {
			os << animal.weight;
			return os;
		}
	};

	void printWeight (Animal animal) {
		std::cout << animal.weight << std::endl;
	}

	class AnimalPrinter {
	public:
		void print (Animal animal) {
			std::cout << animal.weight << std::endl;
		}
	};

	void func () {
		Animal a;
		printWeight (a);
		AnimalPrinter ap;
		ap.print (a);

		std::cout << a << std::endl;
	}
}

int main () {
	access_specifiers::myFunc ();
	std::cout << "====================" << std::endl;
	friendship::func ();
}
