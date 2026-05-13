# include <iostream>
# include <vector>



struct IHaveATypedef {
	typedef int MyTypedef;
};

struct IHaveATemplateTypedef {
	template <typename T>
	using MyTemplateTypedef = std::vector <T>;
};

template <typename T>
struct Helper {
	T data;
};

struct IHaveTypedefs {
	using MyTypedef = IHaveATypedef::MyTypedef;
	typedef Helper <MyTypedef> MyTypedefHelper;
};

IHaveTypedefs::MyTypedef i;
IHaveTypedefs::MyTypedefHelper hi;

typedef IHaveTypedefs::MyTypedef TypedefBeFree;
TypedefBeFree ii;



struct TypedefAccessLevels {
private:
	typedef int PrvInt;
protected:
	typedef int PrtInt;
public:
	typedef int PubInt;
};

struct TypedefAccessLevelsDerivedPublic : TypedefAccessLevels {
	// PrvInt i1; // error
	PrtInt i2;
	PubInt i3;
};

struct TypedefAccessLevelsDerivedProtected : protected TypedefAccessLevels {
	// PrvInt i1; // error
	PrtInt i2;
	PubInt i3;
};

struct TypedefAccessLevelsDerivedPrivate : private TypedefAccessLevels {
	// PrvInt i1; // error
	PrtInt i2;
	PubInt i3;
};

// TypedefAccessLevels::PrvInt i1; // error
// TypedefAccessLevels::PrtInt i2; // error
TypedefAccessLevels::PubInt i3;

// TypedefAccessLevelsDerivedPrivate::PrvInt i11; // error
// TypedefAccessLevelsDerivedPrivate::PrtInt i12; // error
// TypedefAccessLevelsDerivedPrivate::PubInt i13; // error

// TypedefAccessLevelsDerivedProtected::PrvInt i21; // error
// TypedefAccessLevelsDerivedProtected::PrtInt i22; // error
// TypedefAccessLevelsDerivedProtected::PubInt i23; // error

// TypedefAccessLevelsDerivedPublic::PrvInt i31; // error
// TypedefAccessLevelsDerivedPublic::PrtInt i32; // error
TypedefAccessLevelsDerivedPublic::PubInt i33; // error



struct SomeComplexType {};
struct SomeOtherComplexType {};

class Something {
private:
	SomeOtherComplexType m_memb;

public:
	typedef SomeComplexType MyHelper;
	friend MyHelper;

	typedef decltype (m_memb) MyOtherHelper;

	MyHelper getHelper () const { return MyHelper {}; }

	MyOtherHelper & getOtherHelper () { return m_memb; }
};



template <typename T>
struct TemplateTypedef {
	typedef T type;
};

TemplateTypedef <int>::type tt;



template <typename T, std::size_t SZ, std::size_t D>
struct Array {};

template <typename T, std::size_t SZ>
struct OneDArray {
	typedef Array <T, SZ, 1> type;
};

template <typename T, std::size_t SZ>
using TwoDArrayT = Array <T, SZ, 2>;



int main () {
	static IHaveATemplateTypedef::MyTemplateTypedef <
		IHaveATypedef::MyTypedef
	> int_vector;
}
