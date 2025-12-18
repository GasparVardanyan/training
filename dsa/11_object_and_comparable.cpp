# include <iostream>
# include <ostream>
# include <vector>

class Square
{
public:
	explicit Square (double s = 0)
		: m_side {s}
	{}

	double getSide () const {
		return m_side;
	}

	double getArea () const {
		return m_side * m_side;
	}

	double getPerimeter () const {
		return 4 * m_side;
	}

	void print (std::ostream & out = std::cout) const {
		out << "(square " << getSide () << ")";
	}

	bool operator< (const Square & rhs) const {
		return m_side < rhs.m_side;
	}

private:
	double m_side;
};

std::ostream & operator<< (std::ostream & out, const Square & rhs)
{
	rhs.print (out);
	return out;
}

template <typename Comparable>
const Comparable & findMax (const std::vector <Comparable> & a) {
	std::size_t maxIndex = 0;

	for (std::size_t i = 1; i < a.size (); i++) {
		if (a [maxIndex] < a [i]) {
			maxIndex = i;
		}
	}

	return a [maxIndex];
}

int main ()
{
	std::vector <Square> v {
		Square {3},
		Square {2},
		Square {2.5},
	};

	std::cout << "Largest Square: " << findMax (v) << std::endl;
}
