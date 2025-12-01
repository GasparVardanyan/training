# include <iostream>

class IntCell
{
public:
	IntCell () {
		m_storedValue = 0;
		std::cout << "IntCell ()" << std::endl;
	}

	IntCell (int initialValue) {
		m_storedValue = initialValue;
		std::cout << "IntCell (int)" << std::endl;
	}

	IntCell & operator= (const IntCell & other) {
		m_storedValue = other.m_storedValue;
		std::cout << "IntCell & operator=" << std::endl;
		return * this;
	}

	int read () {
		return m_storedValue;
	}

	void write (int x) {
		m_storedValue = x;
	}

private:
	int m_storedValue;
};

class IntCell1
{
public:
	explicit IntCell1 (int initialValue = 0)
		: m_storedValue {initialValue} {}

	int read () const {
		return m_storedValue;
	}

	void write (int x) {
		m_storedValue = x;
	}

private:
	int m_storedValue;
};

int main ()
{
	IntCell x = 10;
	std::cout << "====================" << std::endl;
	x = 20;
	std::cout << "====================" << std::endl;



	IntCell1 y = (IntCell1) 20;
	IntCell1 z (20);
	IntCell1 w { 20 };
	std::cout << "Hello, World!" << std::endl;
	if (y.read()) {}
}
