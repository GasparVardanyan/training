# include <iostream>

class Accesser {
public:
	void private_accesser1 ();
	void private_accesser2 ();
};

class PrivateHolder {
	friend class Accesser;
public:
	explicit PrivateHolder (int val)
		: m_value (val) {}

private:
	int m_value;
};

void Accesser::private_accesser1 () {
	PrivateHolder h (10);
	std::cout << h.m_value << std::endl;
}

void Accesser::private_accesser2 () {
	PrivateHolder h (10);
	std::cout << h.m_value << std::endl;
}

int main () {
	Accesser a;
	a.private_accesser1 ();
	a.private_accesser2 ();
}
