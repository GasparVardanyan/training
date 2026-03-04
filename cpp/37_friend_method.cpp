# include <iostream>

class Accesser {
public:
	void private_accesser ();
	void not_accesser ();
};

class PrivateHolder {
	friend void Accesser::private_accesser ();
public:
	PrivateHolder (int val)
		: m_privateValue (val) {}

private:
	int m_privateValue;
};

void Accesser::private_accesser () {
	PrivateHolder ph (10);
	std::cout << ph.m_privateValue << std::endl;
}

void Accesser::not_accesser () {
	PrivateHolder ph (10);
	// std::cout << ph.m_privateValue << std::endl; // error
}

int main () {
	Accesser a;
	a.private_accesser ();
}
