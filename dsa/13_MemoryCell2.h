# ifndef MemoryCell2_H_13
# define MemoryCell2_H_13

template <typename Object>
class MemoryCell2
{
public:
	explicit MemoryCell2 (const Object & initialValue = {});
	const Object & read () const;
	void write (const Object & v);

private:
	Object m_storedValue;
};

# endif // MemoryCell2_H_13
