# include <iostream>

class IntCell {
public:
	explicit IntCell (int value = 0)
		: m_storedValue {value} {}

	int read () const {
		return m_storedValue;
	}

	void write (int value) {
		m_storedValue = value;
	}

private:
	int m_storedValue;
};

int main ()
{
	IntCell * m;
	m = new IntCell {0};
	m->write (10);
	std::cout << "Cell contents: " << m->read () << std::endl;

	delete m;
}
