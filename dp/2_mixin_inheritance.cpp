# include <iostream>
# include <iterator>

template <typename T>
concept IterableInterface = requires (T t, const T tc) {
	t.begin ();
	t.end ();
	tc.begin ();
	tc.end ();
	tc.cbegin ();
	tc.cend ();
};

template <IterableInterface DataCarrier>
class Printer : public DataCarrier {
public:
	void print () {
		for (const auto & item : * this) {
			std::cout << item << std::endl;
		}
	}
};

class DataCarrier {
public:
	auto begin () { return std::begin (m_data); };
	auto end () { return std::end (m_data); };
	auto begin () const { return std::begin (m_data); };
	auto end () const { return std::end (m_data); };
	auto cbegin () const { return std::begin (m_data); };
	auto cend () const { return std::end (m_data); };

private:
	int m_data [5] = {1, 2, 3, 4, 5};
};

int main () {
	Printer <DataCarrier> d;
	d.print ();
}
