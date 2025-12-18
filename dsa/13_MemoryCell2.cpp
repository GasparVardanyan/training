# include "13_MemoryCell2.h"

template <typename Object>
MemoryCell2<Object>::MemoryCell2 (const Object & initialValue)
	: m_storedValue (initialValue)
{}

template <typename Object>
const Object & MemoryCell2<Object>::read () const {
	return m_storedValue;
}

template <typename Object>
void MemoryCell2<Object>::write (const Object & v) {
	m_storedValue = v;
}

template class MemoryCell2 <int>;
