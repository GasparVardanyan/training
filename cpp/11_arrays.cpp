# include <algorithm>
# include <cstddef>
# include <initializer_list>
# include <iostream>
# include <iomanip>
# include <memory>
# include <stdexcept>
# include <type_traits>
# include <vector>

template <typename T>
class Array2D {
public:
	Array2D (std::size_t rows, std::size_t cols)
		: m_rows (rows)
		, m_cols (cols)
	{
		std::size_t r = 0;
		try {
			m_data = new T* [rows];
			for (; r < rows; r++) {
				m_data [r] = new T [cols];
			}
		} catch (...) {
			for (std::size_t _r = 0; _r < r; _r++) {
				delete [] m_data [_r];
			}
			delete [] m_data;

			m_rows = 0;
			m_cols = 0;

			throw;
		}
	}

	Array2D (const Array2D <T> & array2d)
		: m_rows (array2d.m_rows)
		, m_cols (array2d.m_cols)
	{
		std::size_t r = 0;
		try {
			m_data = new T* [m_rows];
			for (; r < m_rows; r++) {
				m_data [r] = new T [m_cols];
				std::copy (array2d.m_data [r], array2d.m_data [r] + m_cols, m_data [r]);
			}
		} catch (...) {
			for (std::size_t _r = 0; _r < r; _r++) {
				delete [] m_data [_r];
			}
			delete [] m_data;

			m_rows = 0;
			m_cols = 0;

			throw;
		}
	}

	Array2D (Array2D <T> && array2d) noexcept {
		m_rows = array2d.m_rows;
		m_cols = array2d.m_cols;
		m_data = array2d.m_data;
		array2d.m_data = nullptr;
		array2d.m_rows = 0;
		array2d.m_cols = 0;
	}

	Array2D & operator= (const Array2D <T> & array2d) {
		Array2D <T> _array2d (array2d);
		std::swap (* this, _array2d); // not optimal

		return * this;
	}

	Array2D & operator= (Array2D && array2d) noexcept {
		std::swap (m_rows, array2d.m_rows);
		std::swap (m_cols, array2d.m_cols);
		std::swap (m_data, array2d.m_data);

		return * this;
	}

	virtual ~Array2D () {
		if (nullptr != m_data) {
			for (std::size_t r = 0; r < m_rows; r++) {
				delete [] m_data [r];
			}
			delete [] m_data;
		}
	}

private:
	T** m_data;
	std::size_t m_rows;
	std::size_t m_cols;
};

void matrices ()
{
	int arr [5] = {}; // NOTE: not a valid C
	if (arr [0]) {
		std::cout << "smth is wrong" << std::endl;
	}

	int const n_rows = 3;
	int const n_cols = 7;

	int m [n_rows] [n_cols] = {
		{  1,   2,   3,   4,   5,   6,   7, },
		{  8,   9,   10,  11,  12,  13,  14, },
		{  15,  16,  17,  18,  19,  20,  21, },
	};

	for (int y = 0; y < n_rows; y++) {
		for (int x = 0; x < n_cols; x++) {
			std::cout << std::setw (4) << m [y] [x] << std::endl;
		}
	}
}

void array2d ()
{
	Array2D<int> mtx (10, 10);
	Array2D<int> mtx2 (mtx);
	mtx = mtx2;
	Array2D<int> mtx3 (Array2D<int> (10, 20));
	std::swap (mtx2, mtx3);

	std::cout << "====================" << std::endl;

	std::vector <Array2D<int>> a;
	a.push_back (Array2D <int> (10, 10));
}

template <typename T>
class Matrix {
private:
	std::vector <T> items_;
	std::size_t n_cols_;

	std::size_t index_for (std::size_t x, std::size_t y) const {
		return y * n_cols_ + x;
	}

public:
	std::size_t n_cols () const {
		return n_cols_;
	}
	std::size_t n_rows () const {
		return items_.size () / n_cols_;
	}

	T & item (std::size_t x, std::size_t y) {
		return items_ [index_for (x, y)];
	}

	const T & item (std::size_t x, std::size_t y) const {
		return items_ [index_for (x, y)];
	}

	Matrix () : n_cols_ (0) {}

	Matrix (std::size_t n_cols, std::size_t n_rows)
		: n_cols_ (n_cols)
		, items_ (n_cols * n_rows)
	{}

	Matrix (const std::initializer_list<std::initializer_list<T>> & values)
		: n_cols_ (values.size () == 0 ? 0 : values.begin ()->size ())
	{
		for (const auto & row : values) {
			if (n_cols_ != row.size ()) {
				throw std::invalid_argument ("invalid initializer list for Matrix");
			}
			items_.insert (items_.end (), row.begin (), row.end ());
		}
	}
};

template <typename T, std::size_t n>
constexpr std::size_t nItems (T (&) [n]) noexcept { // std::size
	return n;
}

template <typename T, std::size_t n>
void getArr (const T (&) [n]) {
	std::cout << "array passed" << std::endl;
}

template <typename T>
void getArr (const T *) {
	std::cout << "pointer passed" << std::endl;
}

void matrix () {
	Matrix m {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9},
	};

	for (std::size_t i = 0; i < m.n_rows (); i++) {
		for (std::size_t j = 0; j < m.n_cols (); j++) {
			std::cout << m.item (j, i) << ' ';
		}

		std::cout << std::endl;
	}
}

int main ()
{
	matrices ();
	array2d ();

	matrix ();

	std::unique_ptr<int> arr (new int [] {11, 12, 13, 14});
	std::unique_ptr<int> arr1 (new int [5] (1, 2, 3)); // 1, 2, 3, 0, 0
	std::unique_ptr<int> arr2 (new int [5]); // initialized with indeterminate values
	std::unique_ptr<int> arr3 (new int [5] ()); // initialized with zeros

	for (int i = 0; i < 4; i++) {
		std::cout << arr.get () [i] << std::endl;
	}

	int arr4 [5][6][7];
	std::cout << nItems (arr4) << std::endl;

	std::cout << std::size (arr4) << std::endl;
	std::cout << std::extent <decltype (arr4)>::value << std::endl;
	std::cout << std::extent <decltype (arr4), 1>::value << std::endl;
	std::cout << std::extent <decltype (arr4), 2>::value << std::endl;

	{
		int * arr1 = new int [10] ;
		int arr2 [10];

		getArr (arr1);
		getArr (arr2);

		delete [] arr1;
	}
}
