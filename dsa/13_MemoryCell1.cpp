# include "13_MemoryCell1.h"

template <typename Object>
MemoryCell1<Object>::MemoryCell1 (const Object & initialValue)
	: m_storedValue (initialValue)
{}

template <typename Object>
const Object & MemoryCell1<Object>::read () const {
	return m_storedValue;
}

template <typename Object>
void MemoryCell1<Object>::write (const Object & v) {
	m_storedValue = v;
}
