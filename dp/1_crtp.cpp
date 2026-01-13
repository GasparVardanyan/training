# include <iostream>

template <typename T>
concept BaseDerivedInterface = requires (const T t) {
	t.begin ();
	t.end ();
	t.cbegin ();
	t.cend ();
};

template <typename Child>
struct Base {
	Base () {
		static_assert (BaseDerivedInterface <Child> && std::derived_from <Child, Base>);
	}
	void print () {
		Child * childThis = static_cast <Child *> (this);

		for (const std::decay_t <decltype (* childThis->begin ())> & data : * childThis) {
			std::cout << "data: " << data << std::endl;
		}
	}
};

struct Data : public Base <Data> {
	float * begin () { return m_data; }
	float * end () { return m_data + 10; }
	const float * cbegin () const { return m_data; }
	const float * cend () const { return m_data + 10; }
	const float * begin () const { return m_data; }
	const float * end () const { return m_data + 10; }
private:
	float m_data [10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
};

int main () {
	Data d;
	d.print ();
}
