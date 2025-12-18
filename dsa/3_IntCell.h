# ifndef INTCELL_H_3
# define INTCELL_H_3

class IntCell {
public:
	explicit IntCell (int initialValue = 0);
	IntCell(const IntCell &) = default;
	IntCell(IntCell &&) = default;
	IntCell &operator=(const IntCell &) = default;
	IntCell &operator=(IntCell &&) = default;
	int read() const;
	void write (int x);

private:
	int m_storedValue;
};

# endif // INTCELL_H_3
