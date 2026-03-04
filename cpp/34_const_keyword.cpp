# include <iostream>
# include <type_traits>



//                 _   _               _       _          _             _ _           _   _
//  _ __ ___   ___| |_| |__   ___   __| |   __| | ___  __| |_   _ _ __ | (_) ___ __ _| |_(_) ___  _ __
// | '_ ` _ \ / _ \ __| '_ \ / _ \ / _` |  / _` |/ _ \/ _` | | | | '_ \| | |/ __/ _` | __| |/ _ \| '_ \
// | | | | | |  __/ |_| | | | (_) | (_| | | (_| |  __/ (_| | |_| | |_) | | | (_| (_| | |_| | (_) | | | |
// |_| |_| |_|\___|\__|_| |_|\___/ \__,_|  \__,_|\___|\__,_|\__,_| .__/|_|_|\___\__,_|\__|_|\___/|_| |_|
//                                                               |_|

struct Bar {};

class Foo {
public:
	Bar & getBar1 () {
		return m_bar;
	}

	const Bar & getBar1 () const {
		return m_bar;
	}

	Bar & getBar2 () {
		return const_cast <Bar &> (const_cast <const Foo *> (this)->getBar2 ());
	}

	const Bar & getBar2 () const {
		return m_bar;
	}

private:
	Bar m_bar;
};

class Student {
public:
	char & getScore (bool midterm) {
		return const_cast <char &> (const_cast <const Student *> (this)->getScore (midterm));
	}
	const char & getScore (bool midterm) const {
		if (true == midterm) {
			return m_midtermScore;
		}
		else {
			return m_finalScore;
		}
	}

private:
	char m_midtermScore;
	char m_finalScore;
};

void method_deduplication () {
	Foo f1;
	const Foo f2;

	std::cout << std::is_const_v <std::remove_reference_t <decltype (f1.getBar1 ())>> << std::endl;
	std::cout << std::is_const_v <std::remove_reference_t <decltype (f2.getBar1 ())>> << std::endl;
	std::cout << std::is_const_v <std::remove_reference_t <decltype (f1.getBar2 ())>> << std::endl;
	std::cout << std::is_const_v <std::remove_reference_t <decltype (f2.getBar2 ())>> << std::endl;

	Student s1;
	s1.getScore (true) = 'B';
	s1.getScore (false) = 'A';

	const Student s2 (s1);

	std::cout << s2.getScore (true) << std::endl;
	std::cout << s2.getScore (false) << std::endl;
}




//                           _                  __                  _   _
//  _ __ ___   ___ _ __ ___ | |__   ___ _ __   / _|_   _ _ __   ___| |_(_) ___  _ __  ___
// | '_ ` _ \ / _ \ '_ ` _ \| '_ \ / _ \ '__| | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | | | | | |  __/ | | | | | |_) |  __/ |    |  _| |_| | | | | (__| |_| | (_) | | | \__ \
// |_| |_| |_|\___|_| |_| |_|_.__/ \___|_|    |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//

class MyClass {
public:
	int myInt () const { return m_myInt; }
	void setMyInt (int myInt) { m_myInt = myInt; }

private:
	int m_myInt;
};

void member_functions () {
	MyClass c1;
	c1.setMyInt (101);

	MyClass c2 (c1);
	std::cout << c2.myInt () << std::endl;
}



//  _                 _                   _       _     _
// | | ___   ___ __ _| | __   ____ _ _ __(_) __ _| |__ | | ___  ___
// | |/ _ \ / __/ _` | | \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|
// | | (_) | (_| (_| | |  \ V / (_| | |  | | (_| | |_) | |  __/\__ \
// |_|\___/ \___\__,_|_|   \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/
//

void local_variables () {
	const int a = 15;
	// a = 12; // error
	// a += 1; // error

	// int & b = a; // error
	const int & c = a;				(void) c;

	// int * d = & a; // error
	const int * e = & a;			(void) e;

	int f = 0;
	e = & f;

	// * e = 1; // error

	int * g = & f;
	* g = 1;
}



//              _       _
//  _ __   ___ (_)_ __ | |_ ___ _ __ ___
// | '_ \ / _ \| | '_ \| __/ _ \ '__/ __|
// | |_) | (_) | | | | | ||  __/ |  \__ \
// | .__/ \___/|_|_| |_|\__\___|_|  |___/
// |_|

void pointers () {
	int a = 0, b = 2;

	const int * pA = & a;
	int * const pB = & b;
	const int * const pC = & a;

	// * pA = b; // error
	pA = & b;

	* pB = b;
	// pB = & b; // error

	// * pC = b; // error
	// pc = & b; // error

	(void) pA;
	(void) pC;
}



int main () {
	method_deduplication ();
	member_functions ();
	local_variables ();
	pointers ();
}
