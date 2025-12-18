# include <iostream>
# include <string>

template <typename Object>
class MemoryCell
{
public:
	explicit MemoryCell (const Object & defaultValue = {})
		 : m_storedValue {defaultValue}
	{}

	const Object & read () const {
		return m_storedValue;
	}

	void write (const Object & obj) {
		m_storedValue = obj;
	}

private:
	Object m_storedValue;
};

int main ()
{
	MemoryCell<int> intCell;
	MemoryCell<std::string> strCell {"hello"};
	std::cout << "hello, world!" << std::endl;
}
