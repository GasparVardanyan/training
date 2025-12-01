# ifndef IntCell_H_2
# define IntCell_H_2

class IntCell {
public:
	explicit IntCell (int initialValue = 0);
	int read () const;
	void write (int x);

private:
	int m_storedValue;
};

# endif // IntCell_H_2
