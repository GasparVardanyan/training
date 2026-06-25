# include <cstddef>
# include <initializer_list>
# include <iomanip>
# include <iostream>
# include <string>
# include <vector>

namespace {

namespace arithmetic_operators {

struct S {
	int x;

	inline S operator+ (const S & other) const {
		return { .x = x + other.x };
	}

	inline S & operator+= (const S & other) {
		x += other.x;
		return * this;
	}

	// NOLINTNEXTLINE(llvm-prefer-static-over-anonymous-namespace)
	friend std::ostream & operator<< (std::ostream & os, const S & s) {
		return os << "S1 { .x = " << s.x << " }";
	}
};

// NOLINTNEXTLINE(llvm-prefer-static-over-anonymous-namespace)
S operator* (const S & s1, const S & s2) {
	return { .x = s1.x * s2.x };
}

// NOLINTNEXTLINE(llvm-prefer-static-over-anonymous-namespace)
S & operator *= (S & instance, const S & other) {
	instance.x *= other.x;
	return instance;
}


// NOLINTNEXTLINE(llvm-prefer-static-over-anonymous-namespace)
void run () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

	S s1 { .x = 10 };
	S s2 { .x = 20 };

	std::cout << s1 + s2 << '\n';
	std::cout << s1 * s2 << '\n';

	s1 += s2;
	s2 *= s1;

	std::cout << s2 << std::endl;

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

} // end namespace arithmetic_operators

namespace array_subscript_operator {

struct S {
	explicit S (size_t s)
		: ivec (std::vector <int> (s))
	{}

	explicit S (std::initializer_list <int> init)
		: ivec (std::vector <int> (init))
	{}

	inline int & operator [] (std::size_t i) {
		return ivec [i];
	}

	inline const int & operator [] (std::size_t i) const {
		return ivec [i];
	}

	// NOLINTNEXTLINE(hicpp-named-parameter,readability-named-parameter)
	inline void operator[] (const std::string &) {
		std::cout << "what ??\n";
	}

private:
	std::vector <int> ivec;
};

struct S_ {};
// error: 1. clang: Overloaded 'operator[]' must be a non-static member function [operator_overload_must_be_member]
// void operator[] (const S_ & instance, std::size_t i) {}

// NOLINTNEXTLINE(llvm-prefer-static-over-anonymous-namespace)
void run () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

	S s { 1, 2, 3 };
	s [0] = 10;
	const S & sr = s;
	std::cout << sr [0] << '\n';

	s ["hello"];

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

} // end namespace array_subscript_operator

namespace multiple_array_subscript_operators {

template <typename T>
class matrix {
	template <bool ConstRef_ = false>
	class matrix_proxy {
	public:
		explicit matrix_proxy (std::vector <T> & row)
			: m_row (row) {}

		int & operator[] (std::size_t i)
			requires (false == ConstRef_)
		{
			return m_row [i];
		}

		const int & operator[] (std::size_t i) const {
			return m_row [i];
		}

	private:
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
		std::vector <T> & m_row;
	};

public:
	matrix (std::size_t cols, std::size_t rows)
		: m_cols (cols)
		, m_rows (rows)
	{
		m_data.resize (rows);
		for (auto & row : m_data) {
			row.resize (cols);
		}
	}

	matrix_proxy <false> operator[] (std::size_t i) {
		return matrix_proxy <false> (m_data [i]);
	}

	matrix_proxy <true> operator[] (std::size_t i) const {
		return matrix_proxy <true> (const_cast <matrix *> (this)->m_data [i]);
	}

private:
	std::vector <std::vector <T>> m_data;
	// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
	const std::size_t m_cols;
	const std::size_t m_rows;
	// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)
};

// NOLINTNEXTLINE(llvm-prefer-static-over-anonymous-namespace)
void run () {
	// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

	const std::size_t cols = 10;
	const std::size_t rows = 5;

	matrix <int> m (cols, rows);

	for (std::size_t r = 0; r < rows; r++) {
		for (std::size_t c = 0; c < cols; c++) {
			m [r] [c] = static_cast <int> ((r + 1) * 100 + c + 1);
		}
	}

	const matrix <int> & mr = m;
	for (std::size_t r = 0; r < rows; r++) {
		for (std::size_t c = 0; c < cols; c++) {
			std::cout << std::setw (4) << std::setfill ('0') << mr [r] [c] << ", ";
		}
		std::cout << '\n';
	}

	// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

} // end namespace multiple_array_subscript_operators

} // end anonymous namespace

int main () {
	arithmetic_operators::run ();
	std::cout << "====================\n";
	array_subscript_operator::run ();
	std::cout << "====================\n";
	multiple_array_subscript_operators::run ();
}
