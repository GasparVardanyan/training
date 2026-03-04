# include <iostream>

void lambdas () {
	int a = 0;

	auto bad_counter = [a] () {
		// return a++; // error
		(void) a;
	};

	(void) bad_counter;

	auto good_counter = [a] () mutable {
		return a++;
	};

	bad_counter ();

	std::cout << good_counter () << std::endl;
	std::cout << good_counter () << std::endl;
	std::cout << good_counter () << std::endl;
	std::cout << a << std::endl;
}

class MyClass {
public:
	double get_val () const {
		if (false == m_calculated) {
			m_privateValue = 10;
			m_calculated = true;
		}

		return m_privateValue;
	}

private:
	mutable bool m_calculated = false;
	mutable double m_privateValue = -1;
};

void class_members () {
	MyClass c;
	std::cout << c.get_val () << std::endl;
}

int main () {
	lambdas ();
	class_members ();
}
