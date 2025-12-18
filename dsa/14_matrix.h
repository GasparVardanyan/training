# ifndef MATRIX_H_14
# define MATRIX_H_14

# include <cstddef>
# include <utility>
# include <vector>

template <typename Object>
class Matrix
{
public:
	Matrix (std::size_t rows, std::size_t cols)
		: m_data (rows)
	{
		for (auto & row : m_data) {
			row.resize (cols);
		}
	}

	explicit Matrix (const std::vector <std::vector <Object>> & v)
		: m_data (v)
	{}

	explicit Matrix (std::vector <std::vector <Object>> && v)
		: m_data (std::move (v))
	{}

	const std::vector <Object> & operator[] (std::size_t row) const {
		return m_data.at (row);
	}

	std::vector <Object> & operator[] (std::size_t row) {
		return m_data [row];
	}

	std::size_t numrows () const {
		return m_data.size ();
	}

	std::size_t numcols () const {
		if (0 != m_data.size ()) {
			return m_data.at (0).size ();
		}
		else {
			return 0;
		}
	}

private:
	std::vector<std::vector<Object>> m_data;
};

# endif // MATRIX_H_14
