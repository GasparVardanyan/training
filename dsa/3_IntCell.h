# ifndef IntCell_H_3
# define IntCell_H_3

class IntCell {
public:
	explicit IntCell (int initialValue = 0);
	int read () const;
	void write (int x);

private:
	int m_storedValue;
};

# endif // IntCell_H_3
