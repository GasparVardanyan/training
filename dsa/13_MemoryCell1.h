# ifndef MEMORYCELL1_H_13
# define MEMORYCELL1_H_13

template <typename Object>
class MemoryCell1
{
public:
	explicit MemoryCell1 (const Object & initialValue = {});
	const Object & read () const;
	void write (const Object & v);

private:
	Object m_storedValue;
};

# include "13_MemoryCell1.cpp"

# endif // MEMORYCELL1_H_13
