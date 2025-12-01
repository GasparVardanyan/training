# include "2_IntCell.h"

IntCell::IntCell (int initialValue)
	: m_storedValue (initialValue) {}

int IntCell::read () const {
	return m_storedValue;
}

void IntCell::write (int x) {
	m_storedValue = x;
}
