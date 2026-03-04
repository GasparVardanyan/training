# include <iostream>

class MyClass {
public:
	int m_publicValue = 22;
	friend void friend_function1 ();
protected:
	int m_protectedValue = 22;
	friend void friend_function2 ();
private:
	int m_privateValue = 22;
	friend void friend_function3 ();
};

class MyClass1 : public MyClass {
public:
	int m_publicValue1 = 22;
protected:
	int m_protectedValue1 = 22;
private:
	int m_privateValue1 = 22;
};

class MyClass2 : public MyClass {
public:
	int m_publicValue2 = 22;
protected:
	int m_protectedValue2 = 22;
private:
	int m_privateValue2 = 22;
};

class MyClass3 : public MyClass {
public:
	int m_publicValue3 = 22;
protected:
	int m_protectedValue3 = 22;
private:
	int m_privateValue3 = 22;
};
void friend_function1 () {
	MyClass c;
	std::cout << c.m_privateValue << std::endl;
	std::cout << c.m_protectedValue << std::endl;
	std::cout << c.m_publicValue << std::endl;

	MyClass1 c1;
	std::cout << c1.m_privateValue << std::endl;
	std::cout << c1.m_protectedValue << std::endl;
	std::cout << c1.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c1.m_privateValue1 << std::endl; // error
	// std::cout << c1.m_protectedValue1 << std::endl; // error
	std::cout << c1.m_publicValue1 << std::endl; // error

	MyClass2 c2;
	std::cout << c2.m_privateValue << std::endl;
	std::cout << c2.m_protectedValue << std::endl;
	std::cout << c2.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c2.m_privateValue2 << std::endl; // error
	// std::cout << c2.m_protectedValue2 << std::endl; // error
	std::cout << c2.m_publicValue2 << std::endl; // error

	MyClass3 c3;
	std::cout << c3.m_privateValue << std::endl;
	std::cout << c3.m_protectedValue << std::endl;
	std::cout << c3.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c3.m_privateValue3 << std::endl; // error
	// std::cout << c3.m_protectedValue3 << std::endl; // error
	std::cout << c3.m_publicValue3 << std::endl;
}

void friend_function2 () {
	MyClass c;
	std::cout << c.m_privateValue << std::endl;
	std::cout << c.m_protectedValue << std::endl;
	std::cout << c.m_publicValue << std::endl;

	MyClass1 c1;
	std::cout << c1.m_privateValue << std::endl;
	std::cout << c1.m_protectedValue << std::endl;
	std::cout << c1.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c1.m_privateValue1 << std::endl; // error
	// std::cout << c1.m_protectedValue1 << std::endl; // error
	std::cout << c1.m_publicValue1 << std::endl; // error

	MyClass2 c2;
	std::cout << c2.m_privateValue << std::endl;
	std::cout << c2.m_protectedValue << std::endl;
	std::cout << c2.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c2.m_privateValue2 << std::endl; // error
	// std::cout << c2.m_protectedValue2 << std::endl; // error
	std::cout << c2.m_publicValue2 << std::endl; // error

	MyClass3 c3;
	std::cout << c3.m_privateValue << std::endl;
	std::cout << c3.m_protectedValue << std::endl;
	std::cout << c3.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c3.m_privateValue3 << std::endl; // error
	// std::cout << c3.m_protectedValue3 << std::endl; // error
	std::cout << c3.m_publicValue3 << std::endl;
}

void friend_function3 () {
	MyClass c;
	std::cout << c.m_privateValue << std::endl;
	std::cout << c.m_protectedValue << std::endl;
	std::cout << c.m_publicValue << std::endl;

	MyClass1 c1;
	std::cout << c1.m_privateValue << std::endl;
	std::cout << c1.m_protectedValue << std::endl;
	std::cout << c1.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c1.m_privateValue1 << std::endl; // error
	// std::cout << c1.m_protectedValue1 << std::endl; // error
	std::cout << c1.m_publicValue1 << std::endl; // error

	MyClass2 c2;
	std::cout << c2.m_privateValue << std::endl;
	std::cout << c2.m_protectedValue << std::endl;
	std::cout << c2.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c2.m_privateValue2 << std::endl; // error
	// std::cout << c2.m_protectedValue2 << std::endl; // error
	std::cout << c2.m_publicValue2 << std::endl; // error

	MyClass3 c3;
	std::cout << c3.m_privateValue << std::endl;
	std::cout << c3.m_protectedValue << std::endl;
	std::cout << c3.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c3.m_privateValue3 << std::endl; // error
	// std::cout << c3.m_protectedValue3 << std::endl; // error
	std::cout << c3.m_publicValue3 << std::endl;
}

void non_friend_function () {
	MyClass c;
	// std::cout << c.m_privateValue << std::endl;
	// std::cout << c.m_protectedValue << std::endl;
	std::cout << c.m_publicValue << std::endl;

	MyClass1 c1;
	// std::cout << c1.m_privateValue << std::endl;
	// std::cout << c1.m_protectedValue << std::endl;
	std::cout << c1.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c1.m_privateValue1 << std::endl; // error
	// std::cout << c1.m_protectedValue1 << std::endl; // error
	std::cout << c1.m_publicValue1 << std::endl; // error

	MyClass2 c2;
	// std::cout << c2.m_privateValue << std::endl;
	// std::cout << c2.m_protectedValue << std::endl;
	std::cout << c2.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c2.m_privateValue2 << std::endl; // error
	// std::cout << c2.m_protectedValue2 << std::endl; // error
	std::cout << c2.m_publicValue2 << std::endl; // error

	MyClass3 c3;
	// std::cout << c3.m_privateValue << std::endl;
	// std::cout << c3.m_protectedValue << std::endl;
	std::cout << c3.m_publicValue << std::endl;
	// parent's friend functions get access only members inherited from the parent
	// std::cout << c3.m_privateValue3 << std::endl; // error
	// std::cout << c3.m_protectedValue3 << std::endl; // error
	std::cout << c3.m_publicValue3 << std::endl;
}


int main () {
	friend_function1 ();
	friend_function2 ();
	friend_function3 ();
}
