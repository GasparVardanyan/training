# include <type_traits>

template <typename T> struct RemoveReference { using type = T; };
template <typename T> struct RemoveReference <T &> { using type = T; };
template <typename T> struct RemoveReference <T &&> { using type = T; };

template <typename T>
struct IsLvalueReference : std::false_type {};

template <typename T>
struct IsLvalueReference <T &> : std::true_type {};

template <typename T>
using RemoveReferenceT = RemoveReference <T>::type;

template <typename T>
inline constexpr bool IsLvalueReferenceV = IsLvalueReference <T>::value;

template <typename T>
constexpr RemoveReferenceT <T> && Move (T && value) noexcept {
	return static_cast <RemoveReferenceT <T> &&> (value);
}

template <typename T>
constexpr RemoveReferenceT <T> & Forward (RemoveReferenceT <T> & value) noexcept {
	return static_cast <RemoveReferenceT <T> &> (value);
}

template <typename T>
constexpr RemoveReferenceT <T> && Forward (RemoveReferenceT <T> && value) noexcept {
	static_assert (false == IsLvalueReferenceV <T>, "don't do stupid things");
	return static_cast <RemoveReferenceT <T> &&> (value);
}



// # include "../dsa/verbose_class.h"

# include <iostream>
# include <vector>
# include <string>
# include <algorithm>

class C {
public:
	C () {
		std::cout << "called " << makeLabel (this) << " C () " << std::endl;
	}

	C (int x) {
		if (x) {}
		std::cout << "called " << makeLabel (this) << " C (int) " << std::endl;
	}

	C (const C & o) {
		std::cout << "called C (const C &) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
	}

	C (C && o) noexcept {
		std::cout << "called C (C &&) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
	}

	C & operator= (const C & o) {
		std::cout << "called C & operator=(const C &) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
		return *this;
	}

	C & operator= (C && o) noexcept {
		std::cout << "called C & operator=(C &&) " << makeLabel (this)  << " (" << makeLabel (& o) << ')' << std::endl;
		return *this;
	}

	virtual ~C () {
		std::cout << "called ~C () [" << makeLabel (this)  << ']' << std::endl;
	}

public:
	static int maxIndex () {
		return (int) s_ptrs.size () - 1;
	}

private:
	static inline std::vector <const C *> s_ptrs;
	static std::string makeLabel (const C * c) {
		if (s_ptrs.end () == std::find(s_ptrs.begin (), s_ptrs.end (), c)) {
			s_ptrs.push_back(c);
		}

		return "ID" + std::to_string (std::find (s_ptrs.begin (), s_ptrs.end (), c) - s_ptrs.begin ());
	}
};



int main () {
	C a;
	C b = Forward <C&> (a);
	b = Move (a);
}
